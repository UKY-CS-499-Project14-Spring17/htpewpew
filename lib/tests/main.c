#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../main.h"
#include "../cli.h"
#include "../image.h"
#include "../pixelator.h"
#include "../options.h"
#include "../streamer.h"
#include <argp.h>

int parse_veb_1(HTPewPewOpts options);
int parse_veb_2(HTPewPewOpts options);
int parse_veb_3(HTPewPewOpts options);
int parse_veb_4(HTPewPewOpts options);
int parse_opts_1(HTPewPewOpts options);
int parse_opts_2(HTPewPewOpts options);
int parse_opts_3(HTPewPewOpts options);
int parse_opts_4(HTPewPewOpts options);
int parse_opts_5(HTPewPewOpts options);
int parse_opts_6(HTPewPewOpts options);
int parse_opts_7(HTPewPewOpts options);
int parse_opts_8(HTPewPewOpts options);
int parse_opts_9(HTPewPewOpts options);
int parse_opts_10(HTPewPewOpts options);
int parse_opts_11(HTPewPewOpts options);
int parse_opts_12(HTPewPewOpts options);
int parse_opts_13(HTPewPewOpts options);
int parse_opts_14(HTPewPewOpts options);
int parse_opts_15(HTPewPewOpts options);
int parse_opts_16(HTPewPewOpts options);
int parse_opts_17(HTPewPewOpts options);
int parse_opts_18(HTPewPewOpts options);
int parse_opts_19(HTPewPewOpts options);
int parse_opts_20(HTPewPewOpts options);




// test #1
// Calling the program with just ./htpewpew
// should start the program in interactive 
// shell mode, but that is not supported at 
// this time.
int parse_veb_1(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 1;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      ferr("Interactive shell not yet supported\n");

    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_veb_1 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_verbose failed unexpectedly\n");
      return 1;
    } else {
      char errmsg[200] = "";
      char* experrmsg = KBLD KRED "Error: Interactive shell not yet supported\n" KNRM;
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected.\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);
        return 1;
      }
    }
  }
}


// test #2
// Calling the program with the verbose option should print a note to stdout.
int parse_veb_2(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);

    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_veb_2 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_verbose failed unexpectedly\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM;
      read(fdn[0], errmsg, 199);
      errmsg[40] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected.\n");
        ttext("Printed message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected message: %d %s\n",strlen(experrmsg), experrmsg);
        return 1;
      }
    }
  }
}


// test #3
// Calling the program with the silent option should print a warning to stderr.
int parse_veb_3(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-s");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);

    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_veb_3 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_verbose failed unexpectedly\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KYEL "Warning: Silent mode enabled: stdout silenced\n" KNRM;
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected.\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);
        return 1;
      }
    }
  }
}


// test #4
// Calling the program with the silent and verbose options should print an error to stderr.
int parse_veb_4(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-s");
    sprintf(argv[3], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);

    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_veb_4 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_verbose failed unexpectedly\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KYEL "Warning: Silent mode enabled: stdout silenced\n" KNRM 
                        KBLD KRED "Error: Cannot be both silent and verbose\n" KNRM;
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0 ) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected.\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);
        return 1;
      }
    }
  }
}


// test #5
// Calling the program with the burn time option in range should set the burn time correctly.
int parse_opts_1(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-v");
    sprintf(argv[3], "-b");
    sprintf(argv[4], "100");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);

      printf("Burn time = %d\n", options.burn);

    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_1 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM KBLD KCYN "Note: Burn time set to 100\n" KNRM
			"Burn time = 100\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected, or burn time was not set correctly.\n");
        ttext("Printed message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected message: %d %s\n",strlen(experrmsg), experrmsg);

	ttext("Burn time should have been set to 100\n");
        return 1;
      }
    }
  }
}


