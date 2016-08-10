#include "protoc-gen-protorpc.h"

#ifdef WIN32
#include <windows.h>
#define putenv _putenv
#else
#include <unistd.h>
#endif

int exec_protoc(char *argv0, char *argv1) {
    // add our directory to the path
    char *slash = strrchr(argv0, '/');
	char *bslash = strrchr(argv0, '\\');
    if (slash || bslash) {
        str_t path = STR_INIT;
        str_add(&path, "PATH=");
        str_add(&path, getenv("PATH"));
#ifdef WIN32
        str_addch(&path, ';');
#else
        str_addch(&path, ':');
#endif
        str_add2(&path, argv0, (int) ((bslash ? bslash : slash) - argv0));
        // putenv doesn't copy the input, so don't free the string
		putenv(path.buf);
    }
#ifdef WIN32
    str_t cmdline = STR_INIT;
    str_add(&cmdline, "toolchain\\host\\bin\\protoc -I. --protorpc_out=. --error_format=msvs \"");
    str_add(&cmdline, argv1);
    str_add(&cmdline, "\"");

    PROCESS_INFORMATION pi;
    STARTUPINFO si = {0};
    si.cb = sizeof(si);
    CreateProcessA(NULL, cmdline.buf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code;
    GetExitCodeProcess(pi.hProcess, &code);
    // don't bother cleaning up as this function isn't guaranteed to return
    return (int) code;
#else
    execlp("protoc", "protoc", "-I.", "--protorpc.exe_out=.", argv1, NULL);
    perror("exec protoc");
    return 1;
#endif
}
