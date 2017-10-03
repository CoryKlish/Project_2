//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"


 char* sortType; //How we are sorting




void printStructs(Record list[], int numStructs){
	
    int i;
    for (i = 0; i < numStructs; i++){
        printf("%s,%s,%f,%f,%f,%f,%s,%f,%f,%s,%s,%s,%f,%f,%s,%f,%s,%s,%f,%s,%s,%s,%f,%f,%f,%f,%f,%f\n", 
		list[i].color, list[i].director_name, list[i].num_critic_for_reviews,
		list[i].duration, list[i].director_facebook_likes, list[i].actor_3_facebook_likes,
		list[i].actor_2_name, list[i].actor_1_facebook_likes, list[i].gross, list[i].genres,
		list[i].actor_1_name, list[i].movie_title, list[i].num_voted_users, list[i].cast_total_facebook_likes,
		list[i].actor_3_name, list[i].facenumber_in_poster, list[i].plot_keywords,
		list[i].movie_imdb_link, list[i].num_user_for_reviews, list[i].language,
		list[i].country, list[i].content_rating, list[i].budget, list[i].title_year,
		list[i].actor_2_facebook_likes, list[i].imdb_score, list[i].aspect_ratio,
		list[i].movie_facebook_likes);
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//String that holds the modes for the sorter. 
const char* modes = "c";
int numFields = 0; 
//tracks location and name of the column desired
char* column;
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
	
		//Get length, allocate size and copy into 'column' variable
		len = strlen(field);
		sortType = (char*)malloc(sizeof(char) * len);
		sortType = strdup(field);
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
				
				//dynamic allocate the mem and store string
				len = strlen(field);
				sortType = (char*)malloc(sizeof(char) * len);
				sortType = strdup(field);
				
			}
		}
			
	}
	//If the column was never found, then it does not exist within the file.
	if (sortType == NULL)
	{
		printf("The argument %s is not in the file. Ending program", sortType);
		exit(0);
	}
	
	printf("\nThe column to be sorted is %s\n",sortType);
	
	printf("\nOrganizing records...\n");
//////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	//holds initial 4000 records
	struct Record * allrecords = (Record *)malloc(sizeof(Record) * 20000);
	//For reallocation
	struct Record *newall = NULL;
	//size of the records array in bytes
	size_t arSize = 20000 * (sizeof(Record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
	
	//ptr for specifying struct
	struct Record * ptrrecords = allrecords;
	int numRecords = 0;

	//jumpstart the loop
	bytes = getline(&line, &recordsize, stdin);	
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

			
			/*
			//Check if total bytes goes over
			if(totalbytes > arSize)
			{
				//Add 5000 to the number of input records
				arSize += (5000 * sizeof(Record));
				printf("\nexpanded arSize\n");

				//reallocate, move pointer to new memory location with more mem
				newall = realloc(allrecords, arSize);
				ptrrecords = newall;
				
				//If this does not work, there is no more memory left to allocate
				if (newall == NULL)
				{
					printf("Out of memory, exiting");
					exit(0);
				}
			}
			*/
			
			
			//checks for a double quote in the row, which indicates there will be nested commas
			char * check = strstr(row,"\"");
			//If double quotes are present,
			char * qchecker;
			int i;			
			char* comma = ",";
			//get tokens in the line
			for(i = 0; i < numFields+1;i++)
			{	

				//get a field
				field = strsep(&row,",");
				trim(field);
				
				
				//If there is a quote in this line
				if (check != NULL)
				{
					//set qchecker to field to check for quote
					qchecker = check;
					//If there is a quote in the beginning of the field, this is string with " we checked for earlier 
					//then we can replace field with the 'special' var that contains contains the field
					//within the double quotes.
					if (*(field) == '"')
					{
						//create new char array
						char* special = (char*)calloc(strlen(qchecker),sizeof(char));
						// move the ptr to the next char after the initial "
						qchecker++;
					
						//Read everything but the "
						//while (*(qchecker) != '"')
						while (*(qchecker) != '\"')						
						{
							//If the character is a comma, be sure to move the token ptr along with it
							//mainly so that the tokenizer keeps up with the correct field
							//rather than considering the other nested commas to be other fields
							if (*(qchecker) == ',')
							{
								field = strsep(&row,",");
							}
							
							//add to the special str and move ptr
							special = strncat(special,qchecker,1);				
							qchecker++;							

						}

					//get field to get the comma next to the quote out
					field = strsep(&row,",");
					//duplicate special str into field
					field = strdup(special);

					*(special + strlen(special - 1)) = '\0';
				
					}
				}		
				
				//Based on the index, it allocates token to that field in the struct.
				printf("\nGot the token %d\n", i);
				printf("field = %s\n",field);
				allocateToken(ptrrecords, field, i);
		
			}//end token loop
				
			

		}//end if bytes != -1
		
	
		bytes = getline(&line, &recordsize, stdin);
		if (bytes != -1)
			ptrrecords++;

		
	}//end while
	

	
	struct Record * first = allrecords + 500;
	printf("\nmovie_title-%s\n",first->movie_title);
	
	

	int numStructs = numRecords;
	
	if ((strcmp(sortType,"color") == 0)||(strcmp(sortType,"director_name")== 0)||(strcmp(sortType,"actor_name_2")== 0)||(strcmp(sortType,"genres")== 0)||(strcmp(sortType,"actor_1_name")== 0)||(strcmp(sortType,"movie_title")== 0)||(strcmp(sortType,"actor_3_name")== 0)||(strcmp(sortType,"plot_keywords")== 0)||(strcmp(sortType,"movie_imdb_link")== 0)||(strcmp(sortType,"language")== 0)||(strcmp(sortType,"country")== 0)||(strcmp(sortType,"content_rating")== 0))
	  {
	    sortString(allrecords, 0, numStructs - 1);

	  }

          else{
	         sortNum(allrecords, 0, numStructs - 1);
        	}

  
 
    printStructs(allrecords, numStructs);

	
	
	


	
	return 0;
	
	
}//End main

void trim(char* str)
{
	
	//Get rid of leading whitespace
	char* startStr = str;
	while (*startStr != '\0' && isspace(*startStr))
		startStr++;
	//The size of the newstr
	size_t length = strlen(startStr) + 1;
	//removes the leading blanks of the orig string up to startStr
	memmove(str, startStr, length)
	//Get rid of trailing whitespace
	char* endStr = string + length;
	
	while (strcmp(string, endStr) < 0 && isspace(*endStr))
	{
		endStr--;
	}
	
	*endStr = '\0';
	
	
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


