#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "VM.h"

struct instruction
{
	int op;
	int l;
	int m;
	char* label;
};

void initialize_stack()
{
	sp = 0;
	bp = 1;
	pc = 0;
	ir = 0;
	stack_size = 0; //Keeps track of the current stack size
	ar_size = 0;

	current = 0;
	stack[1] = 0;
	stack[2] = 0;
	stack[3] = 0;
	back_pointers[0] = 1;
	
	int i;
	for (i = 0; i < cx; i++)
	{
		code[i].label = malloc(sizeof(char*));
		strcpy(code[i].label, find_label(code[i].op));
	}
}

void print_list()
{
	printf("Line\tOP\tL\tM\n");
	
	int i;
	for (i = 0; i < cx; i++)
	{
		printf("%d\t%s\t%d\t%d\n", i, code[i].label, code[i].l, code[i].m);
	}
}

char* find_label(int n)
{
	switch (n)
	{
		case 1:
			return "lit";
		case 2:
			return "opr";
		case 3:
			return "lod";
		case 4:
			return "sto";
		case 5:
			return "cal";
		case 6:
			return "inc";
		case 7:
			return "jmp";
		case 8:
			return "jpc";
		case 9:
			return "sio";
		case 10:
			return "sio";
	}
	
	return NULL;
}

void Opr(int n)
{
	switch (n)
	{
		case 0:
			stack_size = bp-1;
			sp = bp-1;
			pc = stack[sp+4];
			bp = stack[sp+3];
			break;
		case 1:
			stack[sp] = -stack[sp];
			break;
		case 2:
			stack_size--;
			sp--;
			stack[sp] = stack[sp] + stack[sp+1];
			break;
		case 3:
			stack_size--;
			sp--;
			stack[sp] = stack[sp] - stack[sp+1];
			break;
		case 4:
			stack_size--;
			sp--;
			stack[sp] = stack[sp] * stack[sp+1];
			break;
		case 5:
			stack_size--;
			sp--;
			stack[sp] = stack[sp] / stack[sp+1];
			break;
		case 6:
			stack[sp] = stack[sp]%2;
		case 7:
			stack_size--;
			sp--;
			stack[sp] = stack[sp] % stack[sp+1];
			break;
		case 8:
			stack_size--;
			sp--;
			if (stack[sp] == stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
		case 9:
			stack_size--;
			sp--;
			if (stack[sp] != stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
		case 10:
			stack_size--;
			sp--;
			if (stack[sp] < stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
		case 11:
			stack_size--;
			sp--;
			if (stack[sp] <= stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
		case 12:
			stack_size--;
			sp--;
			if (stack[sp] > stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
		case 13:
			stack_size--;
			sp--;
			if (stack[sp] >= stack[sp+1])
				stack[sp] = 1;
			else
				stack[sp] = 0;
			break;
			
	}
}

void print_stack_list()
{
	printf("\t\t\t\t\tpc\tbp\tsp\tstack\n");
	printf("\nInitial values\t\t\t\t%d\t%d\t%d\n", pc, bp, sp);
	
	while (pc != cx && bp != 0) //While there is more code to execute and bp has not be returned to 0
	{
		ir = pc; //load the pc into the input register
		pc++;
		switch (code[ir].op)
		{
			case 1:
				stack_size++;
				sp++;
				stack[sp] = code[ir].m;
				break;
			case 2:
				Opr(code[ir].m);
				break;
			case 3:
				stack_size++;
				sp++;
				//Move down L levels by moving to the current back pointer index minus L
				stack[sp] = stack[back_pointers[ar_size-code[ir].l]+code[ir].m];
				break;
			case 4:
				stack_size--;
				stack[back_pointers[ar_size-code[ir].l]+code[ir].m] = stack[sp];
				sp--;
				break;
			case 5:
				stack_size += 4;
				stack[sp+1] = 0;
				stack[sp+2] = bp-code[ir].l;
				stack[sp+3] = bp;
				stack[sp+4] = pc;
				bp = sp+1;
				pc = code[ir].m;
				
				//Increase the number of activation records and store the location
				ar[ar_size] = sp+1;
				ar_size++;
				
				//Store the new back_pointer location
				back_pointers[ar_size] = bp;
				break;
			case 6:
				sp = sp+code[ir].m;
				stack_size = sp;
				break;
			case 7:
				pc = code[ir].m;
				break;
			case 8:
				if (stack[sp] == 0)
					pc = code[ir].m;
				sp--;
				stack_size--;
				break;
			case 9:
				sp--;
				stack_size--;
				break;
			case 10:
				stack_size++;
				sp++;
				scanf("%d", &stack[sp]);
				break;
		}
		
		printf("%d\t%s\t%d\t%d\t\t%d\t%d\t%d\t", ir, code[ir].label, code[ir].l, code[ir].m, pc, bp, sp);
		print_stack();
		system("pause");
	}
}

void print_stack()
{
	int i, j;
	for (i=1; i <= stack_size; i++)
	{
		for (j=0; j < ar_size; j++) //If the current index in the stack is the beginning of a new activation record, print '| '
		{
			if (i == ar[j])
				printf("| ");
		}
		printf("%d ", stack[i]);
	}
	printf("\n");
}
