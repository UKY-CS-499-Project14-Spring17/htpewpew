#include "helpers.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../cli.h"

int cli_1();
int cli_2();
int cli_3();
int cli_4();
int cli_5();
int cli_6();
int cli_7();

// test #1
// Calling set_verbose and then set_silent should produce an error.
int cli_1(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    set_verbose();
    set_silent();


    exit(2);

  } else {
    // parent process
    tmsg("cli_1 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("set_silent() did not check verbose option\n");
      return 1;
    } else {
      char errmsg[55], *experrmsg = KBLD KRED "Error: Cannot be both silent and verbose\n" KNRM;
      read(fdn[0], errmsg, 55);
      errmsg[54] = '\0';
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
// Calling set_silent and then set_verbose should produce an error.
int cli_2(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    set_silent();
    set_verbose();


    exit(2);

  } else {
    // parent process
    tmsg("cli_2 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 1) {
      tfail("set_verbose() did not check silent option\n");
      return 1;
    } else {
      char errmsg[55], *experrmsg = KBLD KRED "Error: Cannot be both silent and verbose\n" KNRM;
      read(fdn[0], errmsg, 55);
      errmsg[54] = '\0';
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


// test #3
// Calling set_silent and then fmsg should produce no output.
int cli_3(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
    set_silent();
    fmsg("If you see this something went wrong\n");
    printf("No Problems Found\n");


    exit(2);

  } else {
    // parent process
    tmsg("cli_3 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 2) {
      tfail("set_silent or fmsg exited unexpectedly\n");
      return 1;
    } else {
      char errmsg[153] = "";
      char* experrmsg = "No Problems Found\n";
      read(fdn[0], errmsg, 152);
      errmsg[152] = '\0';
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
// Calling set_silent and then fnote should produce no output.
int cli_4(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
    set_silent();
    fnote("If you see this something went wrong\n");
    printf("No Problems Found\n");


    exit(2);

  } else {
    // parent process
    tmsg("cli_4 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 2) {
      tfail("set_silent or fnote exited unexpectedly\n");
      return 1;
    } else {
      char errmsg[153] = "";
      char* experrmsg = "No Problems Found\n";
      read(fdn[0], errmsg, 152);
      errmsg[152] = '\0';
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
// Calling set_verbose and then fnote should produce output.
int cli_5(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDOUT_FILENO);
    set_verbose();
    fnote("If this prints then the test worked.\n");


    exit(2);

  } else {
    // parent process
    tmsg("cli_5 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 2) {
      tfail("set_verbose or fnote exited unexpectedly\n");
      return 1;
    } else {
      char errmsg[153] = "";
      char* experrmsg = KBLD KCYN "Note: If this prints then the test worked.\n" KNRM;
      read(fdn[0], errmsg, 152);
      errmsg[152] = '\0';
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


// test #6
// Calling fwarn should produce output on stderr.
int cli_6(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    fwarn("If this prints then the test worked.\n");


    exit(2);

  } else {
    // parent process
    tmsg("cli_6 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 2) {
      tfail("fwarn exited unexpectedly\n");
      return 1;
    } else {
      char errmsg[153] = "";
      char* experrmsg = KBLD KYEL "Warning: If this prints then the test worked.\n" KNRM;
      read(fdn[0], errmsg, 152);
      errmsg[152] = '\0';
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


// test #7
// Calling ferr should produce output on stderr.
int cli_7(){
  int fdn[2];
  pipe(fdn);
  if (fork() == 0) {

    //Need to get the KNRM out of the buffer from the previous test.
    fflush(stdout);

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    ferr("If this prints then the test worked.\n");


    exit(2);

  } else {
    // parent process
    tmsg("cli_7 ");
    int wstat;
    wait( &wstat );
    if( WEXITSTATUS(wstat) != 2) {
      tfail("ferr exited unexpectedly\n");
      return 1;
    } else {
      char errmsg[153] = "";
      char* experrmsg = KBLD KRED "Error: If this prints then the test worked.\n" KNRM;
      read(fdn[0], errmsg, 152);
      errmsg[152] = '\0';
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


// This function or's all of the tests' results together to see which ones pass.
int cli_tests(){


   int results = 0;
   results = results | cli_1();
   results = results | cli_2();
   results = results | cli_3();
   results = results | cli_4();
   results = results | cli_5();
   results = results | cli_6();
   results = results | cli_7();

   return results;

}
