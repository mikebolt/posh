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
      fprintf(stderr, "Usage: dup source dest command\n");
      exit(1);
   }

   sourceFD = atoi(argv[1]);
   destFD = atoi(argv[2]);

   if (dup2(sourceFD, destFD) == -1) {
      perror("dup2");
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