// test #6
// Calling the program with the burn time option greater than 250 should print an error.
int parse_opts_2(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-b");
    sprintf(argv[3], "300");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_2 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Burn must be between 1-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #7
// Calling the program with the burn time option less than 1 should print an error.
int parse_opts_3(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-b");
    sprintf(argv[3], "-100");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_3 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Burn must be between 1-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #8
// Calling the program with the dry run option should complete normally.
int parse_opts_4(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-d");
    sprintf(argv[3], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_4 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM KBLD KCYN "Note: Dry run mode enabled\n" KNRM;
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #9
// Calling the program with the intensity option should complete normally and set the intensity.
int parse_opts_5(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-i");
    sprintf(argv[3], "7");
    sprintf(argv[4], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("Intensity = %d\n", options.intensity);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_5 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM KBLD KCYN "Note: Intensity set to 7\n" KNRM
			"Intensity = 7\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #10
// Calling the program with the intensity option greater than 10 should print an error.
int parse_opts_6(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-i");
    sprintf(argv[3], "20");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_6 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Intensity must be between 1-10\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #11
// Calling the program with the intensity option less than 1 should print an error.
int parse_opts_7(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-i");
    sprintf(argv[3], "-5");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_7 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Intensity must be between 1-10\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #12
// Calling the program with the output option should complete normally and sets outfile to the output name.
int parse_opts_8(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-o");
    sprintf(argv[3], "image2.png");
    sprintf(argv[4], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("Outfile = %s\n", options.outfile);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_8 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM KBLD KCYN "Note: Image output set to image2.png\n" KNRM
			"Outfile = image2.png\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #13
// Calling the program with the port option should complete normally and sets port to the port name.
int parse_opts_9(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-p");
    sprintf(argv[3], "/dev/ttyUSB1");
    sprintf(argv[4], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("Port = %s\n", options.port);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_9 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM 
			KBLD KCYN "Note: Serial port communication set to port /dev/ttyUSB1\n" KNRM 
			"Port = /dev/ttyUSB1\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #14
// Calling the program with the threshold option in range 0-100 should complete normally and set threshold to the input value.
int parse_opts_10(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-v");
    sprintf(argv[3], "-t70");
    //sprintf(argv[4], "70");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("Threshold = %d\n", options.threshold);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_10 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM 
			KBLD KCYN "Note: Setting threshold to 70%\n" KNRM 
			"Threshold = 70\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #15
// Calling the program with the threshold option greater than 100 should print an error.
int parse_opts_11(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-t200");
    //sprintf(argv[3], "200");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_11 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Threshold must be between 0-100\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #16
// Calling the program with the threshold option less than 0 should print an error.
int parse_opts_12(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-t-10");
    //sprintf(argv[3], "-10");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_12 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Threshold must be between 0-100\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #17
// Calling the program with the x-offset option in range -250-250 should complete normally and set x to the input value.
int parse_opts_13(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-x");
    sprintf(argv[3], "100");
    sprintf(argv[4], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("X = %d\n", options.x);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_13 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM 
			KBLD KCYN "Note: Setting x-offset to 100\n" KNRM 
			"X = 100\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #18
// Calling the program with the x-offset option greater than -250-250 should print an error.
int parse_opts_14(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-x");
    sprintf(argv[3], "500");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_14 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: X-offset must be between -250-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #19
// Calling the program with the x-offset option less than -250 should print an error.
int parse_opts_15(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-x");
    sprintf(argv[3], "-300");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_15 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: X-offset must be between -250-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #20
// Calling the program with the y-offset option in range -250-250 should complete normally and set y to the input value.
int parse_opts_16(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 5;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-y");
    sprintf(argv[3], "100");
    sprintf(argv[4], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&verbose, argc, argv, 0, 0, &options);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("Y = %d\n", options.y);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_16 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KCYN "Note: Verbose mode enabled\n" KNRM 
			KBLD KCYN "Note: Setting y-offset to 100\n" KNRM 
			"Y = 100\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #21
// Calling the program with the y-offset option greater than 250 should print an error.
int parse_opts_17(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-y");
    sprintf(argv[3], "500");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_17 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Y-offset must be between -250-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #22
// Calling the program with the y-offset option less than -250 should print an error.
int parse_opts_18(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 4;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-y");
    sprintf(argv[3], "-300");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_18 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) == 1) {
      tfail("parse_opt did not exit appropriately.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = KBLD KRED "Error: Y-offset must be between -250-250\n" KNRM "Usage: htpewpew [OPTION...] infile\n"
			"Try `htpewpew --help' or `htpewpew --usage' for more information.\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #23
// Calling parse_opt with the verbose option should complete normally and do nothing.
int parse_opts_19(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-v");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("No Problems Found\n");


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_19 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = "No Problems Found\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// test #24
// Calling parse_opt with the verbose option should complete normally and do nothing.
int parse_opts_20(HTPewPewOpts options) {
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    int argc = 3;
    char** argv=(char**)malloc((argc)*sizeof(char*));
    int i = 0;
    //allocate space for each string
    // here allocate 50 bytes, which is more than enough for the strings
    for(i = 0; i < (argc); i++){
        argv[i] = (char*)malloc(50*sizeof(char));
    }
    //assign them all something
    sprintf(argv[0], "./htpewpew");
    sprintf(argv[1], "image.png");
    sprintf(argv[2], "-s");

    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
      argp_parse (&argp, argc, argv, 0, 0, &options);
      printf("No Problems Found\n");


    //Free each string
    for(i = 0; i < (argc); i++){
        free(argv[i]);
    }
    //finally release the first string
    free(argv);

    exit(1);
  } else {
    // parent process
    tmsg("parse_opts_20 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("parse_opt failed unexpectedly.\n");
      return 1;
    } else {
      char errmsg[200] = ""; 
      char *experrmsg = "No Problems Found\n";
      read(fdn[0], errmsg, 199);
      errmsg[199] = '\0';
      if( strcmp(errmsg,experrmsg) == 0) {
        tpass("\n");
        return 0;
      } else {
        tfail("Message did not match expected\n");
        ttext("Printed error message: %d %s\n",strlen(errmsg), errmsg);
        ttext("Expected error message: %d %s\n",strlen(experrmsg), experrmsg);

        return 1;
      }
    }
  }
}


// This function or's all of the tests' results together to see which ones pass.
int main_tests(){

  HTPewPewOpts options;

  /* Set argument defaults */
  options.burn      = 60; // TODO
  options.dry       = 0;
  options.intensity = 5; // TODO
  options.outfile   = NULL;
  options.port      = NULL;
  options.threshold = -1;
  options.x         = 0; // TODO
  options.y         = 0; // TODO
  options.shell     = 0;


   int results = 0;
   results = results | parse_veb_1(options);
   results = results | parse_veb_2(options);
   results = results | parse_veb_3(options);
   results = results | parse_veb_4(options);
   results = results | parse_opts_1(options);
   results = results | parse_opts_2(options);
   results = results | parse_opts_3(options);
   results = results | parse_opts_4(options);
   results = results | parse_opts_5(options);
   results = results | parse_opts_6(options);
   results = results | parse_opts_7(options);
   results = results | parse_opts_8(options);
   results = results | parse_opts_9(options);
   results = results | parse_opts_10(options);
   results = results | parse_opts_11(options);
   results = results | parse_opts_12(options);
   results = results | parse_opts_13(options);
   results = results | parse_opts_14(options);
   results = results | parse_opts_15(options);
   results = results | parse_opts_16(options);
   results = results | parse_opts_17(options);
   results = results | parse_opts_18(options);
   results = results | parse_opts_19(options);
   results = results | parse_opts_20(options);

   return results;

}
