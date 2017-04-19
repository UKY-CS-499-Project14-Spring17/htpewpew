#include "helpers.h"

void ttext(char* s_, ...) {
  fprintf(stdout, KNRM);
  va_list args;
  va_start(args, s_);
  vfprintf(stdout, s_, args);
  va_end(args);
  fprintf(stdout, KNRM);
}

void tmsg(char* s_, ...) {
  fprintf(stdout, KBLD KBLU);
  va_list args;
  va_start(args, s_);
  vfprintf(stdout, s_, args);
  va_end(args);
  fprintf(stdout, KNRM);
}

void tfail(char* s_, ...) {
  fprintf(stdout, KBLD KRED " FAIL ");
  va_list args;
  va_start(args, s_);
  vfprintf(stdout, s_, args);
  va_end(args);
  fprintf(stdout, KNRM);
}

void tpass(char* s_, ...) {
  fprintf(stdout, KBLD KGRN " PASS ");
  va_list args;
  va_start(args, s_);
  vfprintf(stdout, s_, args);
  va_end(args);
  fprintf(stdout, KNRM);
}

