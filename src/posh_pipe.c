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
   int writePipe, readPipe;
   int pipes[2];

   if (argc != 5) {
      fprintf(stderr,
              "Usage: pipe writePipe readPipe writeCommand readCommand\n");
      exit(1);
   }

   writePipe = atoi(argv[1]);
   readPipe = atoi(argv[2]);

   if (pipe(pipes) == -1) {
      perror("pipe");
      exit(1);
   }

   if (dup2(pipes[0], readPipe) == -1) {
      perror("dup2");
      exit(1);
   }

   /* Parse the writing command */

   parser = POSHmakeParser();

   ptr = argv[3];
   while (c = *ptr++) {
      POSHsubmitCharacter(&parser, c);
   }

   array = POSHlistToStringArray(&parser.list);

   /* Start the writing process */
   if (*array) {
      pid = fork();
      if (!pid) {
         if (dup2(pipes[1], writePipe) == -1) {
            perror("dup2");
            exit(1);
         }

         execvp(array[0], array);
         perror("execvp");
         exit(1);
      }
   }

   close(pipes[1]);
   free(array);
   POSHfreeParser(parser);

   /* Parse the reading command */

   parser = POSHmakeParser();

   ptr = argv[4];
   while (c = *ptr++) {
      POSHsubmitCharacter(&parser, c);
   }

   array = POSHlistToStringArray(&parser.list);

   /* Start the reading process */
   if (*array) {
      pid = fork();
      if (!pid) {
         if (dup2(pipes[0], readPipe) == -1) {
            perror("dup2");
            exit(1);
         }

         execvp(array[0], array);
         perror("execvp");
         exit(1);
      }
   }

   close(pipes[0]);
   free(array);
   POSHfreeParser(parser);

   /* Wait for the two child processes */
   wait(&status);
   wait(&status);

   return 0;
}
