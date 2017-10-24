//Joshua Pineda
//Cory Klish


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "sorter.h"
int main(int argc, char* argv[]) {

////////////////////////////////////Verifying the input arguments////////////////////////////////////    
    // Don't accept less than 2 arguments and more than 6 arguments
	if(argc - 1 < 2 || argc - 1 > 7)
	{
		printf("Wrong number of arguments passed.");
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since argv[0] is the executable, use argv[1]
    //Third argument INPUT directory
	//Third argument is the -d symbol
    //Fifth argument is the -o symbo
    //Fifth Designates OUTPUT directory
    char inputmode = VerifyMode(argv[1]);
    int dirlen, outlen;
    char dir,out;
    char* inDir;
    char* outDir;
    
    if (argc-1 > 2)
    {
        dir = VerifyMode(argv[3]);
        if (dir == 'x')
        {
            printf("\nInput Dir command not recognized, exiting\n");
            exit(0);
        }
        if (argv[4] == NULL)
        {
            printf("\nNo input directory specified, exiting\n");
            exit(0);
        }
        
        //get length of the directory field
        inDir = argv[4];
    }
    
    if (argc-1 > 4)
    {
        out = VerifyMode(argv[5]);
        if (out == 'x')
        {
            printf("\nOutput Dir command not recognized, exiting\n");
            exit(0);
        }
        if (argv[6] == NULL)
        {
            printf("\nNo output directory specified, exiting\n");
            exit(0);
        }
        
        outDir = argv[6];
    }	
////////////////////////Parsing first line for column types and testing user input///////////////////////////////////

////////////////////////Stdin sort///////////////////////////////////////////////////////////////////////////////////
    if (argc-1 == 2)
    {
        //recordsize and line both for getline method
        size_t recordsize;
        char* line = NULL;
        size_t bytes = getline(&line, &recordsize, stdin);

        if (bytes == -1)
        {
            printf("\nEOF, ending program");
            exit(0);
        }
        //create new str to hold getline input
        int len = strlen(line);

        //getSortType updates the number of fields, need a ptr to them
        int numFields = 0;
        int* numP = &numFields; 

        //Validate user input (inputcol) against columns in the given csv
        char* inputCol = argv[2];
        char* sortType = (char*)malloc(sizeof(char) * len);
        sortType = getSortType(line,inputCol,numP);


        int numRecords = 0;
        int* pNumRecords = &numRecords;
        Record* allrecords = createTable(pNumRecords,numFields); 
        sort(sortType,numRecords,allrecords);

        return 0;
    }
 
//////////////////////////////////////////// end stdin sort///////////////////////////////////////////////////////////////
    
////////////////////////////////////////////.csv file sort///////////////////////////
    DIR * pDir = opendir(inDir);
    struct dirent * entry;
    
	
}//End main

char VerifyMode(char* mode)
{
    //list of possible modes in a char*
    const char* modes = "cdo";
    //by default, vmode is a non-valid mode
    char vmode = 'x';
    //for looping through the modes
    int modeLen = strlen(modes);
    int len = strlen(mode);
    if (len > 2 || *(mode) != '-')
    {
        printf("\n %s is not recognized, ending program",mode);
    }
    
	int i;
    for (i = 0; i < modeLen; i++)
    {
        //if the mode in param == something in modes
        if(*(mode+1) == *(modes + i))
            vmode = (*(modes + i));
    }
    
    return vmode;
}//End VerifyMode function