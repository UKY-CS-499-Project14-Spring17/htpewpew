/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

cli.c (short for sommand line input) is the C file that handles 
the output for the program. The functions here set the verbose
and silent options and contain the wrappers that format the output
text depending on whether it is a warning, error, or just a note.
*/
#define CLI_C

#include "cli.h"

//Set the silent option and check if verbose was already set.
//If so report to the user and exit the program.
void set_silent() {
  silent = 1;
  if( verbose ) {
    ferr("Cannot be both silent and verbose\n");
    exit(1);
  }
}

//Set the verbose option and check if silent was already set.
//If so report to the user and exit the program.
void set_verbose() {
  verbose = 1;
  if( silent ) {
    ferr("Cannot be both silent and verbose\n");
    exit(1);
  }
}

//A wrapper for vfprintf that checks to see if the program
//is supposed to be silent before outputting to the user.
void fmsg(char* s_, ...) {
  if( silent == 0 ) {
    va_list args;
    va_start(args, s_);
    vfprintf(stdout, s_, args);
    va_end(args);
  }
}

//Reformats the output to stdout to highlight notes to the user.
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

//Reformats output to stderr to highlight warnings to the user.
void fwarn(char* s_, ...) {
  fprintf(stderr, KBLD KYEL "Warning: ");
  va_list args;
  va_start(args, s_);
  vfprintf(stderr, s_, args);
  va_end(args);
  fprintf(stderr, KNRM);
}

//Reformats output to stderr to higlight errors to the user.
void ferr(char* s_, ...) {
  fprintf(stderr, KBLD KRED "Error: ");
  va_list args;
  va_start(args, s_);
  vfprintf(stderr, s_, args);
  va_end(args);
  fprintf(stderr,KNRM);
}

