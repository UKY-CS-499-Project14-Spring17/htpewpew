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

// test #1
// file does not exist
// throw_wand_exception should throw an error
// if the error throws and stderr prints,
// read and confirm the error status
int throw_wand_exception_test_1() {
  int fdn[2], retval;
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
      retval = 1;
    } else {
      char errmsg[153], *experrmsg = KBLD KRED "Error: image.c throw_wand_exception 11 unable to open image `this is not an image': No such file or directory @ error/blob.c/OpenBlob/2712\n" KNRM;
      read(fdn[0], errmsg, 152);
      errmsg[153] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        retval = 0;
      } else { 
        tfail("message did not match expected.\n");
        tmsg("%s\n",errmsg);
        tmsg("%s\n",experrmsg);
        tmsg("%d ",strlen(errmsg));
        tmsg("%d\n",strlen(experrmsg));
        retval = 1;
      }
    }
  }
  return retval;
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
      return 2;
    }
  }
}

int resize_image_test_1() {
  return 0;
}

int image_tests() {
  int results = 0;
  results = results | throw_wand_exception_test_1();
  results = results | throw_wand_exception_test_2();
  results = results | resize_image_test_1();
  results = results | 0;
  return(results);
}
