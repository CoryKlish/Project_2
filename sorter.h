/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

#include <sys/stat.h>
#include <sys/types.h>

static int processCounter = 1;
static int initpid;

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
//Prototypes


//in sorter.h
static void allocateToken(Record*, char*, int);
static  char* getSortType(char* header,char* colName, int* numFields);
static void sort (char* sortType, int numStructs, Record*);
static void printStructs(Record list[], int numStructs);
static int processDirectory( char* path, char* inputCol, char* outpath,int flag);
static void processFile(char* fileName,char* inputCol, char* path, char* outpath);
static Record * readFile(char *fileName, int *pNumRecords, int numFields, char* inputCol,char** pHeader, char* inpath);
static void writeFile(Record list[] ,char *fileName, int numRecords, char *outDir,char* sortType,char* header);


//In SORTER.C
int VerifyDirectory(char* path);
char VerifyMode(char* mode);

//IN mergesort.c
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
    char* field = strtok(header,",");
    
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
		field = strtok(NULL, ",");
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
    
    if (*numFields != 27)
    {
        printf("\nWrong number of columns in csv.\n");
        exit(0);
    }

    return sortType;

}//end getSortType function
/*
PARAMS:
path is a char* that will be opened using opendir
inputCol is what we are sorting on, which is validated in this
    method
*/
static int processDirectory(char* path, char* inputCol, char* outpath, int flag)
{
   
    struct dirent* entry;
    char* csv = ".csv";
    int len = strlen(path);
    int status = 0;
    
    DIR* directory  = opendir(path);
    //read from directory until nothing left
    
    while ((entry = readdir(directory)) != NULL )
    {
		if ((strcmp (entry->d_name,"."))!= 0 && (strcmp (entry->d_name,"..")) != 0 && (strcmp (entry->d_name,".git")) != 0)
		{

			struct stat buffer;
			char dpath[255];
			dpath[0] = '\0';
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
			//if the path is not a "." or "./"
			else
			{
				//Original path either ends with "/" or not
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

		   //if the entry is another directory
		if (entry->d_type == DT_DIR)
			{
				int len = strlen(path);				
				fflush(stdout);
				int pT = fork();
				
				//in the child process, process the directory 
				if (pT == 0)
				{
                    processCounter = 1;
                    
					printf("%d, " , getpid());
				    processDirectory(dpath,inputCol,outpath,0);
				}
				//If we are the parent process,
				else if (pT > 0)
				{
					
                    //nothing goes on
				}
				else
				{
					printf("fork() failed.");
					exit(0); 
				}
					
					
			}//end if directory


			if (entry->d_type == DT_REG)//if entry = regular file
			{
				//pointer to the filename
				char* fileName = (entry->d_name);
	  
					 //create index that points to the 
					char* fileext = strstr(fileName, csv);
					if (fileext != NULL)
					{
					  //  printf("\ncsv recognized: %s\n",fileName);
						  /* fork() to process the file*/
					
						int numRecords = 0;
						int* pNumRecords = &numRecords;
						
						char* sorted = strstr(fileName,"-sorted-");
						if (sorted != NULL)
						{
						   // printf("ignored\n");
						   continue;
						}
						
						//If it is not already a sorted file
						else
						{
							processFile(fileName,inputCol,path, outpath);
                            
						}
						

					}
					//if file ext is not csv, do nothing
			 
			}//end if regular file
			
        }//end if
    }//end whileloop for readdir
	
	
	//////////////////////////////////////////////////////////BASE CASE NEEDS WORK MAYBE?/////////////////////////////////////
	if((entry = readdir(directory)) == NULL)
	{
		while(1)
	   {		
			if( (wait(&status)) > -1 )
			{
                if (WIFEXITED(status))
                {
                    
                    processCounter+= WEXITSTATUS(status);
                }
              
			}
			else
			{
				break;
			}
	   }
	}
	
	
    if ((getpid() == initpid))
    {
         return processCounter;

    }
    else
    {
        exit (processCounter);
    }

	
	
}//End processDirectory function

static void processFile(char* fileName,char* inputCol, char* path, char* outpath)
{
    int status;
    //need the numrecords for the mergesort
    int numRecords = 0;
    int* pNumRecords = &numRecords;
    //readfile validates the input column and creates a record array
    //char** pheader will change the value of 
    //char* header from within readFile
    char* header;
    char** pHeader = &header;
    fflush(stdout);
    int pT = fork();
    //in the child process

    if (pT == 0)
    {
        Record * table = readFile(fileName, pNumRecords, 0, inputCol, pHeader,path);
        sort(inputCol, numRecords,table);
        writeFile(table,fileName,numRecords,outpath,inputCol,header);
        printf("%d, ",getpid());
        processCounter = 1;

       exit(processCounter); 
    }
  

   
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
static Record * readFile(char *fileName, int *pNumRecords, int numFields, char* inputCol, char** pHeader, char* inpath){ 

    //open the file for reading
	FILE *fp;
    if (strcmp(inpath,".") == 0 || (strcmp(inpath,"./") == 0))
        fp = fopen(fileName, "r");
    else
    { 
        int plen = strlen(inpath);
        int flen = strlen(fileName);
        
        char* pathtofile = (char*)malloc(sizeof(char) * (plen + flen) + 2);
        pathtofile[0] = '\0';
        strcat(pathtofile,inpath);
        strcat(pathtofile,"/");
        strcat(pathtofile,fileName);
        

        fp = fopen(pathtofile,"r");
        
    }
    
    

    //validation if real
	if(fp == NULL)
    {
		printf("Error: Input directory does not exist\n");
		exit(0);
	}
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
 
    if (*numP != 27)
    {
        printf("not the correct number of files. exiting. \n");
        exit(0);
    }
    Record* newRecords = createTable(pNumRecords, numFields, fp);
	fclose(fp);
    return newRecords;
}
//takes in the record set as "list"
static void writeFile(Record list[] ,char *fileName, int numRecords, char *outDir,char* sortType,char* header){
	
	FILE *fp;
	char *fileWrite;
    int len = strlen(fileName);
    char* newFileName = (char*)calloc(len,sizeof(char));
    newFileName = strncpy(newFileName,fileName,len-4);
    
	fileWrite = (char *)malloc(strlen(newFileName) + strlen(sortType)+ 9);
	//+9 for "-sorted-" (8) and null terminating 0 (1)
	
	if(fileWrite == NULL){
		printf("Not enough memory...exiting\n");
		exit(0);
	}
	/// Section where the sorted file name is created
	fileWrite[0] = '\0';
	
	strcat(fileWrite, newFileName);//Append new fileName to empty string
	strcat(fileWrite, "-sorted-");//Append "-sorted-" to end
	strcat(fileWrite, sortType);//Append the  variable "sortType"
    strcat(fileWrite,".csv");
	/// end section
    
    ///Section where the space for the filename and output 
    ///directory is allocated
        //string that will hold the directory + filename
        char *placeToWrite = (char*)malloc(strlen(fileWrite)+strlen(outDir)+2);
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
	for(i = 0; i < numRecords; i++){
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
	free(fileWrite);
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
