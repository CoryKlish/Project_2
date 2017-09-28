//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"

//String that holds the modes for the sorter. 
const char* modes = "c";
int numFields = 0; 
char** fields;
//tracks location and name of the column desired
char* column;
int colNum;
//for dynamic allocation of column name
int len;


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

	
//////////////////Parsing first line for column types and testing user input//////////////////////////////////////////////////
	
	/*
		Make assumption that the first row is 200 chars
		but this doesn't matter bc getline method 
		expands the char* array if it needs to
		
		Use size_t (unsigned integer) bc it's part of getline param
	*/
	size_t recordsize;
	char* line = NULL;
	char* field = NULL;
	line = (char*)malloc(recordsize + 1);
	

	//if getline == -1, means it reached EOF and read nothing
	size_t bytes = getline(&line, &recordsize, stdin);
	
	if (bytes == -1)
	{
		printf("\nEOF, ending program");
		exit(0);
	}
	

	//Take first field, puts ptr on first char of line
	field = strtok(line, ",");
	//Start counting number of fields for indexing purposes
	//If it is null, then there are no fields
	if (field == NULL)
	{
		printf("ERROR, no fields");
	}
	

	//Just in case first column is the column to be sorted
	if (strcmp(field, argv[2]) == 0)
	{
		colnum = 0;
		len = strlen(field);
		column = (char*)malloc(sizeof(char) * len);
		column = strcpy(column, field);
	}

	//Count loop
	while (field != NULL)
	{
		printf("\n%s", field);
		
		
		//Subsequent calls to strtok move the pointer and return the actual token
		field = strtok(NULL, ",");
		
		
		//Then i can start counting tokens
		if (field != NULL)
		{
			numFields++;
			
			//Then check if that column is equivalent to the argument passed
			if (strcmp(field, argv[2]) == 0)
			{
				//index is 1 behind the count, 
				colNum = numFields -1;
				
				//dynamic allocate the mem and store string
				len = strlen(field);
				column = (char*)malloc(sizeof(char) * len);
				column = strcpy(column, field);
				
			}
		}
			
	}
	//If the column was never found, then it does not exist within the file.
	if (column == NULL)
	{
		printf("The argument %s is not in the file. Ending program");
		exit(0);
	}
	
	printf("The column to be sorted is %s",column);
	
	printf("Organizing records...");
///////////////////////////////////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	
	// size of a record + 100 for every char*, which we assume will have a MAX length of 99 (100 for the nullbyte)
	struct record* allrecords = (record*)malloc(sizeof(record) * 10000);
	//size of the records array in bytes
	int arSize = 10000 * (sizeof(record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
	
	//ptr for reallocation
	//struct record * ptrrecords = allrecords;

	

	while (bytes != -1)
	{
		
		//get the line
		bytes = getline(&line, &recordsize, stdin);
		int i;
		//get tokens in the line
		for(i = 0; i < numFields;i++)
		{
			
		}
		
		
	}
	
	
	
	/*
		Put each row into an individual struct
		
		
	*/

	
	return 0;
	
	
}//End main

//method takes in the total num of bytes in the line and the size of the record array
//expands the array if there is no space
struct record* evalArray(record* allrecords, int totalbytes, int arSize)
{
	struct record *newall = NULL;
	//if the ptr for records goes outside of allrecords, realloc more memory.
	if(totalbytes > arSize)
	{
		//Add 5000 to the number of input records
		arSize += (5000 * sizeof(record);
		
		newall = (record*) realloc(allrecords, arSize);
		if (newall == NULL)
		{
			printf("Out of memory, exiting");
			exit(0);
		}
	}
}


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

/*
****Saving this piece jsut in case
//if the ptr for records goes outside of allrecords, realloc more memory.
	if(totalbytes > arSize)
	{
		//Add 5000 to the number of input records
		arSize += (5000 * sizeof(record);
		
		//If the backup ptr has not been realloced into yet, realloc allrecords
		if (newall == NULL)
		{
			//realloc to same ptr
			allrecords = realloc(allrecords,arSize);
		}
		
		//otherwise, newall is being used, realloc to newall		
		else
		{
			//otherwise, newall is being used, realloc to newall
			newall = realloc(newall,arSize);
		}

		if (allrecords == NULL)
		{
			newall = realloc(allrecords,arSize);
			//if successful, free the allrecords
			if (newall != NULL)
			{
				free(allrecords);
			}
			else
			{
				printf("Out of memory, exiting program");
				exit(0);
			}
		}
	}



*/
