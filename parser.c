#include <stdlib.h>
#include "parser.h"

//tokenValues are the syms, tokenTable stores the identifiers, and numberValues stores the numbers
//Everything is stored in respective indices i.e. tokenValues[3] is an identsym, then tokenTable[3] is the identifier

//Addition and subtraction and everything work. According to the grader, I shouldn't print errors automatically I guess?
//So if you maybe want to make an error function or something so that it only prints when you do the parser
//Constants apparently need to be read correctly, and the while loops and if statements need to be made to work
//The main issue is that I have no idea how to evaluate if an expression is true or not




int program()
{
	errorI = 0;
	i = 0; //The current index in the tokenValues
	level = 0; //Tracks the current level
	curr_symbol = 0;
	cx = 0;
	
	
	//Checks for a nulsyms
	int j, error = 0;;
	for (j = 0; j < tableSize; j++)
	{
		if (tokenValues[j] == nulsym)
		{
			addError(25);
			error = 1;
		}
	}
	
	//Checks for a period at the end
	if (tokenValues[tableSize-1] != periodsym)
	{
		addError(9);
		error = 1;
	}
	
	return block(error);
}

int block(int value)
{
	int error = value;
	int num_variables = 0;
	

	
	//Finds constants in the beginning (not working)******
	if (tokenValues[i] == constsym)
	{
		do
		{
			i++;
			if (tokenValues[i] != identsym)
			{
				addError(4);
				error = 1;
			}
			
			i++;
			if (tokenValues[i] != eqsym)
			{
				if (tokenValues[i] == becomessym)
					addError(1);
				else
					addError(3);
				error = 1;
			}
			i++;
			if (tokenValues[i] != numbersym)
			{
				addError(2);
				error = 1;
			}
			
			i++;
			enter(1, tokenTable[i-3], numberValues[i-1], 0);
		} while (tokenValues[i] == commasym);
		
		if (tokenValues[i] != semicolonsym)
		{
			addError(5);
			error = 1;
		}
		i++;
	}
	
	//Finds ints in the beginning, there currently is storage of these
	if (tokenValues[i] == intsym)
	{
		do
		{
			i++;
			if (tokenValues[i] != identsym)
			{
				addError(4);
				error = 1;
			}
			i++;
			enter(2, tokenTable[i-1], level, num_variables+1);	
			num_variables++;
		} while (tokenValues[i] == commasym);
		
		if (tokenValues[i] != semicolonsym)
		{
			addError(5);
			error = 1;
		}
		i++;
	}
	
	int tempcx;
	//Finds procedures, currently does nothing and you don't need to mess with
	while (tokenValues[i] == procsym)
	{
		i++;
		if (tokenValues[i] != identsym)
		{
			addError(4);
			error = 1;
		}
		enter(3, tokenTable[i], level, cx);
		i++;
		if (tokenValues[i] != semicolonsym)
		{
  			addError(5);
			error = 1;
		}
		i++;
		
		tempcx = cx;
		emit(jmp, 0, 0);
		
		level++;
		error = block(error);
		if (tokenValues[i] != semicolonsym)
		{
			addError(5);
			error = 1;
		}
		i++;
		emit(opr, 0, 0);
	}
	code[tempcx].m = cx;
	emit(inc, 0, num_variables+4); //Use to allocate space in the beggining of the AR
	error = statement(error);
	
	level--;
	return error;
}

