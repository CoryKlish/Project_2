/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/


#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
typedef struct ReadParams 
{
    char* path;
    char* inputCol;
    char* outpath;
    char* filename;

} ReadParams;

typedef struct Record{
	char color[30];
	char director_name[50];
	double num_critic_for_reviews;
	double duration;
	double director_facebook_likes;
	double actor_3_facebook_likes;
	char actor_2_name[50];
	double actor_1_facebook_likes;
	double gross;
	char genres[70];
	char actor_1_name[50];
	char movie_title[70];
	double num_voted_users;
	double cast_total_facebook_likes;
	char actor_3_name[50];
	double facenumber_in_poster;
	char plot_keywords[100];
	char movie_imdb_link[70];
	double num_user_for_reviews;
	char language[50];
	char country[40];
	char content_rating[30];
	double budget;
	double title_year;
	double actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	double movie_facebook_likes;
	
	
} Record;

static pthread_mutex_t tidArrayLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t kahunaLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t kahunacountLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t runningThreadLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t kahunaCompLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t rpLock = PTHREAD_MUTEX_INITIALIZER;
static char* header = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,\
movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,\
title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";

//=========Thread Id Section=========
static pthread_t* tidArray;
static int arrSize = 50;
static int threadCounter = 0;
static int threadIndex = 0;
static int runningThreads = 0;
static int inittid;

//==========Table/Arrays are accumulated here=========
static Record* bigKahuna;
static int kahunaIndexCount = 0;
static int kahunaSize = 0;


//===========List of tables/arrays that go inside bigkahuna=====
static Record** kahunaComp;
static Record** kahunaCompPtr;
static int kahunaCompIndex = 0;
static int kahunaCompSize = 1024;

//========Stores the number of records in each array in Kahunacomp=====
static int* tableSizes;
static int tableSizesLength = 1024;
static int tableSizeIndex = 0;


//==========Array of structs: Each struct goes to a thread/call to processdir or getfile=======
static ReadParams** rparray;
static int rpindex = 0;
static int rpsize = 256;

//===============Prototypes===============

//===================SORTER.H=================
static void *processDir(void* params);
static void *getFile(void* params);
static void allocateToken(Record*, char*, int);
static  char* getSortType(char* header,char* colName, int* numFields);
static void sort (char* sortType, int numStructs, Record*);
static void printStructs(Record list[], int numStructs);
static int processDirectory( char* path, char* inputCol, char* outpath);
static Record * readFile(char *fileName, int *pNumRecords, int numFields, char* inputCol,char** pHeader, char* inpath);
static void writeFile(Record list[], char *outDir, char* sortType);
static void kahunaCopy(Record list[], int numRecords);

//=============In SORTER.C=====================
int VerifyDirectory(char* path);
int CheckDirectory(char* path);
char* getArgs(char flag, int numArgs, char* argArr[]);
void reallocThread();
void reallocRps();

//=================MERGESORT.C==================
Record* createTable(int* pNumRecords,int numFields, FILE *fp);
void mergeNum(Record list[], int left, int mid, int right,char* sortType);
void sortNum(Record list[], int left, int right,char* sortType);
void sortString(Record strArr[], int lo, int hi,char* sortType);
void mergeString(Record strArr[], int lo, int mid, int hi,char* sortType);



//Large helper function: getHeader
/*
Params:
header is the very first row of the csv, which is attained outside this method
colName is the desired column to be sorted
numFields is passed to this method so that we can count the
    number of columns in the csv. Used later on to validate 
    the csv. 
    
Gets token one by one from the header
Compares token with colName
Counts how many tokens were taken from header
At the end:
    If no matches with colName, return NULL
    otherwise, return colName
*/

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
		sortType = (char*)malloc((sizeof(char) * len) + 1);
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
				sortType = (char*)malloc(sizeof(char) * len);
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

    return sortType;

}//end getSortType function
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
    if (result)
    {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",result);
		exit(EXIT_FAILURE);
		
	}
    threadIndex++;

	//printf(" returning to the main thread\n");
    return 1;
	
}//End processDirectory function
//==========Function ptr for processDirectory=========
static void *processDir(void* params)
{
    //========Thread Things========
	printf("%d, ",pthread_self());
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
                    runningThreads--;
		pthread_mutex_unlock (&runningThreadLock);  
	   
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
					runningThreads--;
    pthread_mutex_unlock (&runningThreadLock);  
   
    fflush(stdout);
    //printf("\nI am now exiting thread %d\n",pthread_self());
    pthread_exit(&threadCounter);
    

    
} 

