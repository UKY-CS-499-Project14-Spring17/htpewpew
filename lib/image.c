#include "image.h"
#include "options.h"

// grabbed this exception code from:
// https://www.imagemagick.org/script/magick-wand.php
void throw_wand_exception(MagickWand* wand)
{
  char *description;
  ExceptionType severity;

  description=MagickGetException(wand, &severity);
  (void) fprintf(stderr,"%s %s %lu %s\n", GetMagickModule(), description);
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
  fprintf(stdout, "size:  \t%dx%d\n", (int)width, (int)height);
  if (width > 512 || height > 512) {
    if (width > height) {
      height = (int)((height/(float) width) * 512);
      width  = 512;
    } else {
      width  = (int)((width/(float) height) * 512);
      height = 512;
    }
    fprintf(stdout, "resize:\t%dx%d\n", (int)width, (int)height);
    MagickBooleanType status = MagickAdaptiveResizeImage(*wand, width, height);
    if (status == MagickFalse)
      throw_wand_exception(*wand);
  }
  // alter compact size
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
void threshold_image(MagickWand** wand, int threshold)
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
int prepare_image(struct HTPewPewOptions opts)
{
  MagickBooleanType status;
  MagickWand* magick_wand;
  
  // quantum depth should never be bigger than 32 bits (4 bytes) so a unsigned int should always hold it
  // there's some details here: http://www.imagemagick.org/Usage/basics/#quality
  unsigned int depth = opts.threshold * (QuantumRange / 100);

  //    Read an image.
  MagickWandGenesis();
  magick_wand = NewMagickWand();  
  status = MagickReadImage(magick_wand, opts.filename);
  if (status == MagickFalse)
    throw_wand_exception(magick_wand);
  resize_image(&magick_wand);
  if (opts.threshold == -1) // skip threshold
    greyscale_image(&magick_wand);
  else // don't bother greyscaling first
    threshold_image(&magick_wand, depth);
  //    Write the image, then destroy the wand.
  if( opts.output[0] != '\0' ) { // if there is an output file
    status = MagickWriteImages(magick_wand, opts.output, MagickTrue);
    if (status == MagickFalse)
      throw_wand_exception(magick_wand);
  }
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  return(0);
}

