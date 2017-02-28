#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

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

void greyscale_image(MagickWand** wand)
{
  MagickBooleanType status = MagickQuantizeImage(*wand, 255, GRAYColorspace, 0, MagickFalse, MagickTrue);  
  if (status == MagickFalse)
    throw_wand_exception(*wand);
}

void resize_image(MagickWand** wand)
{
  size_t width, height;
  // get the image size
  width = MagickGetImageWidth(*wand);
  height = MagickGetImageHeight(*wand);
  // only resize if it's bigger than 512x512
  fprintf(stdout, "size:   %dx%d\n", (int)width, (int)height);
  if (width > 512 || height > 512) {
    if (width > height) {
      height = (int)((height/(float) width) * 512);
      width  = 512;
    } else {
      width  = (int)((width/(float) height) * 512);
      height = 512;
    }
    fprintf(stdout, "resize: %dx%d\n", (int)width, (int)height);
    MagickBooleanType status = MagickAdaptiveResizeImage(*wand, width, height);
    if (status == MagickFalse)
      throw_wand_exception(*wand);
  }
  // alter compact size
}

void threshold_image(MagickWand** wand)
{
  // 10% ~ 6500.0
  // 40% ~ 27000.0
  // 50% ~ 30500.0
  // 60% ~ 40000.0
  // 90% ~ 58500.0
  // smaller numbers (more white) <---------> (more black) larger numbers
  MagickBooleanType status = MagickThresholdImage(*wand, 30500.0);
  if (status == MagickFalse)
    throw_wand_exception(*wand);
}

int prepare_image(char* image, char* output)
{
  MagickBooleanType status;
  MagickWand* magick_wand;

  //    Read an image.
  MagickWandGenesis();
  magick_wand = NewMagickWand();  
  status = MagickReadImage(magick_wand, image);
  if (status == MagickFalse)
    throw_wand_exception(magick_wand);
  //    Resize image
  resize_image(&magick_wand);
  //    Greyscale image
  greyscale_image(&magick_wand);
  //    Greyscale image
  threshold_image(&magick_wand);
  //    Write the image, then destroy the wand.
  status = MagickWriteImages(magick_wand, output, MagickTrue);
  if (status == MagickFalse)
    throw_wand_exception(magick_wand);
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  return(0);
}

int main(int argc,char **argv)
{
  if (argc != 3)
    {
      (void) fprintf(stdout,"Usage: %s image thumbnail\n",argv[0]);
      exit(0);
    }
  prepare_image(argv[1], argv[2]);
  return(0);
}

