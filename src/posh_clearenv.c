#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"

void clearEnv(char **envp);

int main(int argc, char **argv, char **envp) {

   if (argc != 2) {
      fprintf(stderr, "Usage: clearenv command\n");
      exit(1);
   }

   clearEnv(envp);

   POSHrunCommand(argv[1]);

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