int statement(int value)
{
	int error = value;
	
	//Changes the values of ints (Works)
	if (tokenValues[i] == identsym)
	{
		int j = find(tokenTable[i], level);
		if (j == -1)
		{
			addError(11);
			error = 1;
		}
		if (symbolType(j) != 2)
		{
			addError(12);
			error = 1;
		}
		i++;
		if (tokenValues[i] != becomessym)
		{
			addError(13);
			error = 1;
		}
		i++;
		error = expression(error);
		emit(sto, level-symbolLevel(j), symbolAddress(j));
	}
	else if (tokenValues[i] == callsym) //Callsym, currently does nothing
	{
		i++;
		if (tokenValues[i] != identsym)
		{
			addError(14);
			error = 1;
		}
		else
		{
			int j = find(tokenTable[i], level);
			if (j == -1)
			{
				addError(11);
				error = 1;
			}
			if (symbolType(j) != 3)
			{
				addError(12);
				error = 1;
			}
			emit(cal, level-symbolLevel(j), symbolAddress(j));
		}
		i++;
		
	}
	else if (tokenValues[i] == beginsym)
	{
		i++;
		error = statement(error);
		while (tokenValues[i] == semicolonsym)
		{
			i++;
			error = statement(error);
		}
		
		if (tokenValues[i] != endsym)
		{
			addError(19);
			error = 1;
		}
		i++;
	}
	else if (tokenValues[i] == ifsym) //If then statements, does not work!!!*******************
	{
		i++;
		error = condition(error);
		if (tokenValues[i] != thensym)
		   addError(16);
		
			i++;
			int ctemp = cx;
			emit(jpc, 0, 0);
			error = statement(error);
			
			int ctemp2 = cx;
			emit(jmp,0,0);
			
			code[ctemp].m = cx;
			
		    if(tokenValues[i] == elsesym){
               i++;
              error =  statement(error);
               
               }
		    code[ctemp2].m = cx;   
		
		
            return error;
	
	}
	else if (tokenValues[i] == whilesym) //While, DOES NOT WORK****************
	{
		int cx1 = cx;
		i++;
		error = condition(error);
		int cx2 = cx;
		emit(jpc, 0, 0);
		if (tokenValues[i] != dosym)
		{
			addError(18);
			error = 1;
		}
		else
			i++;
		error = statement(error);
		emit(jmp, 0, cx1);
		code[cx2].m = cx;
	}
	
	else if(tokenValues[i] == writesym){
         i++;
         
         error = expression(error);
         
         emit(sio,0,1);
         
         }
	else if(tokenValues[i] == readsym){
         
         i++;
         
         
 
         if(tokenValues[i] == identsym)
		 {
		 	int j = find(tokenTable[i], level);
            if(symbol_table[j].kind != 2)
               addError(11);
            else
            {
            	emit(sio2,0,2);
				emit(sto,symbol_table[j].level,symbol_table[j].addr);
			}
         }
        i++; 
    } 
         
         
	return error;
}
//condition checks if there is relation operator but then did nothing with it
// now it emits the correct op code based on the realtion operator
int condition(int value)
{
	int error = value;
	int relop = nulsym;
	if (tokenValues[i] == oddsym)
	{
		i++;
		relop = tokenValues[i];
		error = expression(error);
	}
	else
	{
		error = expression(error);
		if ((tokenValues[i] != eqsym) &&
			(tokenValues[i] != neqsym) &&
			(tokenValues[i] != lessym) &&
			(tokenValues[i] != leqsym) &&
			(tokenValues[i] != gtrsym) &&
			(tokenValues[i] != geqsym))
		{
			addError(20);
			error = 1;
		}
		relop = tokenValues[i];;
		i++;
		error = expression(error);
        
        switch(relop) {
                case oddsym: // odd
                         emit(opr, 0, OPR_ODD);
                        break;
                case eqsym: // =
                        emit(opr, 0, OPR_EQL);
                        break;
                case neqsym: // <>
                        emit(opr, 0, OPR_NEQ);
                        break;
                case lessym: // <
                        emit(opr, 0, OPR_LSS);
                        break;
                case leqsym: // <=
                        emit(opr, 0, OPR_LEQ);
                        break;
                case gtrsym: // >
                        emit(opr, 0, OPR_GTR);
                        break;
                case geqsym: // >=
                        emit(opr, 0, OPR_GEQ);
                        break;
                default:
                        break;
        }
	}
	return error;
}

int expression(int value)
{
	int error = value;
	int addop;
	if (tokenValues[i] == plussym || tokenValues[i]== minussym)
	{
		addop = tokenValues[i];
		i++;
		error = term(error);
		if (addop == minussym)
			emit(opr, 0, 1); //Instruction
	}
	else
		error = term(error);
	while (tokenValues[i] == plussym || tokenValues[i] == minussym)
	{
		addop = tokenValues[i];
		i++;
		error = term(error);
		if (addop == plussym)
			emit(opr, 0, 2); //instruction
		else
			emit(opr, 0, 3); //Instruction
	}
	return error;
}

int term(int value)
{
	int mulop;
	int error = value;
	error = factor(error);
	while (tokenValues[i] == multsym || tokenValues[i] == slashsym)
	{
		mulop = tokenValues[i];
		i++;
		error = factor(error);
		if (mulop == multsym)
			emit(opr, 0, 4); //Instruction
		else
			emit(opr, 0, 5); //Instruction
	}
	return error;
}

int factor(int value)
{
	int error = value;
	if (tokenValues[i] == identsym)
	{
		emit(lod, level-symbolLevel(find(tokenTable[i], level)), symbol_table[find(tokenTable[i], level)].addr);
		i++;
	}
	else if (tokenValues[i] == numbersym)
	{
		emit(lit, 0, numberValues[i]);
		i++;
	}
	else if (tokenValues[i] == lparentsym)
	{
		i++;
		error = expression(error);
		if (tokenValues[i] != rparentsym)
		{
			addError(22);
			error = 1;
		}
		i++;
	}
	else
	{
		addError(23);
		error = 1;
	}
	return error;
}

void enter(int type, char* name, int params, int addr)
{
	symbol_table[curr_symbol].kind = type;
	strcpy(symbol_table[curr_symbol].name, name);
	
	switch (type)
	{
		case 1:
			symbol_table[curr_symbol].val = params;
			break;
		case 2:
			symbol_table[curr_symbol].level = params;
			symbol_table[curr_symbol].addr = addr+3;
			break;
		case 3:
			symbol_table[curr_symbol].level = params;
			symbol_table[curr_symbol].addr = addr+1;
			break;		
	}
	
	curr_symbol++;
}

void emit(int op, int l, int m)
{
    code[cx].op = op; 	//opcode
    code[cx].l = l;	// lexicographical level
    code[cx].m = m;	// modifier
    cx++;
}

int find(char ident[10], int level)
{
	int j;
	int temp = -1;
	for (j = 0; j < curr_symbol; j++)
	{
		if (strcmp(ident, symbol_table[j].name) == 0)
			return j;
	}
	return -1;
}

int symbolType(int index)
{
	return symbol_table[index].kind;
}

int symbolLevel(int index)
{
	return symbol_table[index].level;
}

int symbolAddress(int index)
{
	return symbol_table[index].addr;
}

void addError(int num)
{
     
     errors[errorI] = num;
     errorI++;
}
