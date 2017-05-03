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
  options.fan_speed = 10;



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
