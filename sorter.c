//Joshua Pineda
//Cory Klish

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
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
    char dir = 0,out = 0;
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
        inDir = malloc(sizeof(char) * (strlen (argv[4])));
        inDir = strdup(argv[4]);
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
        outDir = malloc(sizeof(char) * (strlen(argv[6])));
        outDir = strdup(argv[6]);
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
        Record* allrecords = createTable(pNumRecords,numFields, NULL); 
        sort(sortType,numRecords,allrecords,1);

        return 0;
    }
 
//////////////////////////////////////////// end stdin sort///////////////////////////////////////////////////////////////
    
////////////////////////////////////////////.csv file sort///////////////////////////
    //process the input directory
    char* inputCol = argv[2];
    //if there is a -d option, which was validated
    if (dir)
    {
        DIR * pDir = getDirectory(inDir);
        DIR* newDir = processDirectory(pDir,inputCol);
    }
    if (dir && out)
    {
        /* do same thing and other stuff*/
    }
    else
    {
        /* do stuff in original directory
        we can open the current directory with
        opendir(".");*/
    }
    
	
}//End main

DIR* getDirectory(char* path)
{
    if (path == NULL)
    {
        printf("\nNo Directory given");
        exit(0);
    }
    
    //verifying that the path passed is actually a directory
    struct stat buffer;
    //get infomation about the path, put into a buffer
    if (stat(path,&buffer) != 0)
        return 0;
    if (!S_ISDIR(buffer.st_mode))
    {
         //if legit
        printf("This is not a legit directory. ENding");
        exit(0);
        
    }
    DIR* newDir = opendir(path);
        return newDir;
    
   
   
}//end getDirectory function

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
