#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "posh.h"

/*
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

POSHlistNode POSHmakeListNode() {
   POSHlistNode node;

   node.string = POSHmakeString();
   node.next = NULL;

   return node;
}

POSHlist POSHmakeList() {
   POSHlist list;

   list.head = NULL;
   list.tail = NULL;

   return list;
}

char **POSHlistToStringArray(POSHlist *list) {
   size_t arraySize = sizeof(char *);
   POSHlistNode *current = list->head;
   char **array;
   int index;

   while (current) {
      arraySize += sizeof(char *);
      current = current->next;
   }

   array = (char **) malloc(arraySize);

   current = list->head;
   index = 0;

   while (current) {
      array[index] = POSHgetStringValue(&current->string);
      ++index;
      current = current->next;
   }

   array[index] = NULL;

   return array;
}

/* Assumes that all nodes were dynamically allocated */
/* Does not free the list itself */
void POSHfreeList(POSHlist *list) {
   POSHlistNode *temp;
   while (list->head) {
      temp = list->head;
      list->head = list->head->next;
      POSHfreeString(&temp->string);
      free(temp);
   }
   list->tail = NULL;
}

POSHparser POSHmakeParser() {
   POSHparser parser;

   parser.list = POSHmakeList();
   parser.parenthesesLevel = 0;
   parser.last = 0;

   return parser;
}

void POSHfreeParser(POSHparser parser) {
   POSHfreeList(&parser.list);
}

/* TODO: This parser has some strange behavior. I'm meaning to rewrite it as
 * a recursive descent parser using a line buffer. In the meantime this
 * mostly works. */
void POSHsubmitCharacter(POSHparser *parser, char c) {
   POSHlistNode *newNode;

   if (c == '(') {
      if (parser->last == ' ' && parser->parenthesesLevel == 0) {
         /* Start a new string */
         newNode = (POSHlistNode *) malloc(sizeof(POSHlistNode));
         *newNode = POSHmakeListNode();
         parser->list.tail->next = newNode;
         parser->list.tail = newNode;
      }

      if (parser->parenthesesLevel > 0) {
         POSHappendCharToString(&parser->list.tail->string, c);
      }

      ++parser->parenthesesLevel;
   }
   else if (parser->parenthesesLevel > 0 && c == ')') {
      --parser->parenthesesLevel;

      if (parser->parenthesesLevel > 0) {
         POSHappendCharToString(&parser->list.tail->string, c);
      }
   }
   else if (c != ' ' || parser->parenthesesLevel > 0) {
      if (!parser->list.head) {
         /* If the list is not yet initialized, initialize it. */
         newNode = (POSHlistNode *) malloc(sizeof(POSHlistNode));
         *newNode = POSHmakeListNode();
         parser->list.head = parser->list.tail = newNode;
      }

      if (parser->last == ' ' && parser->parenthesesLevel == 0) {
         /* Start a new string */
         newNode = (POSHlistNode *) malloc(sizeof(POSHlistNode));
         *newNode = POSHmakeListNode();
         parser->list.tail->next = newNode;
         parser->list.tail = newNode;
      }

      POSHappendCharToString(&parser->list.tail->string, c);
   }

   parser->last = c;
}


POSHstring POSHmakeString() {
   POSHstring string;

   string.base = (char *) malloc(POSH_ALLOC_SIZE);
   string.length = 0;
   string.allocated = POSH_ALLOC_SIZE;

   return string;
}

void POSHfreeString(POSHstring *string) {
   free(string->base);
}

void POSHappendToString(POSHstring *string, char *toAppend) {
   while (*toAppend) {
      POSHappendCharToString(string, *toAppend);
      ++toAppend;
   }
}

void POSHappendCharToString(POSHstring *string, char toAppend) {
   if (string->length + 2 > string->allocated) {
      string->base =
       (char *) realloc(string->base, string->allocated + POSH_ALLOC_SIZE);
   }

   string->base[string->length++] = toAppend;
}

/* Note: calling POSHappendToString or POSHappendCharToString after calling
 * POSHgetStringValue invalidates the string returned by POSHgetStringValue. */
char *POSHgetStringValue(POSHstring *string) {
   string->base[string->length] = 0;
   return string->base;
}

void POSHrunCommand(char *command) {
   POSHparser parser;
   char **array;
   char c;
   int pid, status;

   parser = POSHmakeParser();

   while (c = *command++) {
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
