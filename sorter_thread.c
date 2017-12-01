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
	
	header = strdup("color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,\
movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,\
title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes");
	
	int numArgs = argc - 1;
	int retval;
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
   
     
    if (verification == NULL)
    {
		printf("\ninvalid sort type. exiting.\n");
		exit(0);
	}
	//initialize kahunaComp with 1024 spaces for array
	else
	{
        //====Init=====
        
		kahunaComp = malloc(sizeof(Record*) * kahunaCompSize);
        kahunaCompPtr = kahunaComp;
        retval = pthread_cond_init(&cv, NULL);
        
        

		tableSizes =  malloc(sizeof(int) * tableSizesLength);
        tablesizeptr = tableSizes;
        
		tidArray = malloc(sizeof(pthread_t) * 50);
        
        //====Output=====
        initTID = pthread_self();
		printf("Initial TID: %d\n", initTID);
		printf("TIDs: ");
		processDirectory(inDir,inputCol,outDir);
        
		
    }
		
		while (runningThreads == 0)
		{
				
		}
		
		pthread_mutex_lock(&runningThreadLock);
		
		while(runningThreads != 0)
		{
			retval = pthread_cond_wait(&cv, &runningThreadLock);
		}
		pthread_mutex_unlock(&runningThreadLock);

	sleep(1.375);

	int j = 0;
   //while(1)
   //{
		
		while(runningThreads == 0)
		{
			
			for(j; j < threadCounter; j++)
			{   
				while (1)
				{
					if (runningThreads == 0)
					{
						pthread_join(tidArray[j], NULL);
						//printf("\njoining on thread %d\n",tidArray[i]);
						break;
					}
					else
						continue;
				}
			}
			
			break;
		}
		//else
			//continue;
	//}
	//printf("Khuna size %d\n", kahunaSize);
    
	bigKahuna = malloc(sizeof(Record) * kahunaSize);
	
	//Must loop on kahunaComp and tableSizes
	//up to their respective indices
	int i = 0;
	
	while (i < kahunaCompIndex)
	{
		kahunaCopy(kahunaComp[i],tableSizes[i]);
		i++;
		
	}
		
	printf("\nTotal number of threads created: %d\n", threadCounter + 1); //+1 for initial thread
	
	sort(inputCol,kahunaSize,bigKahuna);
	writeFile(bigKahuna, outDir, inputCol);
    //printf("Done writing, program over.\n");

	
}//End main

/*
PARAMS:
path is a char* that will be opened using opendir
inputCol is what we are sorting on, which is validated in this
    method
*/

static int processDirectory(char* path, char* inputCol, char* outpath)
{
    //======INITIALIZE: rparray, kahunaCompPtr,
    rparray = malloc(sizeof(ReadParams*) * 50);
 
    //======Packing the params passed from main into a struct=====
    rparray[rpindex] = malloc(sizeof(ReadParams));
    rparray[rpindex]->path = strdup(path);
    rparray[rpindex]->inputCol = strdup(inputCol);
    rparray[rpindex]->outpath = strdup(outpath);
        
    
    //printf("Creating a thread to look at the initial directory, %s\n",path);
    int result = pthread_create(&tidArray[threadIndex],NULL,processDir, rparray[rpindex]);
    flag = 1;
    if (result)
    {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",result);
		exit(EXIT_FAILURE);
		
	}
    threadIndex++;

	//printf(" returning to the main thread\n");
    return 1;
	
}

