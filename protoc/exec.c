#include "exec.h"
#include <protorpc/str.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#define putenv _putenv
#else
#include <unistd.h>
#endif

int exec_protoc(char *my_exe, char *protoc_exe, char **files, int num) {
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
		putenv(path.c_str);
    }

	str_t outarg = STR_INIT;
	str_add(&outarg, "--");
	if (bslash) {
		str_add(&outarg, bslash + strlen("\\protoc-gen-"));
	} else if (slash) {
		str_add(&outarg, slash + strlen("/protoc-gen-"));
	} else {
		str_add(&outarg, my_exe + strlen("protoc-gen-"));
	}
	str_add(&outarg, "_out=.");

#ifdef WIN32
    str_t cmdline = STR_INIT;
	str_addf(&cmdline, "\"%s\" -I. -Iprotorpc --error_format=msvs %s", protoc_exe, outarg.c_str);
	for (int i = 0; i < num; i++) {
		str_addf(&cmdline, " \"%s\"", files[i]);
	}

    PROCESS_INFORMATION pi;
    STARTUPINFO si = {0};
    si.cb = sizeof(si);
    CreateProcessA(NULL, cmdline.c_str, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code;
    GetExitCodeProcess(pi.hProcess, &code);
    return (int) code;
#else
	const char **args = (const char*)calloc(5 + num, sizeof(char*));
	args[0] = protoc_exe;
	args[1] = "-I.";
	args[2] = "-Iprotorpc";
	args[3] = outarg.c_str;
	for (int i = 0; i < num; i++) {
		args[i + 4] = files[i];
	}
	args[num + 4] = NULL;
	execvp(protoc_exe, args);
    perror("exec protoc");
    return 1;
#endif
}
