#ifndef POSH_HEADER
#define POSH_HEADER

#define YES 1
#define NO 0

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
} POSHparser;


POSHstring POSHmakeString();
void POSHappendToString(POSHstring *string, char *toAppend);
void POSHappendCharToString(POSHstring *string, char toAppend);
char *POSHgetStringValue(POSHstring *string);
void POSHfreeString(POSHstring *string);

POSHlist POSHmakeList();
char **POSHlistToStringArray(POSHlist *list);
void POSHfreeList(POSHlist *list);

POSHparser POSHmakeParser();
void POSHfreeParser(POSHparser);
void POSHsubmitCharacter(POSHparser *parser, char c);

void POSHrunCommand(char *command);

#endif
