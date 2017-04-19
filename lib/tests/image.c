#include "../image.h"
#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int throw_wand_exception_test_1();
int throw_wand_exception_test_2();
int resize_image_test_1();
int resize_image_test_2();
int resize_image_test_3();
int resize_image_test_4();

// test #1
// file does not exist
// throw_wand_exception should throw an error
// if the error throws and stderr prints,
// read and confirm the error status
int throw_wand_exception_test_1() {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    // child process
    MagickWandGenesis();
    MagickWand* magick_wand = NewMagickWand();
    MagickBooleanType status = MagickReadImage(magick_wand, "this is not an image");

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    if (status == MagickFalse)
      throw_wand_exception(magick_wand);
    exit(1);
  } else {
    // parent process
    tmsg("throw_wand_exception_test_1 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("throw_wand_exception did not throw exception\n");
      return 1;
    } else {
      char errmsg[153], *experrmsg = KBLD KRED "Error: image.c throw_wand_exception 32 unable to open image `this is not an image': No such file or directory @ error/blob.c/OpenBlob/2712\n" KNRM;
      read(fdn[0], errmsg, 152);
      errmsg[153] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        return 0;
      } else { 
        tfail("message did not match expected.\n");
        ttext("%d %s\n",strlen(errmsg), errmsg);
        ttext("%d %s\n",strlen(experrmsg), experrmsg);
        return 1;
      }
    }
  }
}

// test #2
// file does exist
// throw_wand_exception should not throw an error
// if the error throws and stderr prints,
// this test has failed
int throw_wand_exception_test_2() {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    // child process
    MagickWandGenesis();
    MagickWand* magick_wand = NewMagickWand();
    MagickBooleanType status = MagickReadImage(magick_wand, "tests/team.jpg");

    // stdout >> pipe >> strcmp
    if (status == MagickFalse)
      throw_wand_exception(magick_wand);
    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    exit(0);
  } else {
    // parent process
    tmsg("throw_wand_exception_test_2 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 0) {
      tpass("\n");
      return 0;
    } else {
        tfail("\n");
      return 1;
    }
  }
}

// test #1
// image is smaller than 489x489
// image should not resize
int resize_image_test_1() {
  int retval = 0;
  tmsg("resize_image_test_1 ");
  // create wand, load image
  MagickWandGenesis();
  MagickWand* magick_wand = NewMagickWand();
  MagickReadImage(magick_wand, "tests/white.jpg");
  // get the original image size
  size_t old_width, old_height, width, height;
  old_width  = MagickGetImageWidth(magick_wand);
  old_height = MagickGetImageHeight(magick_wand);
  resize_image(&magick_wand);
  // get the original image size
  width  = MagickGetImageWidth(magick_wand);
  height = MagickGetImageHeight(magick_wand);
  if( width != old_width ) {
    tfail("WIDTH");
    retval = 1;
  }
  if( height != old_height ) {
    tfail("HEIGHT");
    retval = 1;
  }
  if( retval == 0 )
    tpass("");
  tmsg("\n");
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  return 0;
}

// test #2
// image is wider than 489 pixels
// image should resize
int resize_image_test_2() {
  int retval = 0;
  tmsg("resize_image_test_2 ");
  // create wand, load image
  MagickWandGenesis();
  MagickWand* magick_wand = NewMagickWand();
  MagickReadImage(magick_wand, "tests/wide.jpg");
  // get the original image size
  size_t old_width, old_height, width, height;
  old_width  = MagickGetImageWidth(magick_wand);
  old_height = MagickGetImageHeight(magick_wand);
  resize_image(&magick_wand);
  // get the original image size
  width  = MagickGetImageWidth(magick_wand);
  height = MagickGetImageHeight(magick_wand);
  if( width >= old_width ) {
    tfail("WIDTH");
    retval = 1;
  }
  if( height >= old_height ) {
    tfail("HEIGHT");
    retval = 1;
  }
  if( retval == 0 )
    tpass("");
  tmsg("\n");
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  return 0;
}

// test #3
// image is taller than 489 pixels
// image should resize
int resize_image_test_3() {
  int retval = 0;
  tmsg("resize_image_test_3 ");
  // create wand, load image
  MagickWandGenesis();
  MagickWand* magick_wand = NewMagickWand();
  MagickReadImage(magick_wand, "tests/tall.jpg");
  // get the original image size
  size_t old_width, old_height, width, height;
  old_width  = MagickGetImageWidth(magick_wand);
  old_height = MagickGetImageHeight(magick_wand);
  resize_image(&magick_wand);
  // get the original image size
  width  = MagickGetImageWidth(magick_wand);
  height = MagickGetImageHeight(magick_wand);
  if( width >= old_width ) {
    tfail("WIDTH");
    retval = 1;
  }
  if( height >= old_height ) {
    tfail("HEIGHT");
    retval = 1;
  }
  if( retval == 0 )
    tpass("");
  tmsg("\n");
  magick_wand = DestroyMagickWand(magick_wand);
  MagickWandTerminus();
  return 0;
}

int image_tests() {
  int results = 0;
  results = results | throw_wand_exception_test_1();
  results = results | throw_wand_exception_test_2();
  results = results | resize_image_test_1();
  results = results | resize_image_test_2();
  results = results | resize_image_test_3();
  return(results);
}
