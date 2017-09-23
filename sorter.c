//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"

/*
	Created argv for the mode of the sorter (-c for column) and name of column
	
*/
int main(int argc, const char* argv[]) {
	
	//argc counts the call of sorter as an argument
	//Since we are trying to count arguments given to sorter, argc-1
	printf("The number of arguments given to sorter: %d\n", argc-1);
	
	if(argc - 1 < 2)
	{
		printf("Not enough arguments passed, ending program");
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since *argv[0] is the executable, use *argv[1]
	const char* mode = argv[1];	
	
	printf("The mode of the sorter is %s", 	mode);
	
	
	

	
	return 0;
	
	
	
}//End main