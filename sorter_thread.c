//Joshua Pineda
//Cory Klish

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sorter_thread.h"

int main(int argc, char** argv) {

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
     	
     if(VerifyDirectory(inDir) == 0 || CheckDirectory(outDir) == 0)
     {
		 printf("Not a valid directory\n");
		 exit(0);
	 }

////////////////////////////////////////////.csv file sort///////////////////////////
    //process the input directory
   
    char * verification = strstr(header,inputCol);
    inittid = pthread_self();
   
     
    if (verification == NULL)
    {
		printf("\ninvalid sort type. exiting.\n");
		exit(0);
	}
	//initialize kahunaComp with 1024 spaces for array
	
	
	else
	{
        //====Init=====
		kahunaComp = (Record**)malloc(sizeof(Record*) * kahunaCompSize);
        kahunaCompPtr = kahunaComp;

		tableSizes = (int*) malloc(sizeof(int) * tableSizesLength);
        tablesizesptr = tableSizes;
        
		tidArray = malloc(sizeof(pthread_t) * 50);
        
        //====Output=====
		printf("Initial TID: %d\n",pthread_self());
		printf("TIDs: ");
		processDirectory(inDir,inputCol,outDir);
		
	}
    sleep(2);
	//The joining loop
 
    while(1)
    {
		if(runningThreads == 0)
		{
            printf("Thread IDs in tidarray: ");
			int i;
            for(i = 0; i < threadCounter; i++)
			{
                printf("%d, ",tidArray[i]);
			}
			
			for(i = 0; i < threadCounter; i++)
			{
            //this is SEGFAULTING idk
                
				pthread_join(tidArray[i], NULL);
                printf("\njoining on thread %d\n",tidArray[i]);
			}
			
			break;
		}
		else
			continue;
	}
	printf("Khuna size%d\n",kahunaSize);
    
	bigKahuna = (Record*)malloc(sizeof(Record) * kahunaSize);
	
	//Must loop on kahunaComp and tableSizes
	//up to their respective indices
	int i = 0;
	while (i <= kahunaCompIndex)
	{
		kahunaCopy(kahunaComp[i],tableSizes[i]);
		i++;
		
	}
		
	printf("\nTotal number of threads: %d\n", threadCounter);
	
	sort(inputCol,kahunaSize,bigKahuna);
	writeFile(bigKahuna, outDir, inputCol);
    printf("Done writing, program over.\n");

	
}//End main
void reallocRps()
{
    rpsize += 256;
    pthread_mutex_lock(&rpLock);
        rparray = (ReadParams**)realloc(rparray,rpsize);
    pthread_mutex_lock(&rpLock);
    if (rparray == NULL)
    {
        printf("Realloc error with structs...exiting\n");
        exit(EXIT_FAILURE);
    }

}
void reallocThread()
{
	arrSize += 50;
	pthread_mutex_lock(&tidArrayLock);
		tidArray = (pthread_t*)realloc(tidArray, arrSize);
	pthread_mutex_unlock(&tidArrayLock);
	if(tidArray == NULL)
	{
		printf("Realloc error...exiting\n");

		exit(0);
	}
}

int VerifyDirectory(char* path)
{
    int flag = 0;
	if (CheckDirectory(path))
	{
		struct stat buffer;
		 //get infomation about the path, put into a buffer
		if (stat(path,&buffer) != 0)
			return flag;
		if (S_ISDIR(buffer.st_mode))
		{
			//this is a directory
		   flag = 1;
		}
		
	}
	return flag;
     
}
int CheckDirectory(char* path)
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
		
	
	rootaccess = NULL;
	rootaccess2 = NULL;
	free(rootaccess);	
	free(rootaccess2);
	flag = 1;
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
