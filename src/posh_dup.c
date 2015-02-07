#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {
   int sourceFD, destFD;

   if (argc != 4) {
      fprintf(stderr, "Usage: dup source dest command\n");
      exit(1);
   }

   sourceFD = atoi(argv[1]);
   destFD = atoi(argv[2]);

   if (dup2(sourceFD, destFD) == -1) {
      perror("dup2");
      exit(1);
   }

   POSHrunCommand(argv[3]);

   return 0;
}
