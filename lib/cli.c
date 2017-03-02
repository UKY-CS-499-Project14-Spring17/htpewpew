#define CLI_C

#include "cli.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void set_silent() {
  silent = true;
}

void set_verbose() {
  verbose = true;
}

char* capture_cli_string(int* argx, int argc, char** argv, char* err_msg, char* warn_msg) {
  *argx = *argx + 1; // step forward to next field
  char* arg = argv[*argx];
  if( *argx < argc ) { // check that argument is in bounds
    if( arg[0] == '-') { // if it starts with a -*
      // it's probably a option, this is not what we want
      fwarn(warn_msg);
    }
    return(arg);
  } else {
    ferr(err_msg);
    helpmac();
    exit(1);
  }
}

int capture_cli_int(int* argx, int argc, char** argv, char* err_msg) {
  *argx = *argx + 1; // step forward to next field
  char* arg = argv[*argx];
  char* old_arg = argv[*argx - 1];
  if( *argx < argc ) { // check that argument is in bounds
    if( arg[0] == '-') { // if it starts with a -*
      // it's probably a option, this is not what we want
      ferr("Argument %s %s\n", old_arg, err_msg);
      helpmac();
      exit(1);
    }
    return( atoi(arg) );
  } else {
    ferr("Argument %s %s\n", old_arg, err_msg);
    helpmac();
    exit(1);
  }
}

void help(char* selfname) {
  fprintf(stdout,
"htpewpew: a CLI for serial communication to a HTPOW laser engraver\n\
usage: %s image [options] \n\n\
ENGRAVER SETTINGS\n\
	-b, --burn-time t       	Set the maximum burn time (default = ? ms)\n\
	-d, --dry-run           	Show engraving box and do not engrave\n\
	-i, --intensity s       	Set the maximum burn intensity (default = ?)\n\
	-o, --output image      	Store the altered image at this location\n\
	-p, --port serial-port 		Send the file to this serial port.\n\
	-t, --bw, --threshold x%%	Use a threshold with x%% (default = 50%%)\n\
	-x, --x-offset x        	Set x offset for image\n\
	-y, --y-offset y        	Set y offset for image\n\n\
COMMAND LINE SETTINGS\n\
	-s, --silent            	Supress all messages (warnings and errors continue)\n\
	-v, --verbose           	Enable notes (warnings and errors on by default)\n\
	-h, --help              	Display this help message and quit\n\n",selfname);
// TODO: gcode
//	-g, --gc, --gcode gcode		Send this gcode file to the engraver.
}

void fmsg(char* s_, ...) {
  if( !silent ) {
    va_list args;
    va_start(args, s_);
    vfprintf(stderr, s_, args);
    va_end(args);
  }
}

void fnote(char* s_, ...) {
  if( verbose ) {
    fprintf(stderr, KBLD KCYN "Note: ");
    va_list args;
    va_start(args, s_);
    vfprintf(stderr, s_, args);
    va_end(args);
    fprintf(stderr, KNRM);
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

