#ifndef OPTIONS_H
#define OPTIONS_H

// This structure is used by main to communicate with parse_opt.
struct htpewpew_opts
{
  /* -d flag, i-shell mode */
  int dry, shell;
  /* infile, -o,  and -p options */
  char *infile, *outfile, *port;
  /* -b, -i, -t, -x, -y options */
  int burn, intensity, threshold, x, y;
};

#endif
