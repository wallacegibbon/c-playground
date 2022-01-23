#ifndef __SYSUTIL_H__
#define __SYSUTIL_H__

#include <stdio.h>
#include <stdlib.h>

void exit_info(int code, char *info) {
	fprintf(stderr, "exit (code: %d) info: ~s~n", code, info);
	exit(code);
}

#endif
