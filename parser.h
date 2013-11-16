#include <stdlib.h>
#include "LA.h"
#include "VM.h"

typedef struct  
{ 
	int kind; 		// const = 1, int = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value) 
	int level; 		// L level
	int addr; 		// M address
} symbol;

symbol symbol_table[500];

//made errors array and errorIndex 
int errors[500];
int errorI;



int i;
int curr_symbol;
int level;

int program();
int block(int value);
int statement(int value);
int condition(int value);
int expression(int value);
int term(int value);
int factor(int value);

void enter(int type, char* name, int params, int addr);
void emit(int op, int l, int m);
int find(char ident[10], int level);
int symbolType(int index);
int symbolLevel(int index);
int symbolAddress(int index);
void addError(int num);
