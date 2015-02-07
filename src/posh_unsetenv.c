#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {

   if (argc != 3) {
      fprintf(stderr, "Usage: unsetenv variable command\n");
      exit(1);
   }

   if (unsetenv(argv[1]) == -1) {
      perror("unsetenv");
      exit(1);
   }

   POSHrunCommand(argv[2]);

   return 0;
}
