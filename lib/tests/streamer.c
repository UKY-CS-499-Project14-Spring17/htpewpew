#include "../streamer.h"
#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "streamer_test_overrides.h"

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
int wait_test_1();

int streamer_tests(){
  int results = 0;
  results = results | stream_test_1();
  results = results | send_pixel_command_test_1();
  results = results | send_pixel_command_test_2();
  results = results | send_pixel_command_test_3();
  results = results | get_next_pixel_count_test_1();
  results = results | get_next_pixel_count_test_2();
  results = results | get_next_pixel_count_test_3();
  results = results | get_next_pixel_count_test_4();
  results = results | initialize_carver_test_1();
  results = results | initialize_carver_test_2();
  return results;
}

int stream_test_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    HTPewPewOpts test_opts;
    PixelatorState *null_pixelator = NULL;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    stream(null_pixelator,test_opts);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("stream_test_1\n");
    int wstat;
    wait( &wstat );
    char errmsg[73];
    char experrmsg[73] = KBLD KRED "Error: Failed to stream. Invalid pixelator state provided.\n" KNRM;
    read(fdn[0], errmsg, 73);
    close(fdn[0]);
    errmsg[72] = '\0';
    //for( int i=0; i<72; i++){
    //  printf("%d %d %d\n", i, experrmsg[i], errmsg[i]);
    //}
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

int send_pixel_command_test_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    PixelatorState *null_pixelator = NULL;
    uint8_t command  = 0x33;
    uint8_t aux_code = 0x00;
    Pixel pixel = {};

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    send_pixel_command(null_pixelator,command,&pixel,aux_code);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("send_pixel_command_test_1\n");
    int wstat;
    wait( &wstat );
    char errmsg[85];
    char experrmsg[85] = KBLD KRED "Error: Failed to send pixel command. Invalid pixelator state provided.\n" KNRM;
    read(fdn[0], errmsg, 85);
    close(fdn[0]);
    errmsg[84] = '\0';
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

int send_pixel_command_test_2(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    PixelatorState pixelator = {};
    uint8_t command   = 0x33;
    uint8_t aux_code  = 0x00;
    Pixel *null_pixel = NULL;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    send_pixel_command(&pixelator,command,null_pixel,aux_code);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("send_pixel_command_test_2\n");
    int wstat;
    wait( &wstat );
    char errmsg[75];
    char experrmsg[75] = KBLD KRED "Error: Failed to send pixel command. Invalid pixel provided.\n" KNRM;
    read(fdn[0], errmsg, 75);
    close(fdn[0]);
    errmsg[74] = '\0';
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

int send_pixel_command_test_3(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    PixelatorState pixelator = {};
    uint8_t command   = 0x33;
    uint8_t aux_code  = 0x00;
    Pixel   pixel = {};

    pixel.x = 300;
    pixel.y = 200;
    pixelator.carver_handle = STDOUT_FILENO;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
    send_pixel_command(&pixelator,command,&pixel,aux_code);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("send_pixel_command_test_3\n");
    int wstat;
    wait( &wstat );
    char output[7];
    char expout[7] = {0x33,0x03,0x00,0x02,0x00,0x00,0xff};
    read(fdn[0], output, 7);
    close(fdn[0]);
    if( memcmp(output,expout,7) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("output did not match expected.\n");
      for( int i=0; i<7; i++){
        printf("Bit %d: Expected: %d Found: %d\n", i, expout[i], output[i]);
      }
      return 1;
    }
  }
}

