#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {

   if (argc != 4) {
      fprintf(stderr, "Usage: setenv variable value command\n");
      exit(1);
   }

   if (setenv(argv[1], argv[2], YES) == -1) {
      perror("setenv");
      exit(1);
   }

   POSHrunCommand(argv[3]);

   return 0;
}
