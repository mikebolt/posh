#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {
   int i;

   if (argc == 1) {
      fprintf(stderr, "Usage: seq commands\n");
      exit(1);
   }

   for (i = 1; i < argc; ++i) {
      POSHrunCommand(argv[i]);
   }

   return 0;
}