//==========Function ptr for processDirectory=========
static void *processDir(void* params)
{
	int retval;
    //========Thread Things========
    //if(flag == 0)
    //{
		printf("%lu, ",pthread_self());
	//}
	
	//flag = 0;
	
	pthread_mutex_lock (&runningThreadLock);
					runningThreads++;
	pthread_mutex_unlock (&runningThreadLock);
	
	//================File related Params=============
	struct dirent* entry;
	char* csv = ".csv";
    //========Thread Related Param====================
    int localindex;
	
	

    /*
    Basically locking the struct array, increasing the index,
    and assigning a localindex for each thread
    In this way, everyone has a different index and therefore
    a different struct* to work with
    */
    
    //======Attaining a new index for this thread=======
    pthread_mutex_lock(&rpLock);
        if (rpindex + 1 > rpsize)
        {
            reallocRps();
        }
        rpindex++;
        localindex = rpindex;
    pthread_mutex_unlock(&rpLock);
    
    //=======Struct initialization=============
	rparray[localindex]= malloc(sizeof(ReadParams));
    rparray[localindex] = params;
    
    //======Assigning values from the copied struct to local vars=======
    char * path = strdup(rparray[localindex]->path);
    char * inputCol = strdup(rparray[localindex]->inputCol);
    char * outpath = strdup(rparray[localindex]->outpath);
    
   
    //=======Opening the directory that was specified by "path", received by params======
    DIR* directory  = opendir(path);
    
    //========Error checking the opendir=============
    if (directory == NULL)
    {
		pthread_mutex_lock (&runningThreadLock);
		pthread_mutex_lock(&safetylock);
                    runningThreads--;
		pthread_mutex_unlock (&runningThreadLock);  
		pthread_mutex_unlock(&safetylock);
		
		fflush(stdout);
		printf("\nDirectory path %s cant be opened\n",rparray[localindex]->path);
		pthread_exit(&threadCounter);
	}
    
    //=========Loop where the directory is read=========================================
    
    while ((entry = readdir(directory)) != NULL )
    { 
        //===Entryindex records the rpindex for the specific directory entry(element)===
        int entryindex;
       
        //===============Taking out unnecessary directories====================
		if ( (strcmp (entry->d_name,"."))!= 0 && (strcmp (entry->d_name,"..")) != 0 && (strcmp (entry->d_name,".git")) != 0)
		{
            int len = strlen(path);
			struct stat buffer;
			char dpath[255];
			dpath[0] = '\0';
            //===================If the directory entry is a "." or a "./"=======
			if (strcmp(path,"./") == 0)
			{
				strcat(dpath,entry->d_name);
				strcat(dpath,"\0");
			}
			else if(strcmp(path,".") == 0)
			{
				strcat(dpath,entry->d_name);
				strcat(dpath,"\0");
			}
			//=============If the path is not "." or "./"==================
			else
			{
				//=========Either the directory path ends w/ "/" or not========
				if (*(path + len - 1) == '/')
				{
					strcpy(dpath,path);
					strcat(dpath,entry->d_name);
					strcat(dpath,"\0");
				}
				else
				{
					strcpy(dpath,path);
					strcat(dpath,"/");
					strcat(dpath,entry->d_name);
					strcat(dpath,"\0");
				}
			}
        //================Creation of another struct to handle the directory element===============
            pthread_mutex_lock(&rpLock);
            if (rpindex + 1 > rpsize)
                {
                    reallocRps();
                }
                rpindex++;
                entryindex = rpindex;
            pthread_mutex_unlock(&rpLock);
            rparray[entryindex] = malloc(sizeof (ReadParams));

            

        //============Directory Section======================            
		if (entry->d_type == DT_DIR)
			{
			          
				rparray[entryindex]->path = strdup(dpath);
				rparray[entryindex]->inputCol = strdup(inputCol);
				rparray[entryindex]->outpath = strdup(outpath);
            
                pthread_mutex_lock (&tidArrayLock);
            //printf("state of struct in DT_DIR: Path: %s\n",dpath);
					
					if(threadCounter + 1 > arrSize)
					{
						reallocThread();
					}
                    
					
					int result = pthread_create(&tidArray[threadIndex],NULL,processDir, rparray[entryindex]);
					
					if (result)
					{
						fprintf(stderr,"Error - pthread_create() return code: %d\n",result);
						exit(EXIT_FAILURE);
						
					}
                    threadIndex++;
					threadCounter++;
					
                pthread_mutex_unlock(&tidArrayLock);
                		
			}

            //============Regular File Section======================
			if (entry->d_type == DT_REG)//if entry = regular file
			{
				rparray[entryindex]->path = strdup(dpath);
				rparray[entryindex]->inputCol = strdup(inputCol);
				rparray[entryindex]->outpath = strdup(outpath);
				char* filename = (entry->d_name);
                rparray[entryindex]->filename = strdup(filename);
                
	          
				char* fileext = strstr(filename, csv);
					if (fileext != NULL)
					{
						int numRecords = 0;
						int* pNumRecords = &numRecords;
						
						char* sorted = strstr(filename,"-sorted-");
						if (sorted != NULL)
						{
						   continue;
						}
						
						//If it is not already a sorted file
						else
						{
                       
                        pthread_mutex_lock (&tidArrayLock);
								
							
								if(threadCounter + 1 > arrSize)
								{
									reallocThread();
								}
								//printf("Current state of the struct in DT_REG: Path: %s, FileName: %s\n",rparray[entryindex]->path,rparray[entryindex]->filename);
								int result = pthread_create(&tidArray[threadIndex],NULL,getFile,rparray[entryindex]);
								
								if (result)
								{
									fprintf(stderr,"Error - pthread_create() return code: %d\n",result);
									exit(EXIT_FAILURE);
									
								}
                                threadIndex++;
                        pthread_mutex_unlock(&tidArrayLock);
                            
						}
						

					}
					//if file ext is not csv, do nothing
			 
			}//end if regular file
			
        }//end if
    }//end whileloop for readdir
    //====================Ending the readdir directory loop=======================
    
    //=====================Thread Things=========================
    pthread_mutex_lock (&runningThreadLock);
    pthread_mutex_lock(&safetylock);
    //=======Decrement rt when processdir ends===========
					runningThreads--;
					if (runningThreads == 0)
						retval = pthread_cond_signal(&cv);
    pthread_mutex_unlock (&runningThreadLock); 
    pthread_mutex_unlock(&safetylock); 
   
    //printf("\nI am now exiting thread %d\n",pthread_self());
    pthread_exit(&threadCounter);
     
	return 0;
    
} 

