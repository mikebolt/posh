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
   int i;

   if (argc == 1) {
      fprintf(stderr, "Usage: seq commands\n");
      exit(1);
   }

   for (i = 1; i < argc; ++i) {
      parser = POSHmakeParser();

      ptr = argv[i];
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
   }

   return 0;
}
