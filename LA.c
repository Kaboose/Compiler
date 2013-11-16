#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LA.h"

void initialize_la()
{
	next = 0;
	currToken = 0;
	int i = 0;
	
	fr = fopen("Input.txt", "r");
	
	do
	{
		fscanf(fr, "%c", &sourceCode[i]);
		i++;
	} while (sourceCode[i-1] != '\0');
	
	size = i-1;
	
	fclose(fr);
	
	i = 0;
	token_type retValue = nulsym;
	while (next != size)
	{
		retValue = DFS();
		
		if (retValue == 0)
			continue;
		
		tokenValues[i] = retValue;
		
		i++;
	}
	
	tableSize = i;
	
	make_symbol_list();
}

void print_token_table()
{
	int i = 0;
	
	for (i=0;i<tableSize;i++)
	{
		printf("%d ", tokenValues[i]);
		if (tokenValues[i] == 2)
			printf("%s ", tokenTable[i]);
		if (tokenValues[i] == 3)
		{
			printf("%d ", numberValues[i]);
		}
	}
	printf("\n");
	
	for (i=0;i<tableSize;i++)
	{
		printf("%s ", symbolList[i]);
		if (tokenValues[i] == 2)
			printf("%s ", tokenTable[i]);
		if (tokenValues[i] == 3)
		{
			printf("%d ", numberValues[i]);
		}
	}
	printf("\n\n");
	
}

void make_symbol_list()
{
	int i;
	for (i = 0; i < tableSize; i++)
	{			
		switch (tokenValues[i])
		{
			case nulsym:
				strcpy(symbolList[i], "nulsym");
				break;
			case identsym:
				strcpy(symbolList[i], "identsym");
				break;
			case numbersym:
				strcpy(symbolList[i], "numbersym");
				break;
			case plussym:
				strcpy(symbolList[i], "plussym");
				break;
			case minussym:
				strcpy(symbolList[i], "minussym");
				break;
			case multsym:
				strcpy(symbolList[i], "multsym");
				break;
			case slashsym:
				strcpy(symbolList[i], "slashsym");
				break;
			case oddsym:
				strcpy(symbolList[i], "oddsym");
				break;
			case eqsym:
				strcpy(symbolList[i], "eqsym");
				break;
			case neqsym:
				strcpy(symbolList[i], "neqsym");
				break;
			case lessym:
				strcpy(symbolList[i], "lessym");
				break;
			case leqsym:
				strcpy(symbolList[i], "leqsym");
				break;
			case gtrsym:
				strcpy(symbolList[i], "gtrsym");
				break;
			case geqsym:
				strcpy(symbolList[i], "geqsym");
				break;
			case lparentsym:
				strcpy(symbolList[i], "lparentsym");
				break;
			case rparentsym:
				strcpy(symbolList[i], "rparentsym");
				break;
			case commasym:
				strcpy(symbolList[i], "commasym");
				break;
			case semicolonsym:
				strcpy(symbolList[i], "semicolonsym");
				break;
			case periodsym:
				strcpy(symbolList[i], "periodsym");
				break;
			case becomessym:
				strcpy(symbolList[i], "becomessym");
				break;
			case beginsym:
				strcpy(symbolList[i], "beginsym");
				break;
			case endsym:
				strcpy(symbolList[i], "endsym");
				break;
			case ifsym:
				strcpy(symbolList[i], "ifsym");
				break;
			case thensym:
				strcpy(symbolList[i], "thensym");
				break;
			case whilesym:
				strcpy(symbolList[i], "whilesym");
				break;
			case dosym:
				strcpy(symbolList[i], "dosym");
				break;
			case callsym:
				strcpy(symbolList[i], "callsym");
				break;
			case constsym:
				strcpy(symbolList[i], "constsym");
				break;
			case intsym:
				strcpy(symbolList[i], "intsym");
				break;
			case procsym:
				strcpy(symbolList[i], "procsym");
				break;
			case writesym:
				strcpy(symbolList[i], "writesym");
				break;
			case readsym:
				strcpy(symbolList[i], "readsym");
				break;
			case elsesym:
				strcpy(symbolList[i], "elsesym");
		}
	}
}