////////////////////////////function ptr for processFile.
static void *getFile(void* params)
{	
	int retval;
    //=======Increase number of threads, and running threads
    pthread_mutex_lock (&runningThreadLock);
            runningThreads++;
            threadCounter++;
    pthread_mutex_unlock (&runningThreadLock);
    
    //===========Attaining a new struct to unpack params arguments=======
   int localindex;
   pthread_mutex_lock(&rpLock);
        if (rpindex + 1 > rpsize)
        {
            reallocRps();
        }
        rpindex++;
        localindex = rpindex;
    pthread_mutex_unlock(&rpLock);
    
    rparray[localindex]= malloc(sizeof(ReadParams));
    rparray[localindex] = params;
    
    //============Assigning local variables components of the struct========
    char* path  = strdup(rparray[localindex]->path);
    char* inputCol = strdup(rparray[localindex]->inputCol);
    char* filename = strdup(rparray[localindex]->filename);
    free(rparray[localindex]->path);
    free(rparray[localindex]->inputCol);
    free(rparray[localindex]->filename);
    
    
   // printf("getFile params received: Path: %s\n",rparray[localindex] -> path);
	printf("%lu, " , pthread_self());

    //=============Components for creating a table==============
    int numRecords = 0;
    int* pNumRecords = &numRecords;
    //readfile validates the input column and creates a record array
    //char** pheader will change the value of 
    //char* header from within readFile
    char* nheader;
    char** pHeader = &nheader;
  
    

    //=================Creates table, fills numrecords, and fills header (deprecated at this point?)=========
    Record * table = readFile(filename, pNumRecords, 0, inputCol, pHeader,path);
    
    //========+=========Sort the table==================================
    sort(inputCol, numRecords,table);
    //printf("numRecords is %d\n",numRecords);

    //==============================================The Big Lock=================================================
    pthread_mutex_lock(&kahunacountLock);//LOCK the LOCK
    if (tableSizeIndex + 1 > tableSizesLength)
    {
         //==================Tablesize Realloc========================================================
        tableSizesLength += 256;
        tableSizes = (int*)realloc(tableSizes,tableSizesLength);
        if (tableSizes == NULL)
        {
            printf("Realloc error, cannot create more space for table lengths\n\n");
            exit(0);

        }
        //=================Move index now that it wont go over=====================
        tableSizeIndex += 1;
        //================Assign value of numRecords to the index in Tablesizes=============
        *tablesizeptr = numRecords;
        tablesizeptr += 1;
        tableSizeIndex += 1;
        //================Add to accumulating kahunaSize====================================
        kahunaSize += *pNumRecords;
        //printf("Kahuna SIeze %d\n",kahunaSize);
    }
    
   
//==================End Tablesize Realloc=======================================================

//==================If no realloc, do regular stuff===================
    else
    {
        //===========Assign value to index in tablesizes============
        *tablesizeptr =  numRecords;
        tableSizeIndex += 1;
        tablesizeptr += 1;
        //============Add to accumulating kahunaSize============
        kahunaSize += numRecords;			
        //printf("Proper KahunaSize is %d",kahunaSize);
    }

    //======================KahunaComp Realloc====================================================
    if (kahunaCompIndex + 1 >= kahunaCompSize)
    {
        kahunaCompSize += 256;
        kahunaComp = realloc(kahunaComp,kahunaCompSize);
        if (kahunaComp == NULL)
        {
            printf("Realloc error, cannot create more space for tables\n");
            exit(EXIT_FAILURE);
        }
        //==============Move the pointer to new position of kahunaComp========
        kahunaCompPtr = kahunaComp;
        //========Go back to the position it was before the realloc + 1========
        kahunaCompPtr += kahunaCompIndex + 1;

        //==============Malloc the position kahunaCompPtr is at, give it 'table' value===========
        *kahunaCompPtr = malloc(sizeof(Record) * numRecords);
        *kahunaCompPtr = table;

        //==============Move the index and the pointer over one =======================
        kahunaCompIndex += 1;
        kahunaCompPtr += 1;
    }
    else
    { 
        int i;
        //=====Malloc the KahunaCompPtr's position, set it equal to table (copies by value)===========
        *kahunaCompPtr = malloc(sizeof(Record) * numRecords);
        
        //replace this with memcpy
        
        *kahunaCompPtr = table;

        //=====Move the index and pointer over by one===============
        kahunaCompIndex += 1;
        kahunaCompPtr += 1;
    }
		
	pthread_mutex_unlock(&kahunacountLock);//UNLOCK LOCK
	//========================================================The End of Big Lock=========================================

   
    pthread_mutex_lock (&runningThreadLock);
    pthread_mutex_lock(&safetylock);
    //=======Decrement rt when getfile is over=======
		runningThreads--;
		if (runningThreads == 0)
			retval = pthread_cond_signal(&cv);
	pthread_mutex_unlock (&runningThreadLock);
	pthread_mutex_unlock(&safetylock);
	
	//printf("Exiting with thread ID %d\n",pthread_self());
	pthread_exit(&threadCounter);
    
}

