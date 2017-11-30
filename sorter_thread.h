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
static pthread_mutex_t safetylock = PTHREAD_MUTEX_INITIALIZER;
static char* header = NULL;

//=========Thread Id Section=========
static pthread_t* tidArray;
static int arrSize = 50;
static int threadCounter = 0;
static int threadIndex = 0;
static int runningThreads = 0;
static int initTID;
static int flag;
static pthread_cond_t cv;

//==========Table/Arrays are accumulated here=========
static Record* bigKahuna;
static int kahunaIndexCount = 0;
static int kahunaSize = 0;


//===========List of tables/arrays that go inside bigkahuna=====
static Record** kahunaComp;
static Record** kahunaCompPtr;
static int kahunaCompIndex = 0;
static int kahunaCompSize = 256;

//========Stores the number of records in each array in Kahunacomp=====
static int* tableSizes;
static int tableSizesLength = 256;
static int tableSizeIndex = 0;
static int* tablesizeptr;


//==========Array of structs: Each struct goes to a thread/call to processdir or getfile=======
static ReadParams** rparray;
static int rpindex = 0;
static int rpsize = 256;

//===============Prototypes===============

//===================SORTER.H=================
static int processDirectory(char* path, char* inputCol, char* outpath);
static void *processDir(void* params);
static void *getFile(void* params);
static void allocateToken(Record*, char*, int);
static  char* getSortType(char* header,char* colName, int* numFields);
static void sort (char* sortType, int numStructs, Record*);
static void printStructs(Record list[], int numStructs);
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
Record* copyTable(Record* dest, Record* src);
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



static void kahunaCopy(Record list[], int numRecords)
{
	int i = 0;
	
	while(i != numRecords)
	{
		//bigKahuna[kahunaIndexCount] = list[i];
		memcpy(&bigKahuna[kahunaIndexCount], &list[i], sizeof(list[i]));
		kahunaIndexCount++;
		i++;
	}
	free(list);
	
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