int DFS()
{
	int i = 1, size = 1;
	token_type toke_type = nulsym;
	if (sourceCode[next] == ' ' || sourceCode[next] == '\n' || sourceCode[next] == '\t' || sourceCode[next] == '\r' ||
		sourceCode[next] == '\b' || sourceCode[next] == '\r')
	{
		next++;
		return 0;
	}
		
	tokenTable[currToken][0] = sourceCode[next];
	next++;
	
	if ((tokenTable[currToken][0] >= 65 && tokenTable[currToken][0] <=90) || (tokenTable[currToken][0] >= 97 && tokenTable[currToken][0] <= 122))
	{
		while ((sourceCode[next] >= 65 && sourceCode[next] <=90) || 
			(sourceCode[next] >= 97 && sourceCode[next] <= 122) || (sourceCode[next] >= 48 && sourceCode[next] <= 57))
		{
			tokenTable[currToken][i] = sourceCode[next];
			next++;
			size++;
			i++;
		}
		tokenTable[currToken][i] = '\0';
		
		toke_type = lexAnalyzer(tokenTable[currToken]);
		
		currToken++;
		
		if (size > IDENT_MAX_LENGTH)
		{
			toke_type = nulsym;
		}
		
		return toke_type;
	}
	else if (tokenTable[currToken][0] >= 48 && tokenTable[currToken][0] <= 57)
	{
		toke_type = numbersym;
		while ((sourceCode[next] >= 48 && sourceCode[next] <= 57) || 
				(sourceCode[next] >= 65 && sourceCode[next] <=90) || 
				(sourceCode[next] >= 97 && sourceCode[next] <= 122))
		{
			if ((sourceCode[next] >= 65 && sourceCode[next] <=90) || (sourceCode[next] >= 97 && sourceCode[next] <= 122))
			{
				toke_type = nulsym;
			}
			tokenTable[currToken][i] = sourceCode[next];
			next++;
			i++;
			size++;
		}
		if (size > NUMBER_MAX_LENGTH)
		{
			toke_type = nulsym;
		}
		tokenTable[currToken][i] = '\0';
		numberValues[currToken] = 0;
		
		int j;
		for (j = 0; j < i; j++)
		{
			numberValues[currToken] *= 10;
			numberValues[currToken] += (tokenTable[currToken][j] - 48);
		}
		
	}
	else if (tokenTable[currToken][0] == ';')
	{
		toke_type = semicolonsym;
	}
	else if (tokenTable[currToken][0] == ':')
	{
		if (sourceCode[next] == '=')
		{
			toke_type = becomessym;
			tokenTable[currToken][1] = sourceCode[next];
			next++;
			size++;
		}
	}
	else if (tokenTable[currToken][0] == ',')
	{
		toke_type = commasym;
	}
	else if (tokenTable[currToken][0] == '+')
	{
		toke_type = plussym;
	}
	else if (tokenTable[currToken][0] == '-')
	{
		toke_type = minussym;
	}
	else if (tokenTable[currToken][0] == '*')
	{
		toke_type = multsym;
	}
	else if (tokenTable[currToken][0] == '/')
	{
		if (sourceCode[next] == '*')
		{
			next++;
			while (sourceCode[next] != '*' && sourceCode[next+1] != '/')
				next++;
			next += 2;
			return 0;
		}
		toke_type = slashsym;
	}
	else if (tokenTable[currToken][0] == '=')
	{
		toke_type = eqsym;
	}
	else if (tokenTable[currToken][0] == '(')
	{
		toke_type = lparentsym;
	}
	else if (tokenTable[currToken][0] == ')')
	{
		toke_type = rparentsym;
	}
	else if (tokenTable[currToken][0] == '<')
	{
		toke_type = lessym;
		if (sourceCode[next] == '=')
		{
			next++;
			toke_type = leqsym;
			size++;
		}
		else if (sourceCode[next] == '>')
		{
			next++;
			toke_type = neqsym;
			size++;
		}
	}
	else if (tokenTable[currToken][0] == '>')
	{
		toke_type = gtrsym;
		if (sourceCode[next] == '=')
		{
			next++;
			toke_type = geqsym;
			size++;
		}
	}
	else if (tokenTable[currToken][0] == '.')
	{
		toke_type = periodsym;
	}	
	
	currToken++;
	return toke_type;
}

int lexAnalyzer(char *token)
{
	token_type type;
	
	if (strcmp("const", token) == 0)
		type = constsym;
	else if (strcmp("int", token) == 0)
		type = intsym;
	else if (strcmp("procedure", token) == 0)
		type = procsym;
	else if (strcmp("call", token) == 0)
		type = callsym;
	else if (strcmp("begin", token) == 0)
		type = beginsym;
	else if (strcmp("end", token) == 0)
		type = endsym;
	else if (strcmp("if", token) == 0)
		type = ifsym;
	else if (strcmp("then", token) == 0)
		type = thensym;
	else if (strcmp("else", token) == 0)
		type = elsesym;
	else if (strcmp("while", token) == 0)
		type = whilesym;
	else if (strcmp("do", token) == 0)
		type = dosym;
	else if (strcmp("read", token) == 0)
		type = readsym;
	else if (strcmp("write", token) == 0)
		type = writesym;
	else if (strcmp("odd", token) == 0)
		type = oddsym;
	else
		type = identsym;
	
	return type;
}
