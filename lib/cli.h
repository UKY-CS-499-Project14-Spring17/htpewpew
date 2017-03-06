#ifndef CLI_H
#define CLI_H

#include "options.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define KNRM  "\x1B[0m"
#define KBLD  "\x1B[1m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define helpmac() help(argv[0])

void set_silent();
void set_verbose();

void fmsg(char* s_, ...);
void fnote(char* s_, ...);
void fwarn(char* s_, ...);
void ferr(char* s_, ...);

#ifdef CLI_C
static int verbose = 0, silent = 0;
#endif

#endif
