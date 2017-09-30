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
	
	// size of a record + 100 for every char*, which we assume will have a MAX length of 99 (100 for the nullbyte)
	struct Record * allrecords = (Record *)malloc(sizeof(Record) * 10000);
	//For reallocation
	Record *newall = NULL;
	//size of the records array in bytes
	int arSize = 10000 * (sizeof(Record));
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
		

		if (bytes != -1)
		{
			printf("\nGot line %d\n",numRecords+1);
			//increase count of records
			numRecords++;
			//Add to total amount of bytes
			totalbytes += sizeof(Record);
			printf("totalbytes is now %d and array size is %d",totalbytes,arSize);
			//Check if total bytes goes over
			allrecords = evalArray(allrecords,newall,totalbytes, arSize);
			
			int i;			
			//get tokens in the line
			for(i = 0; i < numFields+1;i++)
			{
				if (i == 0)
					field = strtok(line, ",");
				else{
					field = strtok(NULL, ",");
					//must handle fields with double quotes around them here
					//maybe handoff flow to a trim function
					
				}
				//Based on the index, it allocates token to that field in the struct.
				printf("\nGot the token %d\n", i);
				allocateToken(ptrrecords, field, i);
				
			}
		}
	
		//get the line
		//-26 because it would be inaccurate if we were to count the commas when 
		//we are not storing them. 
		bytes = getline(&line, &recordsize, stdin);
		printf("\nThe number of bytes read is %lu \n",bytes);
	

		//move ptr to next record for allocation
		ptrrecords++;
	}

	
	
	


	
	return 0;
	
	
}//End main


//method takes in the total num of bytes in the line and the size of the record array
//expands the array if there is no space
struct Record* evalArray(struct Record * allrecords, struct Record* newall, int totalbytes, int arSize)
{
	
	//if the ptr for records goes outside of allrecords, realloc more memory.
	if(totalbytes > arSize)
	{
		printf("\nuh oh\n");
		//Add 5000 to the number of input records
		arSize += (5000 * sizeof(Record));
		printf("\nexpanded arSize\n");
		
		newall = (Record *) realloc(allrecords, arSize);
		printf("\nReallocated to newall\n");
		if (newall == NULL)
		{
			printf("Out of memory, exiting");
			exit(0);
		}
		printf("\nsending back newall\n");
		return newall;
		
	}
	//otherwise, don't do anything and return the original array
	else
	{
		return allrecords;
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


