/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

main.c handles all of the parsing and interpretation of the user input and calls all of 
the necessary functions from the other C files to complete the engraving based on the 
user input.
*/

#include "main.h"

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
      a->burn = atoi(arg);
      //Make sure the intensity is in the acceptable range.
      if( a->burn < 1 || a->burn > 250) {
        ferr("Burn must be between 0-250\n");
    	  argp_usage(state);
      } else {
        fnote("Burn time set to %i\n",a->burn);
      }
      break;
    //dry-run option. If this is input we need to do all of the steps except for sending the commands 
    //to the engraver. 
    case 'd':
      a->dry = 1;
      fnote("Dry run mode enabled\n");
      break;
    //intensity option. If this is input then we need to change the laser intensity setting to what 
    //the user indicated. The intensity only has arange from 1-10 so report an error if it is out of 
    //range.
    case 'i':
      a->intensity = atoi(arg);
      //Check to make sure the input intensity is in the possible range.
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
      //Check to see if the port looks like a path.
      if( a->port[0] != '/') {
        fwarn("%s does not look like a path to a port (i.e. /dev/ttyUSB0)\n", a->port);
      }
      break;
    //threshold option. If this is input we need to use the threshold value indicated by the user 
    //for converting to black and white instead of the default setting of 50.
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
      if(arg[0] == 'n'){
        arg[0] = '-';
      }
      a->x = atoi(arg); // TODO validate // TODO negative numbers
      fnote("Setting x-offset to %d\n", a->x);
      break;
    //y-offset option. If this is input then the image needs to be shifted in the y direction by 
    //pixel amount indicated by the user. This is used by the streamer if it is set. TODO verify
    case 'y':
      if(arg[0] == 'n'){
        arg[0] = '-';
      }
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
  options.x         = 0; // TODO
  options.y         = 0; // TODO
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
