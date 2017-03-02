#ifndef OPTIONS_H
#define OPTIONS_H

#include <linux/limits.h>

typedef enum { false, true } bool;

struct HTPewPewOptions {
  // string of filename and output location
  char filename[PATH_MAX];
  char output[PATH_MAX];
  // string of portname
  char portname[PATH_MAX];
  // silent (supress fmsg) & verbose (include fnote) mode
  bool silent;
  bool verbose;
  // should set a flag if file is image or gcode, two seperate routes
  bool is_image;
  // draw a box and quit
  bool dry_run;
  // should this be thresheld (B&W) or greyscale
  // set to -1 for B&W, set to 0 to 100 for greyscale
  int threshold;
  // burn time and intensity are just ints for now
  int burn_time;
  int intensity;
  // offset values
  // offset is false unless x or y is defined
  bool offset;
  int x_offset;
  int y_offset;
};

#endif
