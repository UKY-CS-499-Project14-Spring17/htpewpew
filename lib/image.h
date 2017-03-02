#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>
#include "options.h"

#ifndef IMAGE_H
#define IMAGE_H

void throw_wand_exception(MagickWand* wand);

void resize_image(MagickWand** wand);
void greyscale_image(MagickWand** wand);
void threshold_image(MagickWand** wand, int threshold);

int prepare_image(struct HTPewPewOptions opts);

#endif
