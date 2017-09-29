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
	//Start counting number of fields for indexing purposes
	//If it is null, then there are no fields
	if (field == NULL)
	{
		printf("ERROR, no fields");
	}
	

	//Just in case first column is the column to be sorted
	if (strcmp(field, argv[2]) == 0)
	{
		colNum = 0;
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
		printf("The argument %s is not in the file. Ending program");
		exit(0);
	}
	
	printf("The column to be sorted is %s",column);
	
	printf("Organizing records...");
//////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	
	// size of a record + 100 for every char*, which we assume will have a MAX length of 99 (100 for the nullbyte)
	struct Record * allrecords = (Record *)malloc(sizeof(Record) * 10000);
	//size of the records array in bytes
	int arSize = 10000 * (sizeof(Record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
	
	//ptr for specifying struct
	struct Record * ptrrecords = allrecords;
	int numRecords = 0;

	//getting first line to jumpstart the loop
	bytes = getline(&line,&recordsize,stdin)-26;
	

	while (bytes != -1)
	{
		

		if (bytes != -1)
		{
			//increase count of records
			numRecords++;
			//Add to total amount of bytes
			totalbytes += bytes;
			//Check if total bytes goes over
			allrecords = evalArray(allrecords, totalbytes, arSize);
			
			int i;			
			//get tokens in the line
			for(i = 0; i < numFields+1;i++)
			{
				if (i == 0)
					field = strtok(line, ",");
				else{
					field = strtok(NULL, ",");
				}
				//Based on the index, it allocates token to that field in the struct.
				allocateToken(ptrrecords, field, i);
				
			}
		}
	
		//get the line
		//-26 because it would be inaccurate if we were to count the commas when 
		//we are not storing them. 
		bytes = getline(&line, &recordsize, stdin) - 26;
		
		//Move ptr over
		//if the line is greater than the initial size of record, base move off that
		if (bytes > sizeof(Record))
		{
			ptrrecords += bytes;
		}
		//Otherwise, base the increment of the ptr on the record size.
		else
		{
			ptrrecords += sizeof(Record);
		}
	}

	
	
	


	
	return 0;
	
	
}//End main

void allocateToken(Record* ptrrecords, char* field, int index)
{
	int length;
	double grossConv;
	
	switch(index){
		
					
		//0,1,6,9,10,11,14,16,17,19,20,21 are indices w/ strings 
		//they need to be malloced
		case 0:
			length = strlen(field);
			ptrrecords->color = (char*)malloc(sizeof(char) * length);
			ptrrecords->color = field;
			break;
			
		case 1:
			length = strlen(field);
			ptrrecords->director_name = (char*)malloc(sizeof(char) * length);
			ptrrecords->director_name = field;
			break;
			
		case 2:
			ptrrecords->num_critic_for_reviews = atof(field);
			break;
		case 3:
			ptrrecords->duration = atof(field);
			break;
		case 4:
			ptrrecords->director_facebook_likes;
			break;
		case 5:
			ptrrecords->actor_3_facebook_likes;
			break;
			
		case 6: 
			length = strlen(field);
			ptrrecords->actor_2_name = (char*)malloc(sizeof(char) * length);
			ptrrecords->actor_2_name = field;
			break;		
			
		case 7:	
			ptrrecords->actor_1_facebook_likes = atof(field);
			break;
		case 8:
			grossConv = atof(field);
			ptrrecords->gross = grossConv;
			break;
			
			
		case 9: 
			length = strlen(field);
			ptrrecords->genres = (char*)malloc(sizeof(char) * length);
			ptrrecords->genres = field;
			break;
		
		case 10: 
			length = strlen(field);
			ptrrecords->actor_1_name = (char*)malloc(sizeof(char) * length);
			ptrrecords->actor_1_name = field;
			break;
			
		case 11:
			length = strlen(field);
			ptrrecords->movie_title = (char*)malloc(sizeof(char) * length);
			ptrrecords->movie_title = field;
			
			break;
			
		case 12:
			ptrrecords->num_voted_users = atof(field);
			break;
		case 13:
			ptrrecords->cast_total_facebook_likes;
			break;
		case 14: 
			length = strlen(field);
			ptrrecords->actor_3_name = (char*)malloc(sizeof(char) * length);
			ptrrecords->actor_3_name = field;
			break;
		case 15:
			ptrrecords->facenumber_in_poster;
			break;
		case 16:
			length = strlen(field);
			ptrrecords->plot_keywords = (char*)malloc(sizeof(char) * length);
			ptrrecords->plot_keywords = field;
			
			break;

		case 17:
			length = strlen(field);
			ptrrecords->movie_imdb_link = (char*)malloc(sizeof(char) * length);
			ptrrecords->movie_imdb_link = field;
			
			break;
		case 18:
			ptrrecords->num_user_for_reviews = atof(field);
			break;
		case 19:
			length = strlen(field);
			ptrrecords->language = (char*)malloc(sizeof(char) * length);
			ptrrecords->language = field;
			break;


		case 20:
			length = strlen(field);
			ptrrecords->country = (char*)malloc(sizeof(char) * length);
			ptrrecords->country = field;
			break;
			
		case 21:
			length = strlen(field);
			ptrrecords->content_rating = (char*)malloc(sizeof(char) * length);
			ptrrecords->content_rating = field;
			
			break;
		case 22:
			ptrrecords->budget = atof(field);
			break;
		case 23:
			ptrrecords->title_year = atof(field);
			break;
		case 24:
			ptrrecords->actor_2_facebook_likes = atof(field);
			break;
			
		case 25:
			ptrrecords->imdb_score = atof(field);
			break;
		case 26:
			ptrrecords->aspect_ratio = atof(field);
			break;
		case 27:
			ptrrecords->movie_facebook_likes = atof(field);
			break;
		default:
			printf("Did not allocate token correctly, ending");
			exit(0);
			break;
	}
						
						
}
//method takes in the total num of bytes in the line and the size of the record array
//expands the array if there is no space
struct Record* evalArray(struct Record * allrecords, int totalbytes, int arSize)
{
	Record *newall = NULL;
	//if the ptr for records goes outside of allrecords, realloc more memory.
	if(totalbytes > arSize)
	{
		//Add 5000 to the number of input records
		arSize += (5000 * sizeof(Record));
		
		newall = (Record *) realloc(allrecords, arSize);
		if (newall == NULL)
		{
			printf("Out of memory, exiting");
			exit(0);
		}
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
