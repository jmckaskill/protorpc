#include <os-proc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static void hex_dump(FILE *f, uint8_t *buf, unsigned n) {
    for (unsigned off = 0; off < n; off += 0x10) {
        fprintf(f, "%02x: ", off);
        for (unsigned i = off; i < off + 0x10; i++) {
            if ((i % 2) == 0) {
                fputc(' ', f);
            }
            if (i < n) {
                const char *hex = "0123456789abcdef";
                fputc(hex[buf[i] >> 4], f);
                fputc(hex[buf[i] & 0xF], f);
            } else {
                fputc(' ', f);
                fputc(' ', f);
            }
        }

        fputc(' ', f);
        fputc(' ', f);

        for (unsigned i = off; i < off + 0x10 && i < n; i++) {
            if (0x20 <= buf[i] && buf[i] < 0x7F) {
                fputc(buf[i], f);
            } else {
                fputc('.', f);
            }
        }

        fputc('\n', f);
    }
}

// this program is run as follows:
// ./run-test [exe] [input] [output] -o [log-file]
// it then runs exe with input as its standard input
// and records the output
// it will fail if the output does not match the contents of
// the [output] file given as an argument

int main(int argc, char *argv[]) {
    FILE *log = stderr;
    if (argc < 4) {
        fprintf(stderr, "./run-test.exe [exe] [input-file] [expected-output-file] -o [log file]\n");
        return 2;
    }
    if (argc > 5 && !strcmp(argv[4], "-o")) {
        log = fopen(argv[5], "w");
        if (!log) {
            fprintf(stderr, "failed to open log file %s\n", argv[5]);
            return 2;
        }
    }

    struct os_proc proc = OS_PROC_INIT;
    proc.err = pipe_from_file(log);

	FILE *input = fopen(argv[2], "rb");
    if (!input) {
        fprintf(log, "failed to open input file %s\n", argv[2]);
        return 2;
    }
	proc.in = pipe_from_file(input);

    FILE *expected = fopen(argv[3], "rb");
    if (!expected) {
        fprintf(log, "failed to open output file %s\n", argv[3]);
        return 2;
    }

    pipe_t output;
    pipe_open(&output, &proc.out);

    const char *child[2];
	child[0] = argv[1];
	child[1] = NULL;

    if (proc_start(&proc, child, PROC_NEW_GROUP)) {
        fprintf(log, "failed to start child process %s\n", argv[1]);
        return 2;
    }
    pipe_close(proc.out);
	fclose(input);

    bool ok = true;

    for (;;) {
        uint8_t have[256], want[256];
        int hr = pipe_read(output, 3000, have, sizeof(have));
        fprintf(log, "pipe finished with %d\n", hr);
        int wr = (int) fread(want, 1, hr > 0 ? hr : sizeof(have), expected);
        fprintf(log, "expected read returned %d\n", wr);
        if (wr != hr) {
            fprintf(log, "pipe returned different amount than expected\n");
            fprintf(log, "have %d\n", hr);
            if (hr > 0) {
                hex_dump(log, have, (unsigned) hr);
            }
            fprintf(log, "expected %d\n", wr);
            if (wr > 0) {
                hex_dump(log, want, (unsigned) wr);
            }
            ok = false;
            break;
        }
        if (wr <= 0) {
            ok = true;
            break;
        }
        if (memcmp(have, want, hr)) {
            fprintf(log, "pipe returned different output than expected\n");
            fprintf(log, "have\n");
            hex_dump(log, have, hr);
            fprintf(log, "want\n");
            hex_dump(log, want, wr);
            ok = false;
            break;
        }

        fprintf(log, "pipe and expectation matched - %d bytes\n", hr);
        hex_dump(log, have, hr);

		fflush(log);
    }

    fprintf(log, "killing process\n");
    fflush(log);
    proc_kill_group(&proc);
    return ok ? 0 : 4;
}