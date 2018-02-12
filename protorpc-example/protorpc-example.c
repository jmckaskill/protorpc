#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
typedef WSAPOLLFD pollfd;
#define poll WSAPoll
#pragma comment(lib,"ws2_32")
#else
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#define closesocket(fd) close(fd)
#endif

#include "example.proto.h"
#include <protorpc/flag.h>
#include <protorpc/http.h>
#include <protorpc/ptr-array.h>
#include <protorpc/char-array.h>
#include <protorpc/heap-inl.h>
#include <time.h>
#include <stdio.h>

#define MAX_CLIENTS 16
#define IDLE_TIMEOUT_SECONDS 10

static bool would_block() {
#ifdef _WIN32
	return WSAGetLastError() == WSAEWOULDBLOCK;
#else
	return errno == EWOULDBLOCK || errno == EINTR;
#endif
}

static int set_non_blocking(int fd) {
#ifdef _WIN32
	u_long nonblock = 1;
	return ioctlsocket(fd, FIONBIO, &nonblock);
#else
	return fcntl(fd, F_SETFL, O_NONBLOCK);
#endif
}

static int non_blocking_accept(int sfd) {
#ifdef __linux__
	return accept4(sfd, NULL, NULL, O_NONBLOCK);
#else
	int fd = accept(sfd, NULL, NULL);
	if (fd >= 0) {
		set_non_blocking(fd);
	}
	return fd;
#endif
}

typedef struct client client;
typedef struct server server;

struct client {
	struct heap_node hn;
	http h;
	int fd;
	const proto_method *method;
	char rx[4096];
	char tx[4096];
	time_t timeout;
};

struct server {
	Example svc;
};

static int example_ping(Example *svc, pb_allocator *obj, const EchoRequest *in, EchoRequest *out) {
	*out = *in;
	return 0;
}

static int example_error(Example *svc, pb_allocator *obj, const Empty *in, Empty *out) {
	return 500;
}

extern const proto_dir dir_www_data;

static const char not_found[] = "HTTP/1.1 404 Not Found\r\nContent-Length:0\r\n\r\n";

static int decide_on_dispatch(server *s, client *c) {
	c->method = NULL;

	if (str_itest(c->h.method, "GET")) {
		int len;
		const char *file = pb_lookup_file(&dir_www_data, c->h.path.c_str, c->h.path.len, &len);
		if (file) {
			return http_send_response(&c->h, file, len);
		}
	} else if (str_itest(c->h.method, "POST")) {
		c->method = pb_lookup_method(&s->svc, &proto_Example, c->h.path.c_str, c->h.path.len);
		if (c->method) {
			return http_send_continue(&c->h);
		}
	}

	return http_send_response(&c->h, not_found, sizeof(not_found) - 1);
}

static int open_server(const char *host, const char *port) {
	struct addrinfo hints, *result, *rp;
	int sfd = -1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* TCP socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */

	int err = getaddrinfo(host, port, &hints, &result);
	if (err) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
			rp->ai_protocol);
		if (sfd == -1) {
			continue;
		}

		unsigned long reuse = 1;
		if (!setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(reuse))
		&& !bind(sfd, rp->ai_addr, rp->ai_addrlen)
		&& !listen(sfd, SOMAXCONN)) {
			break;                  /* Success */
		}

		closesocket(sfd);
	}

	if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	return sfd;
}

static int compare_client(const struct heap_node *a, const struct heap_node *b) {
	client *ca = (client*)a;
	client *cb = (client*)b;
	return (int) (ca->timeout - cb->timeout);
}

static void disconnect(client *c) {
	shutdown(c->fd, SHUT_WR);
	closesocket(c->fd);
	free(c);
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

	const char *port = "80";
	flag_string(&port, 0, "port", "HTTP server listen port");
	flag_parse(&argc, argv, "protorpc-example.exe [arguments]", 0);

	int sfd = open_server(NULL, port);
	set_non_blocking(sfd);

	server s = { 0 };
	s.svc.ping = &example_ping;
	s.svc.error = &example_error;

	struct {
		int len;
		client *v[MAX_CLIENTS];
	} clients = { 0 };

	struct heap client_heap = { 0 };
	time_t now = time(NULL);

	for (;;) {		
		int timeoutms;
		for (;;) {
			client *c = (client*)heap_min(&client_heap);
			if (!c) {
				timeoutms = -1;
				break;
			} else if (c->timeout - now <= 0) {
				heap_remove(&client_heap, &c->hn, &compare_client);
				for (int i = 0; i < clients.len; i++) {
					if (clients.v[i] == c) {
						pa_remove(&clients, i);
						break;
					}
				}
				disconnect(c);
			} else {
				timeoutms = (int) ((c->timeout - now) * 1000);
				break;
			}
		}

		struct pollfd fds[MAX_CLIENTS + 1];

		for (int i = 0; i < clients.len; i++) {
			client *c = clients.v[i];
			int wlen;
			http_send_buffer(&c->h, &wlen);

			fds[i].fd = c->fd;
			fds[i].events = wlen ? POLLOUT : POLLIN;
			fds[i].revents = 0;
		}

		fds[clients.len].events = POLLIN;
		fds[clients.len].revents = 0;
		fds[clients.len].fd = sfd;

		int ret = poll(&fds[0], clients.len + ((clients.len < MAX_CLIENTS) ? 1 : 0), timeoutms);
		now = time(NULL);

		if (!ret) {
			continue;
		}

		for (int i = 0; i < clients.len; i++) {
			if (!fds[i].revents) {
				continue;
			}

			client *c = clients.v[i];
			heap_remove(&client_heap, &c->hn, &compare_client);

			int err;
			while ((err = http_pump(&c->h, c->fd)) > 0) {
				if (c->h.state == HTTP_RESPONSE_SENT) {
					if ((err = http_next_request(&c->h)) != 0) {
						break;
					}
					c->method = NULL;
				}

				if (c->h.state == HTTP_HEADERS_RECEIVED) {
					decide_on_dispatch(&s, c);
				}

				if (c->h.state == HTTP_DATA_RECEIVED && c->method) {
					char obj[65536];
					pb_allocator alloc = PB_INIT_ALLOCATOR(obj);
					int rxlen;
					char *req = http_request_data(&c->h, &rxlen);
					int txlen = pb_dispatch(&s.svc, c->method, &alloc, req, rxlen, c->tx, sizeof(c->tx));
					http_send_response(&c->h, c->tx, txlen);
				}
			}

			if (err) {
				pa_remove(&clients, i);
				disconnect(c);
				i--;
			} else {
				c->timeout = now + IDLE_TIMEOUT_SECONDS;
				heap_insert(&client_heap, &c->hn, &compare_client);
			}
		}

		if (fds[clients.len].revents & POLLIN) {
			int fd;
			while (clients.len < MAX_CLIENTS && (fd = non_blocking_accept(sfd)) >= 0) {
				client *c = (client*)calloc(1, sizeof(client));
				http_reset(&c->h, c->rx, sizeof(c->rx), NULL);
				c->fd = fd;
				c->timeout = now + IDLE_TIMEOUT_SECONDS;
				pa_append(&clients, c);
				heap_insert(&client_heap, &c->hn, &compare_client);
			}
		}
	}


}
