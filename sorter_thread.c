//Joshua Pineda
//Cory Klish

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sorter_thread.h"
#include <pthread.h>
char* header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,\
movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,\
title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";


int main(int argc, char* argv[]) {

	int numArgs = argc - 1;
////////////////////////////////////Verifying the input arguments////////////////////////////////////    
    // Don't accept less than 2 arguments and more than 6 arguments
	if(numArgs < 2 || numArgs > 6)
	{
		printf("Wrong number of arguments passed\n");
		exit(0);
	}
	
	//First argument should be -mode, indicating what to analyze
	//Since argv[0] is the executable, use argv[1]
    //Third argument INPUT directory
	//Third argument is the -d symbol
    //Fifth argument is the -o symbo
    //Fifth Designates OUTPUT directory
   
    int dirlen, outlen;
    char dir = 0,out = 0;
    char* inDir;
    char* outDir;
    char* inputCol;
    
    inputCol = getArgs('c', numArgs, argv);
    inDir = getArgs('d', numArgs, argv);
    outDir = getArgs('o', numArgs, argv);
   
    if(outDir == NULL)
    {
		outDir = strdup(inDir);
	}
     	
     if(!VerifyDirectory(inDir) || !VerifyDirectory(outDir))
     {
		 printf("Not a valid directory\n");
		 exit(0);
	 }
////////////////////////Parsing first line for column types and testing user input///////////////////////////////////

    /*
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
        sort(sortType,numRecords,allrecords);
        return 0;
    }
 
//////////////////////////////////////////// end stdin sort///////////////////////////////////////////////////////////////
    */
////////////////////////////////////////////.csv file sort///////////////////////////
    //process the input directory
   
    char * verification = strstr(header,inputCol);
    inittid = gettid();
   
     
    if (verification == NULL)
    {
		printf("\ninvalid sort type. exiting.\n");
		exit(0);
	}
	else
	{
		printf("Initial TID: %d\n",gettid());
		printf("TIDs: ");
		processDirectory(inDir,inputCol,outDir);
		
	}
    
	//
    
    
	printf("\nTotal number of threads: %d\n", threadCounter);

	
	
}//End main

int VerifyDirectory(char* path)
{
    int flag = 0;
    if (path == NULL)
    {
        printf("\npath is null.");
        exit(0);
    }
   
	char *rootaccess = (char *)malloc(sizeof(char) * 7);
	char *rootaccess2 = (char *)malloc(sizeof(char) * 6);
	strncpy(rootaccess, path, 6);
	strncpy(rootaccess2, path, 5);
	rootaccess[6] = '\0';
	rootaccess2[5] = '\0';
	if (strcmp(rootaccess,"/root/") == 0 || strcmp(rootaccess2,"/root") == 0 || strcmp(rootaccess2, "root/") == 0)
	{
		printf("No permissions to access root directory\n");
		exit(0);
	}
		
	free(rootaccess);
	free(rootaccess2);
	rootaccess = NULL;
	rootaccess2 = NULL;
		
     struct stat buffer;
     //get infomation about the path, put into a buffer
    if (stat(path,&buffer) != 0)
        return flag;
    if (S_ISDIR(buffer.st_mode))
    {
        //this is a directory
       flag = 1;
    }
    return flag;
}
char* getArgs(char flag, int numArgs, char* argArr[])
{
	int i = 1;
	int found = 0;
	char * argVal = NULL;
	
	switch(flag)
	{
		case 'c':
		{
			while(i <= numArgs)
			{
				if(strcmp(argArr[i], "-c") == 0)
				{
					
					found = 1;
					
					if(i == numArgs || strcmp(argArr[i+1], "-d") == 0 || strcmp(argArr[i+1], "-o") == 0 || strcmp(argArr[i+1], "-c") == 0)
					{
						printf("Error, '-c' must have an argument\n");
						exit(0);
					}
					
					argVal = strdup(argArr[i+1]);
					break;
				}
				
				i++;
			}
			
			if(found == 0)
			{
				printf("Error: No sort type found\n");
				exit(0);
			}
			
			break;
		}
		
		case 'd':
		{
			while(i <= numArgs)
			{
				if(strcmp(argArr[i], "-d") == 0)
				{
					if(i == numArgs) // "-d" is last arg
					{
						argVal = strdup(".");
					}
					else if(strcmp(argArr[i+1], "-d") == 0)
					{
						printf("Error: '-d' is a command\n");
						exit(0);
					}
					else if(strcmp(argArr[i+1], "-c") == 0 || strcmp(argArr[i+1], "-o") == 0)
					{
						argVal = strdup(".");
					}
					else
					{
						argVal = strdup(argArr[i+1]);
					}
				}
				i++;
			}
			
			if(argVal == NULL)
			{
				argVal = strdup(".");
			}
			
			break;
		}
		
		case 'o':
		{
			while(i <= numArgs)
			{
				if(strcmp(argArr[i], "-o") == 0)
				{
					if(i == numArgs) // "-o" is last arg
					{
						argVal = NULL;
					}
					else if(strcmp(argArr[i+1], "-o") == 0)
					{
						printf("Error: '-o' is a command\n");
						exit(0);
					}
					else if(strcmp(argArr[i+1], "-c") == 0 || strcmp(argArr[i+1], "-d") == 0)
					{
						argVal = NULL;
						break;
					}
					else
					{
						argVal = strdup(argArr[i+1]);
						break;
					}
				}
				i++;
			}
			
			break;
		}
	}
	return argVal;
}
