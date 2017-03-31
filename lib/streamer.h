#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>

#define COMMAND_SIZE 7
#define SET_BORDER_CMD 0x1b
#define DRAW_BOX_CMD   0x1c
// TODO is this goto?
#define GOTO_CMD       0x18
#define INIT_CMD       0x15
#define MAX_PIXEL_COUNTER_BW 0x78
#define MIN_PIXEL_COUNTER_BW 0x3d
#define MAX_LOWER_BYTE 0xff

void    stream               ( PixelatorState *pixelator                       );
void    send_pixel_command   ( uint8_t command, Pixel *pixel, uint8_t aux_code );
uint8_t get_next_pixel_count ( uint8_t previous_pixel_count                    );
void    initialize_carver    ( PixelatorState *pixelator                       );

#endif