///////////////////////////////////////READ & WRITE//////////////////////////////////////////
//Large Helper function: readFile
/*
PARAMS
filename is given
pNumRecords will initially be a pointer to a number that is 0
    createTable method counts the number of records and changes the value of pNumRecords
numFields is initially 0 as well and its value is given by the 
    getSortType function
inputCol is the given type to sort by. getSortType checks if the input from the user is in the csv file that is currently being read. if it is and the csv also has 27 fields then it is legit and we create a method
header is a null char* that is to be filled with the header of 
    a csv file. This is needed when we need to write the header at the top of -sorted csv- files.
RETURNS: A Record* of the given csv file
*/
static Record * readFile(char *fileName, int *pNumRecords, int numFields, char* inputCol, char** pHeader, char* inpath)
{ 

    //open the file for reading
	FILE *fp;
    if (strcmp(inpath,".") == 0 || (strcmp(inpath,"./") == 0))
    {
        fp = fopen(fileName, "r");
		//printf("I am attempting to open the file: %s\n",fileName);

	}   
    else
    { 
		/*
        int plen = strlen(inpath);
        int flen = strlen(fileName);
        
        char* pathtofile = (char*)malloc(sizeof(char) * (plen + flen) + 2);
        pathtofile[0] = '\0';
        strcat(pathtofile,inpath);
        strcat(pathtofile,"/");
        strcat(pathtofile,fileName);
        * */
        
        //printf("I am attempting to open the file: %s\n",inpath);

        fp = fopen(inpath,"r");
        
    }
    
    
    

    //validation if real
	if(fp == NULL)
    {
		printf("The file read of path %s with filename %s was a failure\n", inpath, fileName);
		exit(0);
	}
	//printf("The file read of path %s with filename %s was a success\n",inpath,fileName);
    //taking the header
    size_t recordsize;
    char* line = NULL;
    //get a line from the file
    size_t bytes = getline(&line, &recordsize, fp);

    if (bytes == -1)
    {
        printf("\nEOF reached\n");
        exit(0);
    }
    
    int len = strlen(line);
    //copy the header into "header" variable
    *pHeader = malloc(sizeof(char) * len);
    *pHeader = strdup(line);
    
    //pointer to numfields in order to change its value
    int* numP = &numFields;
    //getting the sortType
    char* sortType = malloc(sizeof(char) * len);
    //getSortType also gets the number of fields
    sortType = getSortType(line,inputCol,numP);
    free(line);
 
	/*
    if (*numP != 27)
    {
        printf("not the correct number of columns. exiting. \n");
        exit(0);
    }
    * */
    pthread_mutex_lock(&recordlock);
    Record* newRecords = createTable(pNumRecords, numFields, fp);
    pthread_mutex_unlock(&recordlock);
	fclose(fp);
    return newRecords;
}
//takes in the record set as "list"

