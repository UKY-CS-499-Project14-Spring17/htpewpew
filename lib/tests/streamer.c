#include "../streamer.h"
#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int stream_test_1();
int send_pixel_command_test_1();
int send_pixel_command_test_2();
int send_pixel_command_test_3();
int get_next_pixel_count_test_1();
int get_next_pixel_count_test_2();
int get_next_pixel_count_test_3();
int get_next_pixel_count_test_4();
int initialize_carver_test_1();
int initialize_carver_test_2();
int carve_image_test_1();
int carve_image_test_2();
int finalize_carving_test_1();
int finalize_carving_test_2();
int init_serial_port_test_1();
int init_serial_port_test_2();
int send_command_test_1();
int send_command_test_2();
int send_command_test_1();
int send_command_test_2();

int stream_test_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    HTPewPewOpts test_opts;
    PixelatorState *null_pixelator = NULL

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    stream(null_pixelator,test_opts);
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
