#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>
#include "options.h"
#include "cli.h"

#define CANVAS_SIZE 489

void throw_wand_exception(MagickWand* wand);

void resize_image(MagickWand** wand);
void antialias_image(MagickWand** wand);
void greyscale_image(MagickWand** wand);
void threshold_image(MagickWand** wand, Quantum threshold);

MagickWand* prepare_image(HTPewPewOpts opts);
int cleanup_image(MagickWand* wand);
void check_offsets(MagickWand** wand, int *x_offset, int *y_offset);

#endif
