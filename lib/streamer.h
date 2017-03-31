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

#define COMMAND_SIZE         7
#define SET_BORDER_CMD       0x1b
#define DRAW_BOX_CMD         0x1c
// TODO is this goto?
#define GOTO_CMD             0x18
#define INIT_CMD             0x15
#define MAX_PIXEL_COUNTER_BW 0x78
#define MIN_PIXEL_COUNTER_BW 0x3d
#define MAX_LOWER_BYTE       0xff

void    stream                 ( PixelatorState *pixelator                                                  );
void    send_pixel_command     ( PixelatorState *pixelator, uint8_t command, Pixel *pixel, uint8_t aux_code );
uint8_t get_next_pixel_count   ( uint8_t previous_pixel_count                                               );
Pixel   *initialize_carver     ( PixelatorState *pixelator                                                  );
uint8_t carve_image            ( PixelatorState *pixelator, Pixel *first_pixel                              );
void    finalize_carving       ( PixelatorState *pixelator, uint8_t final_counter_value                     );
int     initialize_serial_port (                                                                            );
void    send_command           ( PixelatorState *pixelator, uint8_t *command_buffer                         );

#endif
