#define CLI_C

#include "cli.h"

void set_silent() {
  silent = 1;
  if( verbose ) {
    ferr("Cannot be both silent and verbose\n");
    exit(1);
  }
}

void set_verbose() {
  verbose = 1;
  if( silent ) {
    ferr("Cannot be both silent and verbose\n");
    exit(1);
  }
}

void fmsg(char* s_, ...) {
  if( silent == 0 ) {
    va_list args;
    va_start(args, s_);
    vfprintf(stdout, s_, args);
    va_end(args);
  }
}

void fnote(char* s_, ...) {
  if( verbose ) {
    fprintf(stdout, KBLD KCYN "Note: ");
    va_list args;
    va_start(args, s_);
    vfprintf(stdout, s_, args);
    va_end(args);
    fprintf(stdout, KNRM);
  }
}

void fwarn(char* s_, ...) {
  fprintf(stderr, KBLD KYEL "Warning: ");
  va_list args;
  va_start(args, s_);
  vfprintf(stderr, s_, args);
  va_end(args);
  fprintf(stderr, KNRM);
}

void ferr(char* s_, ...) {
  fprintf(stderr, KBLD KRED "Error: ");
  va_list args;
  va_start(args, s_);
  vfprintf(stderr, s_, args);
  va_end(args);
  fprintf(stderr,KNRM);
}

