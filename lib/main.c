#include "image.h"
#include "options.h"
#include "cli.h"
#include <string.h>

const struct HTPewPewOptions HTPEWPEW_DEFAULT_OPTS = {  "" /* filename */,
"" /* output */, "" /* portname */, false /* is_image */, false /* dry_run */,
-1 /* threshold */, 60 /* TODO burn_time */, 10 /* TODO intensity */,
false /* offset */, 0 /* x_offset */, 0 /* y_offset */ };

// main
// inputs:
//    argc -- int    -- number of argument inputs
//    argv -- argv** -- array of argument inputs
// outputs:
//    exit -- int    -- exit status of program
// side effects:
//    sorts argv and constructs struct of option data
//    can exit early if instructions are not understood
//    can exit with status 0 if '-h' or '--help' are used
int main(int argc,char **argv)
{
  char* arg, * old_arg;
  struct HTPewPewOptions options = HTPEWPEW_DEFAULT_OPTS;
  // check each argument, prepare data
  for( int argx = 1; argx < argc; argx = argx + 1 ) {
    arg = argv[argx];
    old_arg = argv[argx];

    if( strcmp(arg,"-b") == 0 ||
        strcmp(arg,"--burn-time") == 0 ) { // set burn (dwell) time
      // TODO confirm max dwell time
      options.burn_time = capture_cli_int(&argx, argc, argv, "requires an integer 0-200");
      fnote("Burn time set to %d.\n", options.burn_time);

    } else if( strcmp(arg, "-d") == 0 ||
               strcmp(arg, "--dry-run") == 0 ) { // enable dry run mode
      options.dry_run = true;
      fnote("Dry run mode enabled.\n");

    } else if( strcmp(arg, "-i") == 0 ||
               strcmp(arg, "--intensity") == 0 ) { // set intensity
      // TODO confirm max intensity
      options.intensity = capture_cli_int(&argx, argc, argv, "requires an integer 0-10");
      fnote("Intensity set to %d.\n", options.intensity);

    } else if( strcmp(arg, "-o") == 0 ||
        strcmp(arg, "--output") == 0 ) { // set output destination
// TODO combine -o and -p with a new cli function. They're similar
      argx = argx + 1; // step forward to next field
      arg = argv[argx];
      if( argx < argc ) { // check that argument is in bounds
        if( arg[0] == '-') { // if it starts with a -*
        // it's probably a option, this is not what we want
          fwarn("You probably don't want a file named %s. Proceeding anyways.\n",argv[argx]);
        } else {
          fnote("Image output set to %s.\n", arg);
        }
        strcpy(options.output, arg);
      } else {
        ferr("Argument %s requires a filename for output.\n", old_arg);
        helpmac();
        exit(1);
      }

    } else if( strcmp(arg, "-p") == 0 ||
               strcmp(arg, "--port") == 0 ) { // set serial port
      argx = argx + 1; // step forward to next field
      arg = argv[argx];
      if( argx < argc ) { // check that argument is in bounds
        if( arg[0] != '/') { // if it starts with a -*
        // it's probably not a filepath, this is not what we want
          fwarn("You probably don't want a serial port named %s. Proceeding anyways.\n",argv[argx]);
        } else {
          fnote("Port set to %s.\n", arg);
        }
        strcpy(options.portname, arg);
      } else {
        ferr("Argument %s requires a serial port (i.e. /dev/ttyUSB0) for connection.\n", old_arg);
        helpmac();
        exit(1);
      }

    } else if( strcmp(arg, "-t") == 0 ||
               strcmp(arg, "--bw") == 0 ||
               strcmp(arg, "--threshold") == 0 ) { // set threshold mode
      argx = argx + 1; // step forward to next field
      arg = argv[argx];
      if( argx < argc ) { // check that argument is in bounds
        if( arg[0] == '-') { // if it starts with a -*
          // it's probably a option, this is not what we want
          fnote("Threshold set to 50%% by default.\n");
          options.threshold = 50;
          argx = argx - 1; // back up one step
        } else {
          // convert this input to a number
          options.threshold = atoi(arg);
          // TODO what to do about > 100%
          fnote("Threshold set to %i%%.\n", options.threshold);
        }
      } else {
        fnote("Threshold set to 50%% by default.\n");
        options.threshold = 50;
      }

    } else if( strcmp(arg, "-x") == 0 ||
               strcmp(arg, "--x-offset") == 0 ) { // set x offset
      // TODO confirm max intensity
      options.x_offset = capture_cli_int(&argx, argc, argv, "requires an integer 0-10");
      options.offset = true;
      fnote("X offset set to %d.\n", options.x_offset);

    } else if( strcmp(arg, "-y") == 0 ||
               strcmp(arg, "--y-offset") == 0 ) { // set y offset
      // TODO confirm max intensity
      options.y_offset = capture_cli_int(&argx, argc, argv, "requires an integer 0-10");
      options.offset = true;
      fnote("Y offset set to %d.\n", options.y_offset);

    } else if( strcmp(arg, "-h") == 0 ||
               strcmp(arg, "--help") == 0 ) { // display help and exit
      helpmac();
      exit(0);

    } else if( arg[0] == '-' ) { // known argument
      ferr("Argument %s not understood.\n", arg);
      helpmac();
      exit(1);

    } else { // probably the input filename
      if( strcmp(options.filename, "") == 0) {
        // filename not yet set
        strcpy(options.filename, arg);
      } else {
        // filename already set
        fnote("Multiple image burn not yet supported. It may never be.\n");
        ferr("File %s already given, but additional file %s supplied.\n", options.filename, arg);
        helpmac();
        exit(1);
      }
    }
  } // all arguments checked
  if( strcmp(options.filename,"") == 0) {
    ferr("Filename not given. Not sure how to proceed.\n");
    helpmac();
    exit(1);
  } else {
    prepare_image(options);
  }
  exit(0);
}

