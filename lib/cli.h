#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include "options.h"

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

// I added this function because we needed the same method
// with only slight variations several times. Could be improved. ~Grant
char* capture_cli_string(int* argx, int argc, char** argv, char* err_msg, char* warn_msg);
int capture_cli_int(int* argx, int argc, char** argv, char* err_msg);
void help(char* progname);
void fnote(char* s_, ...);
void fwarn(char* s_, ...);
void ferr(char* s_, ...);

#ifdef CLI_C
static bool verbose = false, silent = false;
#endif

#endif
