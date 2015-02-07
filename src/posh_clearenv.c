#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"

void clearEnv(char **envp);

int main(int argc, char **argv, char **envp) {
   POSHparser parser;
   char **array;
   char *ptr;
   char c;
   int pid;
   int status;

   if (argc != 2) {
      fprintf(stderr, "Usage: clearenv command\n");
      exit(1);
   }

   clearEnv(envp);

   parser = POSHmakeParser();

   ptr = argv[1];
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
         perror("execve");
         exit(1);
      }
   }

   free(array);
   POSHfreeParser(parser);

   return 0;
}

void clearEnv(char **envp) {
   char **array;
   char *ptr;
   POSHlist list;
   POSHstring name;
   POSHlistNode *node;

   list = POSHmakeList();

   while (*envp) {
      ptr = *envp;

      name = POSHmakeString();
      while (*ptr && *ptr != '=') {
         POSHappendCharToString(&name, *ptr);
         ++ptr; 
      }

      node = (POSHlistNode *) malloc(sizeof(POSHlistNode)); 

      if (!node) {
         perror("malloc");
         exit(1);
      }

      node->string = name;

      if (!list.tail) {
         list.tail = node;
      }

      node->next = list.head;
      list.head = node;

      ++envp;
   }

   node = list.head;
   while (node) {
      if (unsetenv(POSHgetStringValue(&node->string)) == -1) {
         perror("unsetenv");
      } 
      node = node->next;
   }

   POSHfreeList(&list);
}
