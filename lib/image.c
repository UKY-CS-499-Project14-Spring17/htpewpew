/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

image.c is the C file that handles all of the image editing 
necessary so that an image can be engraved. An image must be 
489 by 489 pixels at the most or the engraver will run into 
hard stops, so shrinking large images is a must. An image 
must also be black and white or greyscale to be engravable, 
so this file contains he functions that convert images to 
either back and white or greyscale.

The imagemagick library is used to complete all of these 
necessary functions.
*/

#include "image.h"

// grabbed this exception code from:
// https://www.imagemagick.org/script/magick-wand.php
//After detecting a bad wand this function is called to output 
//more information about the error to the user and clean up the 
//wand.
void throw_wand_exception(MagickWand* wand)
{
  char *description;
  ExceptionType severity;

  description=MagickGetException(wand, &severity);
  (void) ferr("%s %s %lu %s\n", GetMagickModule(), description);
  description=(char *) MagickRelinquishMemory(description);
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
  exit(-1);
}

// resize_image
// inputs:
//    wand -- MagickWand** -- passed by reference
// outputs:
//    void
// side effects:
//    wand image is resized so the longest edge is 489px
//    if the image is smaller than 489x489, nothing happens
void resize_image(MagickWand** wand)
{
  size_t width, height;
  // get the image size
  width  = MagickGetImageWidth(*wand);
  height = MagickGetImageHeight(*wand);
  //Output the size of the input image.
  fmsg("size:  \t%dx%d\n", (int)width, (int)height);
  // only resize if it's bigger than 489x489
  if (width > CANVAS_SIZE || height > CANVAS_SIZE) {
    if (width > height) {
      height = (int)((height/(float) width) * CANVAS_SIZE);
      width  = CANVAS_SIZE;
    } else {
      width  = (int)((width/(float) height) * CANVAS_SIZE);
      height = CANVAS_SIZE;
    }
    //Output resized dimensions.
    fmsg("resize:\t%dx%d\n", (int)width, (int)height);
    //Use the ImageMagick library to resize the image.
    MagickBooleanType status = MagickAdaptiveResizeImage(*wand, width, height);
    //Make sure the resizing didn't error.
    if (status == MagickFalse)
      throw_wand_exception(*wand);
  }
  // alter compact size
}

// antialias_image
// inputs:
//    wand -- MagickWand** -- passed by reference
// outputs:
//    void
// side effects:
//    wand image is antialiased
//    TODO: is this the best way to do this?
void antialias_image(MagickWand** wand)
{
  MagickBooleanType status = MagickBlurImage(*wand, 0.0, 0.5);
  if (status == MagickFalse)
    throw_wand_exception(*wand);
}

// greyscale_image
// inputs:
//    wand -- MagickWand** -- passed by reference
// outputs:
//    void
// side effects:
//    wand image is changed to greyscale using -quantize
//    TODO: is this the best way to do this?
void greyscale_image(MagickWand** wand)
{
  MagickBooleanType status = MagickQuantizeImage(*wand, 255, GRAYColorspace, 0, MagickFalse, MagickTrue);  
  if (status == MagickFalse)
    throw_wand_exception(*wand);
}

// threshold_image
// inputs:
//    wand      -- MagickWand** -- passed by reference
//    threshold -- int          -- value between 0 and QuantumRange
// outputs:
//    void
// side effects:
//    wand image is changed to threshold
//    TODO: add support for native numbers 0..QuantumRange
//    TODO: add support for percentages 0..100%
//Uses the ImageMagick library to change the image to black and 
//white based on the threshold value. A higher threshold value 
//will turn whiter pixels to black.
void threshold_image(MagickWand** wand, Quantum threshold)
{
  MagickBooleanType status = MagickThresholdImage(*wand, threshold);
  if (status == MagickFalse)
    throw_wand_exception(*wand);
}

// prepare_image
// inputs:
//    image  -- char* -- filepath to image
//    output -- char* -- filepath to store image
// outputs:
//    void
// side effects:
//    wand image created, file changed and output
//    TODO: add options for threshold/greyscale and image dry-run
//This function calls all of the necessary functions in the right 
//order to prepare an image for engraving. It starts by setting up 
//the ImageMagick overhead and then reads in the image. Next it 
//resizes the image then does anitaliasing to thicken the lines. 
//Then it either converts the image to black and white or greyscale 
//depending on the user input.
MagickWand* prepare_image(HTPewPewOpts opts)
{
  MagickBooleanType status;
  MagickWand* magick_wand;
  
  // quantum depth should never be bigger than 32 bits (4 bytes) so a unsigned int should always hold it
  // there's some details here: http://www.imagemagick.org/Usage/basics/#quality
  Quantum depth = opts.threshold * (QuantumRange / 100);

  //    Set up ImageMagick
  MagickWandGenesis();
  // Create a new wand.
  magick_wand = NewMagickWand();
  //Read in the image and check for failure.
  fnote("Reading Image\n");
  status = MagickReadImage(magick_wand, opts.infile);
  if (status == MagickFalse)
    throw_wand_exception(magick_wand);
  //Resize the image.
  fnote("Resizing Image\n");
  resize_image(&magick_wand);
  fnote("Antialiasing Image\n");
  //Add antialiasing to thicken lines.
  antialias_image(&magick_wand);
  //Convert the image to greyscale or black and white depending on user input.
  if (opts.threshold == -1) { // skip threshold
    fnote("Converting to greyscale\n");
    greyscale_image(&magick_wand);
  } else { // don't bother greyscaling first
    fnote("Converting to black and white. Threshold = %i%%\n", opts.threshold);
    threshold_image(&magick_wand, depth);
  }
  fnote("Conversion complete\n");
  // Write the image, then destroy the wand.
  if( opts.outfile != NULL ) { // if there is an output file
    fnote("Writing temporary image to %s\n", opts.outfile);
    status = MagickWriteImages(magick_wand, opts.outfile, MagickTrue);
    if (status == MagickFalse)
      throw_wand_exception(magick_wand);
  }
  return(magick_wand);
}

//Uses the ImageMagick library to clean up the wand and ImageMagick's overhead.
int cleanup_image(MagickWand* wand) {
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
  return(0);
}

