#ifndef STREAMER_H
#define STREAMER_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "options.h"
#include "pixelator.h"

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyUSB0"

#define COMMAND_SIZE          7
#define HANDSHAKE_CMD         0x1a
#define SET_BORDER_CMD        0x1b
#define DRAW_BOX_CMD          0x1c
#define LASER_INTENSITY_CMD   0x33
#define LASER_DWELL_TIME_CMD  0x17
// TODO is this goto?
#define GOTO_CMD              0x18
#define INIT_CMD              0x15
#define MAX_PIXEL_COUNTER_BW  0x78
#define HALF_PIXEL_COUNTER_BW 0x5a
#define MIN_PIXEL_COUNTER_BW  0x3d
// Radix is just the fancy word for number base
// and it sounds better as a variable name. This
// is the number where the low byte goes to 0x00 
// and the high byte increments
#define COORDINATE_RADIX      0x64

#define READ_BUFFER_SIZE      256

void    stream                   ( PixelatorState *pixelator, HTPewPewOpts options                            );
void    send_pixel_command       ( PixelatorState *pixelator, uint8_t command, Pixel *pixel, uint8_t aux_code );
uint8_t get_next_pixel_count     ( PixelatorState *pixelator, uint8_t previous_pixel_count                    );
Pixel   *initialize_carver       ( PixelatorState *pixelator                                                  );
uint8_t carve_image              ( PixelatorState *pixelator, Pixel *first_pixel                              );
void    finalize_carving         ( PixelatorState *pixelator, uint8_t final_counter_value                     );
int     initialize_serial_port   ( HTPewPewOpts    options                                                    );
void    send_command             ( PixelatorState *pixelator, uint8_t *command_buffer                         );
void    wait_for_carver_response ( PixelatorState *pixelator                                                  );

#endif