////////////////////////////function ptr for processFile.
static void *getFile(void* params)
{
    //=======Increase number of threads, and running threads
    threadCounter++;
    pthread_mutex_lock (&runningThreadLock);
            runningThreads++;
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
    
    
   // printf("getFile params received: Path: %s\n",rparray[localindex] -> path);
	printf("%d, " , pthread_self());

    //=============Components for creating a table==============
    int numRecords = 0;
    int* pNumRecords = &numRecords;
    //readfile validates the input column and creates a record array
    //char** pheader will change the value of 
    //char* header from within readFile
    char* header;
    char** pHeader = &header;
  
    

    //=================Creates table, fills numrecords, and fills header (deprecated at this point?)=========
    Record * table = readFile(filename, pNumRecords, 0, inputCol, pHeader,path);
    
    //========+=========Sort the table==================================
    sort(inputCol, numRecords,table);
    printf("numRecords is %d\n",numRecords);

    //==============================================The Big Lock=================================================\\
    pthread_mutex_lock(&kahunacountLock);//LOCK the LOCK
    if (tableSizeIndex + 1 > tableSizesLength)
    {
         //==================Tablesize Realloc========================================================\\
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
        tableSizes[tableSizeIndex] = numRecords;
        tableSizeIndex += 1;
        //================Add to accumulating kahunaSize====================================
        kahunaSize += *pNumRecords;
        printf("Kahuna SIeze %d\n",kahunaSize);
    }
    
   
//==================End Tablesize Realloc=======================================================\\

//==================If no realloc, do regular stuff===================
    if (tableSizeIndex + 1 < tableSizesLength)
    {
        //===========Assign value to index in tablesizes============
        tableSizes[tableSizeIndex] = numRecords;
        tableSizeIndex += 1;
        //============Add to accumulating kahunaSize============
        kahunaSize += numRecords;			
        printf("Proper KahunaSize is %d",kahunaSize);
    }

    //======================KahunaComp Realloc====================================================\\
    if (kahunaCompIndex + 1 >= kahunaCompSize)
    {
        kahunaCompSize += 256;
        kahunaComp = (Record**)realloc(kahunaComp,kahunaCompSize);
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
        *kahunaCompPtr = (Record*)malloc(sizeof(Record) * numRecords);
        *kahunaCompPtr = table;

        //==============Move the index and the pointer over one =======================
        kahunaCompIndex += 1;
        kahunaCompPtr += 1;
    }
    if (kahunaCompIndex + 1 < kahunaCompSize)
    {
        //=====Malloc the KahunaCompPtr's position, set it equal to table (copies by value)===========
        *kahunaCompPtr = (Record*)malloc(sizeof(Record) * numRecords);
        *kahunaCompPtr = table;

        //=====Move the index and pointer over by one===============
        kahunaCompIndex += 1;
        kahunaCompPtr += 1;
    }
		
	pthread_mutex_unlock(&kahunacountLock);//UNLOCK LOCK
	//========================================================The End of Big Lock=========================================\\

   
    pthread_mutex_lock (&runningThreadLock);
		runningThreads--;
	pthread_mutex_unlock (&runningThreadLock);
	
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
		printf("I am attempting to open the file: %s\n",fileName);

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
        
        printf("I am attempting to open the file: %s\n",inpath);

        fp = fopen(inpath,"r");
        
    }
    
    
    

    //validation if real
	if(fp == NULL)
    {
		printf("The file read of path %s with filename %s was a failure\n", inpath, fileName);
		exit(0);
	}
	printf("The file read of path %s with filename %s was a success\n",inpath,fileName);
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
    *pHeader = (char*)malloc(sizeof(char) * len);
    *pHeader = strdup(line);
    
    //pointer to numfields in order to change its value
    int* numP = &numFields;
    //getting the sortType
    char* sortType = (char*)malloc(sizeof(char) * len);
    //getSortType also gets the number of fields
    sortType = getSortType(line,inputCol,numP);
 
	/*
    if (*numP != 27)
    {
        printf("not the correct number of columns. exiting. \n");
        exit(0);
    }
    * */
    
    Record* newRecords = createTable(pNumRecords, numFields, fp);
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
	for(i = 0; i < (kahunaIndexCount + 1); i++){
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

static void kahunaCopy(Record list[], int numRecords)
{
	int i = 0;
	
	while(i != numRecords-1)
	{
		bigKahuna[kahunaIndexCount] = list[i];
		kahunaIndexCount++;
		i++;
	}
	
}

//Large helper function: allocateToken
/*
	Allocate tokens to their fields based on the index of the loop. 
	First index goes to color (first column), second index goes to director_name (second column)
	and so on for all of the fields in the csv.
	
	Most of the conversions for the numeric types are in double because the mergesort sorts
	based on doubles, not ints
	This was mainly due to the column "gross" being so large that an int cannot fit it. 
*/


static void allocateToken(Record* ptrrecords,char* field,int index)
{
	size_t length;
	double grossConv;
	
	switch(index){
		
					
		case 0:
			length = strlen(field);
			strncpy(ptrrecords->color,field,length);
			break;
			
		case 1:
			length = strlen(field);
			strncpy(ptrrecords->director_name,field,length);
			break;
			
		case 2:
			ptrrecords->num_critic_for_reviews = atof(field);
			break;
		case 3:
			ptrrecords->duration = atof(field);
			break;
		case 4:
			ptrrecords->director_facebook_likes = atof(field);
			break;
		case 5:
			ptrrecords->actor_3_facebook_likes = atof(field);
			break;
			
		case 6: 
			length = strlen(field);
			strncpy(ptrrecords->actor_2_name,field,length);
			break;		
			
		case 7:	
			ptrrecords->actor_1_facebook_likes = atof(field);
			break;
		case 8:
			grossConv = atof(field);
			ptrrecords->gross = grossConv;
			break;
			
			
		case 9: 
			length = strlen(field);
			strncpy(ptrrecords->genres,field,length);
			break;
		
		case 10: 
			length = strlen(field);
			strncpy(ptrrecords->actor_1_name,field,length);
			break;
			
		case 11:
			length = strlen(field);
			strncpy(ptrrecords->movie_title,field,length);
			
			break;
			
		case 12:
			ptrrecords->num_voted_users = atof(field);
			break;
		case 13:
			ptrrecords->cast_total_facebook_likes = atof(field);
			break;
		case 14: 
			length = strlen(field);
			strncpy(ptrrecords->actor_3_name,field,length);
			break;
		case 15:
			ptrrecords->facenumber_in_poster = atof(field);
			break;
		case 16:
			length = strlen(field);
			strncpy(ptrrecords->plot_keywords,field,length);
			
			break;

		case 17:
			length = strlen(field);
			strncpy(ptrrecords->movie_imdb_link, field,length);
			
			break;
		case 18:
			ptrrecords->num_user_for_reviews = atof(field);
			break;
		case 19:
			length = strlen(field);
			strncpy(ptrrecords->language,field,length);
			break;


		case 20:
			length = strlen(field);
			strncpy(ptrrecords->country,field,length);
			break;
			
		case 21:
			length = strlen(field);
			strncpy(ptrrecords->content_rating,field,length);
			
			break;
		case 22:
			ptrrecords->budget = atof(field);
			break;
		case 23:
			ptrrecords->title_year = atof(field);
			break;
		case 24:
			ptrrecords->actor_2_facebook_likes = atof(field);
			break;
			
		case 25:
			ptrrecords->imdb_score = atof(field);
			break;
		case 26:
			ptrrecords->aspect_ratio = atof(field);
			break;
		case 27:
			ptrrecords->movie_facebook_likes = atof(field);
			break;
		default:
			printf("Did not allocate token correctly, ending");
			exit(0);
			break;
	}
						
						
}

static void printStructs(Record list[], int numStructs){
	
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

//if the type is a string, the use the string sort
//flag is parameter whether or not to print the output
static void sort (char* sortType, int numStructs, Record* allrecords)
{
	if ((strcmp(sortType,"color") == 0)||(strcmp(sortType,"director_name")== 0)||(strcmp(sortType,"actor_name_2")== 0)||(strcmp(sortType,"genres")== 0)||(strcmp(sortType,"actor_1_name")== 0)||(strcmp(sortType,"movie_title")== 0)||(strcmp(sortType,"actor_3_name")== 0)||(strcmp(sortType,"plot_keywords")== 0)||(strcmp(sortType,"movie_imdb_link")== 0)||(strcmp(sortType,"language")== 0)||(strcmp(sortType,"country")== 0)||(strcmp(sortType,"content_rating")== 0))
	  {
	    sortString(allrecords, 0, numStructs - 1,sortType);

	  }
	//Otherwise, use the numsort		
    else
	{
	   sortNum(allrecords, 0, numStructs - 1,sortType);
    }

  
	
}//End mergesort function