static void writeFile(Record list[], char *outDir, char* sortType){
	
	//AllFiles-sorted-<fieldname>.csv
	
	FILE *fp;
	char *fileWrite;
    
	fileWrite = (char *)malloc(strlen(sortType) + 21);
		
	if(fileWrite == NULL){
		printf("Not enough memory...exiting\n");
		exit(0);
	}
	/// Section where the sorted file name is created
	fileWrite[0] = '\0';
	
	strcat(fileWrite, "AllFiles-sorted-");
	strcat(fileWrite, sortType);
    strcat(fileWrite,".csv");
	/// end section
   
    ///Section where the space for the filename and output 
    ///directory is allocated
        //string that will hold the directory + filename
        char *placeToWrite = (char*)malloc(strlen(fileWrite) + strlen(outDir) + 2);
        placeToWrite[0] = '\0';
    ///End section
    
    //if the outdir is the current directory or not even existing
	if(strcmp(outDir,".") == 0){
        strcat(placeToWrite,"./");
        strcat(placeToWrite,fileWrite);
		fp = fopen(placeToWrite, "w");
	}
	else
	{
       
        
        //checking if the output directory exists
        DIR* testDir = opendir(outDir);
        if (testDir)
        {
            //the directory exists and we can just append file to the name
            strcat(placeToWrite, outDir);//Append directory to store file
            strcat(placeToWrite, "/");//Append forward slash
            strcat(placeToWrite, fileWrite);//Append file to write name
            fp = fopen(placeToWrite,"w");
        }
	
		else
        {
            //If it doesnt exist, make a new directory
            //0744 is the permissions of the directory
            //user and group have read-only access
            mkdir(outDir,0744);
            strcat(placeToWrite, outDir);//Append directory to store file
            strcat(placeToWrite, "/");//Append forward slash
            strcat(placeToWrite, fileWrite);//Append file to write name
            fp = fopen(placeToWrite, "w");
        }
	}

	if(fp == NULL){
		printf("Error: OutDir not found.\n");
		exit(0);
	}
    
    int i;
	for(i = 0; i < (kahunaIndexCount); i++){
       if (i == 0)
           fprintf(fp,"%s\n",header);
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
	//free(fileWrite);
}
static char* getSortType(char* header, char* colName, int* numFields)
{
    //If colName matches with a field, colName is copied into
    //sortType
    char* sortType = NULL;
    int len;
    //put ptr on first char of line
    char* field = strsep(&header,",");
    
    if (field == NULL)
	{
		printf("ERROR, no fields");
        exit(0);
	}
    //Just in case first column is the column to be sorted
	if (strcmp(field, colName) == 0)
	{
		//Get length, allocate size and copy into 'column' variable
		len = strlen(field);
		sortType = malloc((sizeof(char) * len) + 1);
		sortType = strdup(field);
	}
    
    //Count loop
	while (field != NULL)
	{		
		//Subsequent calls to strtok move the pointer and return the actual token
		field = strsep(&header, ",");
		//Then i can start counting tokens
		if (field != NULL)
		{
			*numFields = *numFields + 1;
			
			//Then check if that column is equivalent to the argument passed
			if (strcmp(field, colName) == 0)
			{
				
				//dynamic allocate the mem and store string
				len = strlen(field);
				sortType = malloc(sizeof(char) * len);
				sortType = strdup(field);
				
			}
		}
			
	}//End while counting loop
    /*
    if (*numFields != 27)
    {
        printf("\nWrong number of columns in csv.\n");
        exit(0);
    }
    */
    free(field);

    return sortType;

}//end getSortType function
void reallocRps()
{
    rpsize += 256;
    pthread_mutex_lock(&rpLock);
        rparray = realloc(rparray,rpsize);
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
		tidArray = realloc(tidArray, arrSize);
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
   
	char *rootaccess = malloc(sizeof(char) * 7);
	char *rootaccess2 = malloc(sizeof(char) * 6);
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
	//free(rootaccess);	
	//free(rootaccess2);
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
