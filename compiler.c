#include <stdlib.h>
#include <string.h>
#include "parser.h"

const char *errorList[26] = {
        /* 0. */ "No errors, program is syntactically correct.",
        /* 1. */ "Use = instead of :=.",
        /* 2. */ "= must be followed by a number.",
        /* 3. */ "Identifier must be followed by =.",
        /* 4. */ "const, int, procedure must be followed by identifier.",
        /* 5. */ "Semicolon or comma missing.",
        /* 6. */ "Incorrect symbol after procedure declaration.",
        /* 7. */ "Statement expected.",
        /* 8. */ "Incorrect symbol after statement part in block.",
        /* 9. */ "Period expected.",
        /* 10. */ "Semicolon between statements missing.",
        /* 11. */ "Undeclared identifier.",
        /* 12. */ "Assignment to constant or procedure is not allowed.",
        /* 13. */ "Assignment operator expected.",
        /* 14. */ "call must be followed by an identifier.",
        /* 15. */ "Call of a constant or variable is meaningless.",
        /* 16. */ "then expected.",
        /* 17. */ "Semicolon or end expected.",
        /* 18. */ "do expected.",
        /* 19. */ "Incorrect symbol following statement.",
        /* 20. */ "Relational operator expected.",
        /* 21. */ "Expression must not contain a procedure identifier.",
        /* 22. */ "Right parenthesis missing.",
        /* 23. */ "The preceding factor cannot begin with this symbol.",
        /* 24. */ "An expression cannot begin with this symbol.",
        /* 25. */ "This number is too large.",
};


int main(int argc, char **argv)
{
	int l = 0, v = 0, a = 0;
	int j;
	for (j = 0; j < argc; j++)
	{
		if (strcmp(argv[j], "-l") == 0)
			l = 1;
		if (strcmp(argv[j], "-a") == 0)
			a = 1;
		if (strcmp(argv[j], "-v") == 0)
			v = 1;
	}
	
	int error_found = 0;
	
	initialize_la();
	
	error_found = program();
	if (error_found == 1)
	{
        int z = 0;
        for( z = 0; z < errorI ;z++){
             printf("%s", errorList[errors[errorI]]);
             }   
		return 0;
	}
	printf("No errors, program is syntactically correct\n\n");
	
	initialize_stack();
	
	if (l == 1)
		print_token_table();
	if (a == 1)
		print_list();
	if (v == 1)
		print_stack_list();
		
	print_token_table();
	print_list();
	print_stack_list();
	
	return 0;
}
