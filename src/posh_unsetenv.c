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

   if (argc != 3) {
      fprintf(stderr, "Usage: unsetenv variable command\n");
      exit(1);
   }

   if (unsetenv(argv[1]) == -1) {
      perror("unsetenv");
      exit(1);
   }

   parser = POSHmakeParser();

   ptr = argv[2];
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
