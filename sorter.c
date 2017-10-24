//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sorter.h"
int main(int argc, char* argv[]) {
    
	if(argc - 1 < 2)
	{
		printf("Wrong number of arguments passed.");
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since argv[0] is the executable, use argv[1]
    //Third argument is optional directory mode
	//Third argument is the -d symbol
    char* inputmode = argv[1];
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
 

    
    char* inputCol = argv[2];
    //evaluates the header, assume size of 40
    char* sortType = (char*)malloc(sizeof(char) * len);
    sortType = getSortType(line,inputCol,numP);
    
    int numRecords = 0;
    int* pNumRecords = &numRecords;
    Record* allrecords = createTable(pNumRecords,numFields); 


    
    sort(sortType,numRecords,allrecords);
	
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

