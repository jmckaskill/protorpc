#include "protoc-gen-protorpc.h"

#ifdef WIN32
#include <windows.h>
#define putenv _putenv
#else
#include <unistd.h>
#endif

int exec_protoc(char *my_exe, char *protoc_exe, char *file) {
    // add our directory to the path
    char *slash = strrchr(my_exe, '/');
	char *bslash = strrchr(my_exe, '\\');
    if (slash || bslash) {
        str_t path = STR_INIT;
        str_add(&path, "PATH=");
        str_add(&path, getenv("PATH"));
        #ifdef WIN32
        str_addch(&path, ';');
        #else
        str_addch(&path, ':');
        #endif
        str_add2(&path, my_exe, (int) ((bslash ? bslash : slash) - my_exe));
        // putenv doesn't copy the input, so don't free the string
		putenv(path.buf);
    }
#ifdef WIN32
    str_t cmdline = STR_INIT;
	str_addch(&cmdline, '"');
	str_add(&cmdline, protoc_exe);
    str_add(&cmdline, "\" -I. --protorpc_out=. --error_format=msvs \"");
    str_add(&cmdline, file);
	str_addch(&cmdline, '"');
	fprintf(stderr, "running %s\n", cmdline.buf);

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
    execlp(protoc_exe, protoc_exe, "-I.", "--protorpc.exe_out=.", file, NULL);
    perror("exec protoc");
    return 1;
#endif
}
