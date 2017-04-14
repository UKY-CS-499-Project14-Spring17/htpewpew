/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

main.c handles all of the parsing and interpretation of the user input and calls all of 
the necessary functions from the other C files to complete the engraving based on the 
user input.
*/

#include "cli.h"
#include "image.h"
#include "pixelator.h"
#include "options.h"
#include "streamer.h"
#include <argp.h>

// This file taken and modified from [http://www.crasseux.com/books/ctutorial/argp-example.html]

const char *argp_program_version ="htpewpew 1.0";

const char *argp_program_bug_address = "<TODO@uky.edu>";

// OPTIONS.  Field 1 in ARGP. Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.

static struct argp_option options[] =
{
  {0,           0,   0,           0,                    "ENGRAVER PARAMETERS:"},
  {"burn-time", 'b', "burn",      0,                    "Set maximum burn time (default = ? ms)"},
  {"dry-run",   'd', 0,           0,                    "Edit image, show engraving area, and quit"},
  {"intensity", 'i', "intensity", 0,                    "Set the laser intensity (default = ?)"},
  {"output",    'o', "outfile",   0,                    "Output to OUTFILE instead of to standard output"},
  {"port",      'p', "port",      0,                    "Location of USB serial port"},
  {"threshold", 't', "threshold", OPTION_ARG_OPTIONAL,  "Use a threshold (0-100%) for black and white (default = 50%)"},
  {"bw",        't', "threshold", OPTION_ALIAS,         "Use a threshold (0-100%) for black and white (default = 50%)"},
  {"x-offset",  'x', "x",         0,                    "Automatically offset x location (0-?)"},
  {"y-offset",  'y', "y",         0,                    "Automatically offset y location (0-?)"},
  {0,           0,   0,           0,                    "INTERFACE PARAMETERS:"},
  {"verbose",   'v', 0,           0, "Enables additional debugging notes"},
  {"silent",    's', 0,           0, "Supress all standard messages"},
  {0} // this MUST be the last option in the struct
};

// PARSER. Field 2 in ARGP. Order of parameters: KEY, ARG, STATE.

static error_t parse_verbose (int key, char *arg, struct argp_state *state) // set verbose and silent flags first
{
  //Look through the user input and see if the verbose or silent modes are set first so that the program knows to be 
  //verbose or silent in parsing the user's input.
  switch (key)
    {
    case 'v':
      set_verbose();
      fnote("Verbose mode enabled\n");
      break;
    case 's':
      set_silent();
      fwarn("Silent mode enabled: stdout silenced\n");
    }//TODO Need a default option?
  return 0;
}

static error_t parse_opt (int key, char *arg, struct argp_state *state) // check the other flags
{
  HTPewPewOpts *a = state->input;

  //Look through the user input and see which options are set.
  switch (key)
    {
    //burn-time option. If this is input we need to change the laser engravers burn time setting to 
    //what the user indicates.
    case 'b':
      a->burn = atoi(arg); // TODO validate
      fnote("Burn time set to %i ms\n",a->intensity);
      break;
    //dry-run option. If this is input we need to do all of the steps except for sending the commands 
    //to the engraver. 
    case 'd':
      a->dry = 1;
      fnote("Dry run mode enabled\n",a->intensity);
      break;
    //intensity option. If this is input then we need to change the laser intensity setting to what 
    //the user indicated. The intensity only has arange from 1-10 so report an error if it is out of 
    //range.
    case 'i':
      a->intensity = atoi(arg);
      if( a->intensity < 1 || a->intensity > 10) {
        ferr("Intensity must be between 0-10\n");
    	  argp_usage(state);
      } else {
        fnote("Intensity set to %i\n",a->intensity);
      }
      break;
    //outfile option. If this is input we need to output the altered image to the file indicated by 
    //the user.
    case 'o':
      a->outfile = arg; // TODO validate
      if( a->outfile[0] == '-') {
        fwarn("%s does not look like a path to a file\n", a->outfile);
      }
      fnote("Image output set to %s\n",a->outfile);
      break;
    //port option. If this is input we need to connect to the engraver on a different port than the 
    //default of /dev/ttyUSB0.
    case 'p':
      a->port = arg; // TODO validate
      if( a->port[0] != '/') {
        fwarn("%s does not look like a path to a port (i.e. /dev/ttyUSB0)\n", a->port);
      }
      break;
    //threshold option. If this is input we need to use the threshold value indicated by the user 
    //for converting to back and white instead of the default setting of 50.
    case 't':
      if( arg != NULL) {
        a->threshold = atoi(arg);
        fnote("Setting threshold to %d%%\n", a->threshold);
      } else {
        a->threshold = 50;
        fwarn("Setting threshold to default 50%%\n");
      }
      break;
    //x-offset option. If this is input then the image needs to be shifted in the x direction by 
    //pixel amount indicated by the user. This is used by the streamer if it is set. TODO verify
    case 'x':
      a->x = atoi(arg); // TODO validate // TODO negative numbers
      fnote("Setting x-offset to %d\n", a->x);
      break;
    //y-offset option. If this is input then the image needs to be shifted in the y direction by 
    //pixel amount indicated by the user. This is used by the streamer if it is set. TODO verify
    case 'y':
      a->y = atoi(arg); // TODO validate // TODO negative numbers
      fnote("Setting y-offset to %d\n", a->y);
      break;
    //The verbose and silent options are handled separatley on thier own in parse_verbose().
    case 'v': // skip verbose and silent
    case 's': // these are checked first
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
    	  argp_usage(state);
    	}
      a->infile = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) {
  	    a->shell = 1; // TODO i-shell
	    }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

// ARGS_DOC. Field 3 in ARGP.
// A description of the non-option command-line arguments that we accept.
static char args_doc[] = "infile";

// Program documentation.
static char doc[] = "htpewpew: a CLI for serial communication to a HTPOW laser engraver\n\
NOTE: If no file is supplied, htpewpew will enter interactive shell mode.";

// The ARGP structure itself.
static struct argp argp    = {options, parse_opt,     args_doc, doc};
static struct argp verbose = {options, parse_verbose, args_doc, doc};

/*
   The main function.
   Notice how now the only function call needed to process
   all command-line options and arguments nicely
   is argp_parse.
*/
int main (int argc, char **argv)
{
  HTPewPewOpts options;

  /* Set argument defaults */
  options.burn      = 60; // TODO
  options.dry       = 0;
  options.intensity = 5; // TODO
  options.outfile   = NULL;
  options.port      = NULL;
  options.threshold = -1;
  options.x         = 60; // TODO
  options.y         = 60; // TODO
  options.shell     = 0;

  /* Where the magic happens */
  argp_parse (&verbose, argc, argv, 0, 0, &options);
  argp_parse (&argp, argc, argv, 0, 0, &options);

  if( options.shell ) {
    ferr("Interactive shell not yet supported\n"); // TODO
    return 1;
  } else {
    MagickWand* wand = prepare_image(options);
    if( options.dry != 1) {
      PixelatorState *pixelator_state = pixelator_init(options,wand);

      stream(pixelator_state, options);
    }

    cleanup_image(wand);
  }

  return 0;
}
