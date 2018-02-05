#include <protorpc/http.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <errno.h>
#include <sys/socket.h>
#endif

static inline bool would_block() {
#ifdef _WIN32
	return WSAGetLastError() == WSAEWOULDBLOCK;
#else
	return errno == EWOULDBLOCK || errno == EINTR;
#endif
}

int http_pump(http *h, int fd) {
	int w;
	const char *wbuf = http_send_buffer(h, &w);
	if (w) {
		w = send(fd, wbuf, w, 0);
		if (w < 0 && would_block()) {
			return 0;
		} else if (http_sent(h, w)) {
			return -1;
		}
	} else {
		int r;
		char *rbuf = http_recv_buffer(h, &r);
		r = recv(fd, rbuf, r, 0);
		if (r < 0 && would_block()) {
			return 0;
		} else if (http_received(h, r)) {
			return -1;
		}
	}
	return 1;
}
