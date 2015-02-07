#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {
   POSHparser parser;
   char **array;
   char *ptr;
   char c;
   int pid;
   int status;
   int sourceFD, destFD;

   if (argc != 4) {
      fprintf(stderr, "Usage: setenv variable value command\n");
      exit(1);
   }

   if (setenv(argv[1], argv[2], YES) == -1) {
      perror("setenv");
      exit(1);
   }

   parser = POSHmakeParser();

   ptr = argv[3];
   while (c = *ptr++) {
      POSHsubmitCharacter(&parser, c);
   }

   array = POSHlistToStringArray(&parser.list);

   if (*array) {
      pid = fork();
      if (pid) {
         wait(&status);
      }
      else {
         execvp(array[0], array);
         perror("execvp");
         exit(1);
      }
   }

   free(array);
   POSHfreeParser(parser);

   return 0;
}
