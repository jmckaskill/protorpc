
// websocket messages come in a series of frames
// these frames have no guaranteed correlation with the packets sent by the user (though most of the time they do)
// the incoming data stream is a series of json messages separated by a newline
// or 2 byte BE length followed by proto messages
// we have the following variables:
// m_reader.buf the full buffer
// m_reader.have all the data that we've read from the socket
// m_reader.used packet data that we've removed the framing
// m_nextline offset into the used packet data where the next message (json or proto) starts

static uint16_t read_big_16(uint8_t const *p) {
	return (size_t(p[0]) << 8) | size_t(p[1]);
}

static void write_big_16(uint8_t* p, size_t v) {
	p[0] = uint8_t(v >> 8);
	p[1] = uint8_t(v);
}

#define WS_FIN 0x80
#define WS_OPMASK 0xF
#define WS_CONTINUATION 0x0
#define WS_TEXT 0x1
#define WS_BINARY 0x2
#define WS_CLOSE 0x8
#define WS_PING 0x9
#define WS_PONG 0xA
#define WS_MASK 0x80U

#define WSC_NORMAL 1000
#define WSC_GOAWAY 1001
#define WSC_PROTOCOL_ERROR 1002
#define WSC_INVALID_TYPE 1003
#define WSC_OVERLARGE 1009

#define WS_CONTROL_MAX 125

static bool stream_send_ping(rpc_stream_data *m, uint8_t type, const uint8_t *data, size_t sz) {
	uint8_t buf[WS_CONTROL_MAX + 2];
	buf[0] = WS_FIN | type;
	buf[1] = uint8_t(sz);
	memcpy(&buf[2], data, sz);
	std::unique_lock<sendlock_t> lk(m->sendlk);
	return send(m->reader.fd, (char*) buf, sz + 2, 0) == int(sz + 2);
}

static int stream_send_close(rpc_stream_data *m, uint16_t reason) {
	fprintf(stderr, "send_close %d\n", reason);
	uint8_t buf[4];
	buf[0] = WS_FIN | WS_CLOSE;
	buf[1] = 2;
	buf[2] = uint8_t(reason >> 8);
	buf[3] = uint8_t(reason);
	std::unique_lock<sendlock_t> lk(m->sendlk);
	send(m->reader.fd, (char*) buf, 4, 0);
	shutdown(m->reader.fd, SHUT_RDWR);
	return -1;
}

static int stream_read_frame(const uint8_t *data, size_t sz, uint8_t data_opcode) {
	size_t msgoff = m->reader.used;

	// if we haven't seen anything in a while, send a ping to try and get something back
	if (m->reader.have == msgoff && m->reader.read_timeout() && !stream_send_ping(m, WS_PING, NULL, 0)) {
		return stream_send_close(m, WSC_PROTOCOL_ERROR);
	}

	// if the ping times out then this read will fail
	if (!m->reader.read_size(msgoff + 6)) {
		fprintf(stderr, "read 6 err\n");
		return stream_send_close(m, WSC_PROTOCOL_ERROR);
	}

	uint8_t *p = (uint8_t*) m->reader.buf + msgoff;
	bool have_mask = (p[1] & WS_MASK) != 0;
	bool need_mask = m->server;
	if (have_mask != need_mask) {
		return stream_send_close(m, WSC_PROTOCOL_ERROR);
	}
	uint8_t mask[4];
	size_t len = (p[1] & ~WS_MASK);
	memcpy(mask, p + 2, 4);

	if (len == 126) {
		// 2 byte length
		if (!m->reader.read_size(msgoff + 8)) {
			return stream_send_close(m, WSC_PROTOCOL_ERROR);
		}
		p = (uint8_t*) m->reader.buf + msgoff;
		len = read_big_16(&p[2]);
		memcpy(mask, p + 4, 4);
		if (len < 126) {
			return stream_send_close(m, WSC_PROTOCOL_ERROR);
		}
	} else if (len == 127) {
		// 8 byte is not supported as we can't buffer that much
		return stream_send_close(m, WSC_OVERLARGE);
	}

	uint8_t opcode = p[0] & WS_OPMASK;

	if (opcode >= WS_CLOSE && (!(p[0] & WS_FIN) || len > WS_CONTROL_MAX)) {
		return stream_send_close(m, WSC_PROTOCOL_ERROR);
	}

	// buffer up the reply message at the beginning of the buffer
	m->reader.compact(msgoff);
	if (!m->reader.read_size(msgoff + len)) {
		return stream_send_close(m, WSC_OVERLARGE);
	}

	// unmask the new data
	uint8_t *data = (uint8_t*) m->reader.buf + msgoff;
	if (have_mask) {
		for (size_t i = 0; i < len; i++) {
			data[i] = data[i] ^ mask[i % 4];
		}
	}

	switch (opcode) {
	case WS_PING:
		if (!stream_send_ping(m, WS_PONG, data, len)) {
			return stream_send_close(m, WSC_PROTOCOL_ERROR);
		}
		m->reader.compact(msgoff);
		return 0;
	case WS_PONG:
		m->reader.compact(msgoff);
		return 0;
	case WS_TEXT:
	case WS_BINARY:
		if (opcode != data_opcode) {
			return stream_send_close(m, WSC_INVALID_TYPE);
		}
		return 0;
	case WS_CONTINUATION:
		return 0;
	case WS_CLOSE:
		if (len > 2) {
			return stream_send_close(m, read_big_16(&data[0]));
		} else {
			return stream_send_close(m, WSC_NORMAL);
		}
	default:
		return stream_send_close(m, WSC_PROTOCOL_ERROR);
	}
}


char *rpc_stream_base::recv_json() {
	m_alloc.clear();

	for (;;) {
		char *nl = (char*) memchr(m->reader.buf + m->nextline, '\n', m->reader.used - m->nextline);
		if (nl) {
			char *ret = m->reader.buf + m->nextline;
			*nl = '\0';
			fprintf(stderr, "ret %s\n", ret);
			m->nextline = nl - m->reader.buf + 1;
			return ret;
		}

		// clear out previous returned frames
		m->reader.compact(m->reader.used - m->nextline);
		m->nextline = 0;

		if (stream_read_frame(m, WS_TEXT)) {
			return NULL;
		}
	}
}


bool rpc_stream_base::recv_proto(pb_bytes *ret) {
	m_alloc.clear();

	for (;;) {
		if (m->reader.used - m->nextline >= 2) {
			size_t msgsz = read_big_16((uint8_t*) &m->reader.buf[m->nextline]);
			if (m->nextline + 2 + msgsz < m->reader.have) {
				ret->setref((uint8_t*) &m->reader.buf[m->nextline + 2], msgsz);
				m->nextline += 2 + msgsz;
				return true;
			}
		}

		// clear out previous returned frames
		m->reader.compact(m->reader.used - m->nextline);
		m->nextline = 0;

		if (stream_read_frame(m, WS_TEXT)) {
			return NULL;
		}
	}
}
