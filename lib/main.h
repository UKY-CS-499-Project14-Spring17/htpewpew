#ifndef MAIN_H
#define MAIN_H

#include "cli.h"
#include "image.h"
#include "pixelator.h"
#include "options.h"
#include "streamer.h"
#include <argp.h>

// This file taken and modified from [http://www.crasseux.com/books/ctutorial/argp-example.html]

const char *argp_program_version ="htpewpew 1.0";

const char *argp_program_bug_address = "<TODO@uky.edu>";

// OPTIONS.  Field 1 in ARGP. Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.

static struct argp_option options[] =
{
  {0,           0,   0,           0,                    "ENGRAVER PARAMETERS:"},
  {"burn-time", 'b', "burn",      0,                    "Set maximum burn time (1-250 ms range, default = 60 ms)"},
  {"dry-run",   'd', 0,           0,                    "Edit image, show engraving area, and quit"},
  {"intensity", 'i', "intensity", 0,                    "Set the laser intensity (1-10 range, default = 5)"},
  {"output",    'o', "outfile",   0,                    "Output to OUTFILE instead of to standard output"},
  {"port",      'p', "port",      0,                    "Location of USB serial port"},
  {"threshold", 't', "threshold", OPTION_ARG_OPTIONAL,  "Use a threshold (0-100%) for black and white (default = 50%)"},
  {"bw",        't', "threshold", OPTION_ALIAS,         "Use a threshold (0-100%) for black and white (default = 50%)"},
  {"x-offset",  'x', "x",         0,                    "Automatically offset x location, prefix with 'n' for negative"},
  {"y-offset",  'y', "y",         0,                    "Automatically offset y location  prefix with 'n' for negative"},
  {0,           0,   0,           0,                    "INTERFACE PARAMETERS:"},
  {"verbose",   'v', 0,           0, "Enables additional debugging notes"},
  {"silent",    's', 0,           0, "Supress all standard messages"},
  {0} // this MUST be the last option in the struct
};

static error_t parse_verbose (int key, char *arg, struct argp_state *state);
static error_t parse_opt (int key, char *arg, struct argp_state *state);

// ARGS_DOC. Field 3 in ARGP.
// A description of the non-option command-line arguments that we accept.
static char args_doc[] = "infile";

// Program documentation.
static char doc[] = "htpewpew: a CLI for serial communication to a HTPOW laser engraver\n\
NOTE: If no file is supplied, htpewpew will enter interactive shell mode.";

// The ARGP structure itself.
static struct argp argp    = {options, parse_opt,     args_doc, doc};
static struct argp verbose = {options, parse_verbose, args_doc, doc};

#endif
