//Joshua Pineda
//Cory Klish

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "sorter.h"
char* header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,\
movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,\
title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";


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
    
    if(VerifyMode(argv[1]) != 'c')
    {
		printf("First argument must be -c\n");
		exit(0);
	} 
	
	if( ((argc - 1) == 6) && VerifyMode(argv[3]) == 'd' && VerifyMode(argv[5]) == 'o' ) //6 arguments
	{
		inDir = strdup(argv[4]);
		outDir = strdup(argv[6]);
	}
	else if( (argc - 1) == 5) //5 arguments
	{
		if(VerifyMode(argv[4]) != 'o')
		{
			inDir = strdup(argv[4]);
			outDir = strdup(argv[4]);
		}
		else if(VerifyMode(argv[4]) == 'o' && VerifyMode(argv[3]) == 'd')
		{
			inDir = ".";
			outDir = strdup(argv[5]);
		}
		else
		{
			printf("Invalid argument passing...Exiting\n");
			exit(0);
		}
	}
	else if( (argc - 1) == 4) //4 arguments
	{
		if(VerifyMode(argv[3]) == 'd' && VerifyMode(argv[4]) == 'o')
		{
			inDir = ".";
			outDir = ".";
		}
		else if(VerifyMode(argv[3]) == 'd' && VerifyMode(argv[4]) != 'o')
		{
			inDir = strdup(argv[4]);
			outDir = strdup(argv[4]);
		}
		else if(VerifyMode(argv[3]) == 'o')
		{
			inDir = ".";
			outDir = strdup(argv[4]);
		}
	}
	else if( (argc - 1) == 2 || (argc - 1) == 3 ) //2 or 3 arguments
	{
		inDir = ".";
		outDir = ".";
	}
	else
	{
		printf("Incorrect formatting of arguments...Exiting\n");
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
    char* inputCol = argv[2];
    char * verification = strstr(header,inputCol);
    initpid = getpid();
   
     
    if (verification == NULL)
    {
		printf("\ninvalid sort type. exiting.\n");
		exit(0);
	}
	else
	{
		printf("Initial PID: %d\n",getpid());
		printf("PID's of Child Processes: ");
		processDirectory(inDir,inputCol,outDir,1);
		
	}
	
	printf("\nTotal number of processes: %d\n", processCounter);

	
	
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

char VerifyMode(char* mode)
{
    //list of possible modes in a char*
    const char* modes = "cdo";
    //by default, vmode is a non-valid mode
    char vmode = 'x';
    //for looping through the modes
    int modeLen = strlen(modes);
    
    /*
    int len = strlen(mode);
    if (len > 2 || *(mode) != '-')
    {
        printf("\n %s is not recognized, ending program\n",mode);
    }
    */
    
	int i;
    for (i = 0; i < modeLen; i++)
    {
        //if the mode in param == something in modes
        if(*(mode+1) == *(modes + i))
            vmode = (*(modes + i));
    }
    
    return vmode;               
}//End VerifyMode function
