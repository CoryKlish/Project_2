//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"

//String that holds the modes for the sorter. 
const char* modes = "c"; 
/*
	Created argv for the mode of the sorter (-c for column) and name of column
	
*/
int main(int argc, const char* argv[]) {
	
	//argc counts the call of sorter as an argument
	//Since we are trying to count arguments given to sorter, argc-1
	printf("The number of arguments given to sorter: %d\n", argc-1);
	
	//For now, only accepting two arguments in the sorter
	if(argc - 1 < 2 || argc - 1 > 2)
	{
		printf("Not enough arguments passed, ending program");
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since argv[0] is the executable, use argv[1]
	const char* inputmode = argv[1]	;	
	
	int len = strlen (inputmode);
	if (len > 2 || *(inputmode) != '-')
	{
		printf("First argument is not recognized, ending program");
		exit(0);
	}
	
	
	//Verify mode against array of modes
	// *inputmode+1 to get the character instead of hyphen
	char mode = VerifyMode(*(inputmode + 1));
	if(mode == 'x')
	{
		exit(0);
	}

	/*
	//This causes seg fault
	char* fields;
	fgets(fields, 500, stdin);
	printf("\n%c%c%c", *(fields),*(fields+1),*(fields+2));
	*/
	return 0;
	
	
}//End main


char VerifyMode(char mode)
{
	//If the mode == 'c' (first element in char* modes)
	if (*(modes) == mode)
	{
		printf("\nThe sorter will organize by column");
		return 'c';
	}
	//If a mode is entered that does not exist, return invalid char
	else
	{
		printf("The sorter does not recognize the first argument -%c",mode);
		return 'x';
	}
	
}//End VerifyMode function
