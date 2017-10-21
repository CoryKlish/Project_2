//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter.h"
/*
	Created argv for the mode of the sorter (-c for column) and name of column
	
*/
int main(int argc, char* argv[]) {
    //String that holds the modes for the sorter. 
	
    
	if(argc - 1 < 2)
	{
		printf("Wrong number of arguments passed.");
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since argv[0] is the executable, use argv[1]
    char* inputmode = argv[1];
 
	
	//Third argument is optional directory mode
	//Third argument is the -d symbol
    
    char * dir = argv[3];
 
	
	//checking if first argument is legitimate
	int len = strlen(inputmode);
	if (len > 2 || *(inputmode) != '-')
	{
		printf("First argument is not recognized, ending program");
		exit(0);
	}
    
	//if directory option exists, check if legit
    if (dir != NULL)
    {
        len = strlen (dir);
        if (len > 2 || *(dir) != '-')
        {
            printf("Third argument is not recognized, ending program");
            exit(0);
        }
        char dirMode = VerifyMode(*(dir + 1));
        if(dirMode == 'x')
            exit(0);
    }
	
	//Verify mode against array of modes
	// *inputmode+1 to get the character instead of hyphen
	char mode = VerifyMode(*(inputmode + 1));
	if(mode == 'x')
		exit(0);
//////////////////Parsing first line for column types and testing user input//////////////////////////////////////////////////
	
	/*
		Make assumption that the first row is 200 chars
		but this doesn't matter bc getline method 
		expands the char* array if it needs to
		
		Use size_t (unsigned integer) bc it's part of getline param
	*/
    int numFields = 0;
    int* numP = &numFields; 
	size_t recordsize;
	char* line = NULL;

	
	size_t bytes = getline(&line, &recordsize, stdin);
	if (bytes == -1)
	{
		printf("\nEOF, ending program");
		exit(0);
	}
	
	//create new str to hold getline input
    len = strlen(line);
    char* header = malloc(sizeof(char) * len);
    header = strdup(line);

    
    char* inputCol = argv[2];
    //evaluates the header, assume size of 40
    char* sortType = (char*)malloc(sizeof(char) * 40);
    sortType = getHeader(header,inputCol,numP);

    // if headerEval == null, then specified  arg doesnt 
    // exist in the csv

  
 
    
//////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	//holds initial 20000 records
	struct Record * allrecords = 
        (Record *)malloc(sizeof(Record) * 5000);
	//For reallocation
	struct Record *newall = NULL;
	//size of the records array in bytes
	size_t arSize = 5000* (sizeof(Record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
    char* field;
	
	//ptr for specifying struct
	struct Record * ptrrecords = allrecords;
	int numRecords = 0;

	//jumpstart the loop
	bytes = getline(&line, &recordsize, stdin);	


	while (bytes != -1)
	{
		
		//copy to row to free up the line var
		char* row = malloc(sizeof(char) * strlen(line)); 
        row = strdup(line);
		free(line);
		line = NULL;

		if (bytes != -1)
		{

			//increase count of records
			numRecords++;
			//Add to total amount of bytes
			totalbytes += sizeof(Record);
			

			
			
		
			//Check if total bytes goes over
			if(totalbytes > arSize)
			{
				//Add 5000 to the number of input records
				arSize = arSize + (5000 * sizeof(Record));
				printf("\nexpanded arSize\n");

				//reallocate, move pointer to new memory location with more mem
				ptrrecords = realloc(allrecords, arSize);

				
				//If this does not work, there is no more memory left to allocate
				if ( ptrrecords== NULL)
				{
					printf("Out of memory, exiting");
					exit(0);
				}
			}
			
			
			
			//checks for a double quote in the row, which indicates there will be nested commas
			char * check = strstr(row,"\"");
			//If double quotes are present,
			char * qchecker;
			int i;			
	
			//get tokens in the line
			for(i = 0; i < numFields + 1;i++)
			{	

				//get a field
				field = strsep(&row,",");
			
				
				
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

				allocateToken(ptrrecords, field, i);
                
		
			}//end token loop
				
			

		}//end if bytes != -1
		
	//get next line, move pointer of records over
		bytes = getline(&line, &recordsize, stdin);
		if (bytes != -1)
			ptrrecords++;
 
        /*
        free(row);
        row = NULL;
		*/
	}//end while
	
	

	int numStructs = numRecords;
	//if the type is a string, the use the string sort
	if ((strcmp(sortType,"color") == 0)||(strcmp(sortType,"director_name")== 0)||(strcmp(sortType,"actor_name_2")== 0)||(strcmp(sortType,"genres")== 0)||(strcmp(sortType,"actor_1_name")== 0)||(strcmp(sortType,"movie_title")== 0)||(strcmp(sortType,"actor_3_name")== 0)||(strcmp(sortType,"plot_keywords")== 0)||(strcmp(sortType,"movie_imdb_link")== 0)||(strcmp(sortType,"language")== 0)||(strcmp(sortType,"country")== 0)||(strcmp(sortType,"content_rating")== 0))
	  {
	    sortString(allrecords, 0, numStructs - 1,sortType);

	  }
	//Otherwise, use the numsort		
    else
	{
	   sortNum(allrecords, 0, numStructs - 1,sortType);
    }

  
	
    printStructs(allrecords, numStructs);
	
	return 0;
	
	
}//End main


char VerifyMode(char mode)
{
    const char* modes = "cd";
	//If the mode == 'c' (first element in char* modes)
	if (*(modes) == mode)
	{
		
		return 'c';
	}
	if (*(modes + 1) == mode)
	{
		
		return 'd';
	}
	//If a mode is entered that does not exist, return invalid char
	else
	{
		printf("The sorter does not recognize the first argument -%c",mode);
		return 'x';
	}
	
}//End VerifyMode function

