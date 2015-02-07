#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {
   int openedFD, destFD;
   
   if (argc != 4) {
      fprintf(stderr, "Usage: read file fd command\n");
      exit(1);
   }

   openedFD = open(argv[1], O_RDONLY);
   if (openedFD == -1) {
      perror("open");
      exit(1);
   }

   destFD = atoi(argv[2]);

   if (dup2(openedFD, destFD) == -1) {
      perror("dup2");
      exit(1);
   }

   POSHrunCommand(argv[3]);

   if (close(openedFD) == -1) {
      perror("close");
      exit(1);
   }

   return 0;
}
