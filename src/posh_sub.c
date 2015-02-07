#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"


int main(int argc, char **argv) {
   POSHparser parser;
   char **array;
   char c;
   int pid;
   int status;

   if (argc != 1) {
      fprintf(stderr, "Usage: sub\n");
      exit(1);
   }

   parser = POSHmakeParser();

   while ((c = getchar()) != EOF && c != '\n') {
      POSHsubmitCharacter(&parser, c);
   }

   array = POSHlistToStringArray(&parser.list);

   pid = fork();
   if (pid) {
      wait(&status);
   }
   else {
      execvp(array[0], array);
      perror("execvp");
      exit(1);
   }

   POSHfreeParser(parser);

   return 0;
}
