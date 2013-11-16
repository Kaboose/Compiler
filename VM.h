#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int sp;
int bp;
int pc;
int ir;
int stack_size; //Keeps track of the current stack size
int ar_size; //Keeps track of the current amount of activation records (1 less than actual)

int code_size;
int current;

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

int stack[MAX_STACK_HEIGHT];
int ar[MAX_STACK_HEIGHT]; //Stores the location of the beginning of new activation records
int back_pointers[MAX_STACK_HEIGHT]; //Stores the location of each back pointer

int cx;

typedef struct
{
	int op;
	int l;
	int m;
	char* label;
} instruction;

instruction code[500];

typedef enum {
	lit = 1, opr, lod, sto, cal, inc, jmp, jpc, sio, sio2
} op_codes;

enum {
        OPR_RET, OPR_NEG, OPR_ADD, OPR_SUB, OPR_MUL, OPR_DIV, OPR_ODD, OPR_MOD,
        OPR_EQL, OPR_NEQ, OPR_LSS, OPR_LEQ, OPR_GTR, OPR_GEQ
};



//Inputs the op code and returns the string for the label
char* find_label(int n);

//Inputs the opr code and carries out the associated function
void Opr(int n);

//Prints the current stack
void print_stack();

void initialize_stack();
void print_list();
