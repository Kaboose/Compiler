#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IDENT_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5

typedef enum { 
nulsym=1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
whilesym, dosym, callsym, constsym, intsym, procsym, writesym,
readsym , elsesym } token_type;

char symbolList[500][20];
char tokenTable[500][20];
int tokenValues[500];
int numberValues[500];
char sourceCode[1000];

FILE *fr;

int next;
int currToken;
int size;
int tableSize;

int DFS();
int lexAnalyzer(char *token);

void initalize_la();
void print_token_table();
void make_symbol_list();