int get_next_pixel_count_test_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    PixelatorState *null_pixelator = NULL;
    uint8_t previous_pixel_count = 0x3d;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    get_next_pixel_count(null_pixelator,previous_pixel_count);
    close(fdn[1]);
    exit(0);
  } else {
    close(fdn[1]);
    // parent process
    tmsg("get_next_pixel_count_test_1\n");
    int wstat;
    wait( &wstat );
    char errmsg[87];
    char experrmsg[87] = KBLD KRED "Error: Failed to get next pixel count. Invalid pixelator state provided.\n" KNRM;
    read(fdn[0], errmsg, 87);
    close(fdn[0]);
    errmsg[86] = '\0';
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

int get_next_pixel_count_test_2(){
  PixelatorState pixelator = {};
  uint8_t previous_pixel_count = 0x3d;

  // parent process
  tmsg("get_next_pixel_count_test_2\n");
  uint8_t expected_count = 0x3e;

  uint8_t actual_count;
  actual_count = get_next_pixel_count(&pixelator,previous_pixel_count);

  if( expected_count == actual_count ) {
    tpass("\n");
    return 0;
  } else { 
    tfail("output did not match expected.\n");
    ttext("Expected: %d Found: %d\n", expected_count, actual_count);
    return 1;
  }
}

int get_next_pixel_count_test_3(){
  int fdn[2];
  pipe(fdn);
  if(fork() == 0){
    close(fdn[0]);
    char response[10] = "123456789";
    usleep(10000);
    write( fdn[1], response, 10);
    close(fdn[1]);
    exit(0);
  }else{
    close(fdn[1]);

    PixelatorState pixelator = {};
    uint8_t previous_pixel_count = 0x78;
    uint8_t buffer[256];

    pixelator.carver_handle = fdn[0];
    pixelator.readbuffer    = buffer;

    // parent process
    tmsg("get_next_pixel_count_test_3\n");
    uint8_t expected_count = 0x3d;

    uint8_t actual_count;
    actual_count = get_next_pixel_count(&pixelator,previous_pixel_count);
    
    char errmsg[256];
    read(fdn[0], errmsg, 256);

    close(fdn[0]);

    if( expected_count == actual_count ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("output did not match expected.\n");
      ttext("Expected: %d Found: %d\n", expected_count, actual_count);
      return 1;
    }
  }
}

int get_next_pixel_count_test_4(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    PixelatorState pixelator = {};
    uint8_t previous_pixel_count = 0x01;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    get_next_pixel_count(&pixelator,previous_pixel_count);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_next_pixel_count_test_4\n");
    int wstat;
    wait( &wstat );
    char errmsg[86];
    char experrmsg[86] = KBLD KRED "Error: Pixel count out of bounds with value 0x01. Something went wrong.\n" KNRM;
    read(fdn[0], errmsg, 86);
    close(fdn[0]);
    errmsg[85] = '\0';
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

int initialize_carver_test_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {
    close(fdn[0]);
    HTPewPewOpts test_opts;
    PixelatorState *null_pixelator = NULL;

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    initialize_carver(null_pixelator, test_opts);
    close(fdn[1]);
    exit(0);
  } else {
    close(fdn[1]);
    // parent process
    tmsg("initialize_carver_test_1\n");
    int wstat;
    wait( &wstat );
    char errmsg[84];
    char experrmsg[84] = KBLD KRED "Error: Failed to initialize carver. Invalid pixelator state provided.\n" KNRM;
    read(fdn[0], errmsg, 84);
    errmsg[83] = '\0';

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

int initialize_carver_test_2(){
  int fdn[2];
  if(socketpair(AF_LOCAL, SOCK_STREAM, 0, fdn) < 0)
    perror("Unable to create socket pair.\n");

  if (fork() == 0) {
    HTPewPewOpts test_opts;
    PixelatorState pixelator;
    
    test_opts.burn = 50;
    test_opts.intensity = 5;

    uint8_t buffer[256];
    pixelator.carver_handle = fdn[0];
    pixelator.readbuffer    = buffer;
    
    initialize_carver(&pixelator, test_opts);

    exit(0);
  } else {
    PixelatorState *pixelator = NULL;
    get_top_left_pixel(pixelator);
    bla(pixelator);
    // parent process
    tmsg("initialize_carver_test_2\n");
    int finished = 0;
    int failed   = 0;

    uint8_t command_buffer[7];
    char response[10] = "123456789";

    int wstat;
    wait( &wstat );

    uint8_t valid_commands[8][7] = {
      {0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
      {0x33, 0x05, 0x00, 0x00, 0x00, 0x00, 0xff},
      {0x17, 0x32, 0x00, 0x00, 0x00, 0x00, 0xff},
      {0x1b, 0x01, 0x00, 0x01, 0x20, 0x00, 0xff},
      {0x1b, 0x01, 0x01, 0x01, 0x21, 0x01, 0xff},
      {0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
      {0x18, 0x01, 0x00, 0x01, 0x20, 0x00, 0xff},
      {0x15, 0x01, 0x01, 0x00, 0x00, 0x00, 0xff}
    };
    int command_index = 0;

    while( !finished ){
      if(read(fdn[1], command_buffer, 7)<0)
        perror("Test failed to read: ");

      if(command_buffer[0] == HANDSHAKE_CMD         ||
         command_buffer[0] == MAX_PIXEL_COUNTER_BW  ||
         command_buffer[0] == HALF_PIXEL_COUNTER_BW){
        if(write( fdn[1], response, 10)<0) perror("Test failed to write");
      }else if(command_buffer[0] == INIT_CMD){
        finished = 1;
        if(write( fdn[1], response, 10)<0) perror("Test failed to write");
      }
      for( int i=0; i<7; i++){
        if(command_buffer[i] != valid_commands[command_index][i]){
          failed = 1;
          tmsg("Expected: %#02x %#02x %#02x %#02x %#02x %#02x %#02x\nActual:   %#02x %#02x %#02x %#02x %#02x %#02x %#02x\n",
              valid_commands[command_index][0], valid_commands[command_index][1], valid_commands[command_index][2], valid_commands[command_index][3],
              valid_commands[command_index][4], valid_commands[command_index][5], valid_commands[command_index][6], command_buffer[0], command_buffer[1],
              command_buffer[2], command_buffer[3], command_buffer[4], command_buffer[5], command_buffer[6]);
          break;
        }
      }
      command_index++;
    }

    if( !failed ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("output did not match expected.\n");
      return 1;
    }
  }
}
