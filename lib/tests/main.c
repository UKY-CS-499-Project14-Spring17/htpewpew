#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../cli.h"
#include "../image.h"
#include "../pixelator.h"
#include "../options.h"
#include "../streamer.h"
#include <argp.h>

int parse_veb_1();
int parse_veb_2();
int parse_veb_3();
int parse_veb_4();
int parse_opts_1();
int parse_opts_2();
int parse_opts_3();
int parse_opts_4();
int parse_opts_5();
int parse_opts_6();
int parse_opts_7();
int parse_opts_8();
int parse_opts_9();
int parse_opts_10();
int parse_opts_11();
int parse_opts_12();
int parse_opts_13();
int parse_opts_14();
int parse_opts_15();
int parse_opts_16();
int parse_opts_17();
int parse_opts_18();
int parse_opts_19();
int parse_opts_20();


// test #1
// file does not exist
// throw_wand_exception should throw an error
// if the error throws and stderr prints,
// read and confirm the error status
int parse_veb_1() {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      parse_verbose();
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
