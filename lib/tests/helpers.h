#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "../cli.h"

void ttext(char* s_, ...); // use normal text
void tmsg(char* s_, ...);  // blue text
void tfail(char* s_, ...); // red " FAIL " text
void tpass(char* s_, ...); // green " PASS " text

#endif
