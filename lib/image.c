#include "image.h"

// grabbed this exception code from:
// https://www.imagemagick.org/script/magick-wand.php
void throw_wand_exception(MagickWand* wand)
{
  char *description;
  ExceptionType severity;

  description=MagickGetException(wand, &severity);
  (void) ferr("%s %s %lu %s\n", GetMagickModule(), description);
  description=(char *) MagickRelinquishMemory(description);
  exit(-1);
}

// resize_image
// inputs:
//    wand -- MagickWand** -- passed by reference
// outputs:
//    void
// side effects:
//    wand image is resized so the longest edge is 512px
//    if the image is smaller than 512x512, nothing happens
void resize_image(MagickWand** wand)
{
  size_t width, height;
  // get the image size
  width  = MagickGetImageWidth(*wand);
  height = MagickGetImageHeight(*wand);
  // only resize if it's bigger than 512x512
  fmsg("size:  \t%dx%d\n", (int)width, (int)height);
  if (width > 512 || height > 512) {
    if (width > height) {
      height = (int)((height/(float) width) * 512);
      width  = 512;
    } else {
      width  = (int)((width/(float) height) * 512);
      height = 512;
    }
    fmsg("resize:\t%dx%d\n", (int)width, (int)height);
    MagickBooleanType status = MagickAdaptiveResizeImage(*wand, width, height);
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
MagickWand* prepare_image(HTPewPewOpts opts)
{
  MagickBooleanType status;
  MagickWand* magick_wand;
  
  // quantum depth should never be bigger than 32 bits (4 bytes) so a unsigned int should always hold it
  // there's some details here: http://www.imagemagick.org/Usage/basics/#quality
  Quantum depth = opts.threshold * (QuantumRange / 100);

  //    Read an image.
  MagickWandGenesis();
  magick_wand = NewMagickWand();
  fnote("Reading Image\n");
  status = MagickReadImage(magick_wand, opts.infile);
  if (status == MagickFalse)
    throw_wand_exception(magick_wand);
  fnote("Resizing Image\n");
  resize_image(&magick_wand);
  fnote("Antialiasing Image\n");
  antialias_image(&magick_wand);
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

int cleanup_image(MagickWand* wand) {
  wand = DestroyMagickWand(wand);
  MagickWandTerminus();
  return(0);
}

