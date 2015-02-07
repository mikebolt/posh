#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"

/*
#define POSH_ALLOC_SIZE 64

typedef struct POSHstring {
   char *base;
   size_t length;
   size_t allocated;
} POSHstring;

typedef struct POSHlistNode {
   POSHstring string;
   struct POSHlistNode *next;
} POSHlistNode;

typedef struct POSHlist {
   POSHlistNode *head;
   POSHlistNode *tail;
} POSHlist;

typedef struct POSHparser {
   POSHlist list;
   int parenthesesLevel;
   char last;
   char commentLine;
} POSHparser;
*/

int main(int argc, char **argv) {
   POSHparser parser;
   char **array;
   char **iter;
   char c;
   char exit = NO;
   int pid;
   int status;

   while (!exit) {
      printf("> ");

      parser = POSHmakeParser();

      while ((c = getchar()) != EOF && c != '\n') {
         POSHsubmitCharacter(&parser, c);
      }

      /* If the command line is empty, exit the shell. */
      if (parser.list.head == NULL) {
         exit = YES;
      }
      else {
         array = POSHlistToStringArray(&parser.list);

         pid = fork();
         if (pid) {
            wait(&status);
         }
         else {
            execvp(array[0], array);

            exit = YES; /* Exec failed, so exit the child */
         }
      }

      POSHfreeParser(parser);
   }

   return 0;
}
