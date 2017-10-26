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
        sort(sortType,numRecords,allrecords);

        return 0;
    }
 
//////////////////////////////////////////// end stdin sort///////////////////////////////////////////////////////////////
    
////////////////////////////////////////////.csv file sort///////////////////////////
    //process the input directory
    
     DIR * pDir = getDirectory(inDir);
     DIR* newDir = processDirectory(pDir);
    
	
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
    stat(path,&buffer);
    if (!(S_ISDIR(buffer.st_mode)))
    {
        printf("\nThe path passed to directory option NOT a directory\n");
        exit(0);
    }
    
    //otherwise, it is legit
    DIR* dir =  opendir(path);
    
    return dir;
   
}//end getDirectory function

///////////////////////////////////////READ & WRITE//////////////////////////////////////////

void readFile(char *fileName, int *pNumRecords, int numFields){ 

	FILE *fp;
	char *line = NULL;
	size_t recSize = 0;

	fp = fopen(fileName, "r");

	if(fp == NULL){
		printf("Error: File does not exist\n");
		exit(0);
	}

	getline(&line, &recSize, fp);

	while(!feof(fp)){
		createTable(pNumRecords, numFields, fp);
	}

	fclose(fp);
}

void writeFile(char *fileName, int numRecords, char *outDir){
	
	FILE *fp;
	char *fileWrite;
	fileWrite = (char *)malloc(strlen(fileName)+strlen(sortType)+9);
	//+9 for "-sorted-" (8) and null terminating 0 (1)
	
	if(fileWrite == NULL){
		printf("Not enough memory...exiting\n");
		exit(0);
	}
	
	fileWrite[0] = '\0';
	
	strcat(fileWrite, fileName);//Append fileName to empty string
	strcat(fileWrite, "-sorted-");//Append "-sorted-" to end
	strcat(fileWrite, sortType);//Append the global variable "sortType"
	
	if(outDir == NULL){
		fp = fopen(fileWrite, "w");
	}
	else
	{
		char *placeToWrite = (char *)malloc(strlen(fileWrite)+strlen(outDir)+2);
		placeToWrite[0] = '\0';
		strcat(placeToWrite, outDir);//Append directory to store file
		strcat(placeToWrite, "/");//Append forward slash
		strcat(placeToWrite, fileWrite);//Append file to write name
		
		fp = fopen(placeToWrite, "w");
	}

	if(fp == NULL){
		printf("Error: File does not exist\n");
		exit(0);
	}

	for(i = 0; i < numRecords, i++){
		fprintf(fp, "%s,%s,%f,%f,%f,%f,%s,%f,%f,%s,%s,%s,%f,%f,%s,%f,%s,%s,%f,%s,%s,%s,%f,%f,%f,%f,%f,%f\n", 
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

	fclose(fp);
	free(fileWrite);
}
