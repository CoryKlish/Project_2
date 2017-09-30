//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"

//String that holds the modes for the sorter. 
const char* modes = "c";
int numFields = 0; 
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
		printf("Wrong number of arguments passed.");
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
	
	//Was once dynamically allocated, but getline does it for you
	line = NULL;
	

	//if getline == -1, means it reached EOF and read nothing
	size_t bytes = getline(&line, &recordsize, stdin);
	
	if (bytes == -1)
	{
		printf("\nEOF, ending program");
		exit(0);
	}
	//create new str to hold getline input
	char* header = strdup(line);
	//Free getline allocated
	free(line);
	//Making sure the line is null after freeing it. 
	line = NULL;
	

	//Take first field, puts ptr on first char of line
	field = strtok(header, ",");
	//Start counting number of fields 
	//If it is null, then there are no fields
	if (field == NULL)
	{
		printf("ERROR, no fields");
	}
	
	//Just in case first column is the column to be sorted
	if (strcmp(field, argv[2]) == 0)
	{
		colNum = 0;
		//Get length, allocate size and copy into 'column' variable
		len = strlen(field);
		column = (char*)malloc(sizeof(char) * len);
		column = strcpy(column, field);
	}

	//Count loop
	while (field != NULL)
	{		
		
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
		printf("The argument %s is not in the file. Ending program", column);
		exit(0);
	}
	
	printf("\nThe column to be sorted is %s\n",column);
	
	printf("\nOrganizing records...\n");
//////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	//holds 10000 records
	struct Record * allrecords = (Record *)malloc(sizeof(Record) * 4000);
	//For reallocation
	Record *newall = NULL;
	//size of the records array in bytes
	int arSize = 4000 * (sizeof(Record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
	
	//ptr for specifying struct
	struct Record * ptrrecords = allrecords;
	int numRecords = 0;

	//getting first line to jumpstart the loop
	bytes = getline(&line,&recordsize,stdin);
	
	printf("\nThe size of record is %lu\n",sizeof(Record));
	printf("\nThe number of bytes read is %zu\n",bytes);


	while (bytes != -1)
	{
		//copy to row to free up the line var
		char* row = strdup(line);
		free(line);
		line = NULL;

		if (bytes != -1)
		{
			printf("\nGot line %d\n",numRecords+1);
			//increase count of records
			numRecords++;
			//Add to total amount of bytes
			totalbytes += sizeof(Record);
			printf("totalbytes is now %d and array size is %d",totalbytes,arSize);
			//Check if total bytes goes over
			if(totalbytes > arSize)
			{
				//Add 5000 to the number of input records
				arSize += (5000 * sizeof(Record));
				printf("\nexpanded arSize\n");

				//reallocate, move pointer to new memory location with more mem
				newall = (Record *) realloc(allrecords, arSize);
				ptrrecords = newall;
				
				//If this does not work, there is no more memory left to allocate
				if (newall == NULL)
				{
					printf("Out of memory, exiting");
					exit(0);
				}
				
			}
			
			int i;			
			
			//get tokens in the line
			for(i = 0; i < numFields+1;i++)
			{	
				field = strsep(&row,",");
			
				//Based on the index, it allocates token to that field in the struct.
				printf("\nGot the token %d\n", i);
				printf("field = %s\n",field);
				allocateToken(ptrrecords, field, i);
			}	
			ptrrecords++;
		}
	
		bytes = getline(&line, &recordsize, stdin);

		
	}
	

	
	Record * first = newall;
	printf("Record 1: Genre- %s\nRecord 5043: movie_title- %s\n",first->genres,second->movie_title);

	
	
	


	
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


