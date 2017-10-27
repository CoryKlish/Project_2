/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/




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



static void allocateToken(Record*, char*, int);
static  char* getSortType(char* header,char* colName, int* numFields);
static void sort (char* sortType, int numStructs, Record*, int);
static void printStructs(Record list[], int numStructs);
DIR* getDirectory(char* path);
DIR* processDirectory( DIR* directory, char* inputCol);
char VerifyMode(char* mode);
Record* createTable(int* pNumRecords,int numFields, FILE *fp);
void mergeNum(Record list[], int left, int mid, int right,char* sortType);
void sortNum(Record list[], int left, int right,char* sortType);
void sortString(Record strArr[], int lo, int hi,char* sortType);
void mergeString(Record strArr[], int lo, int mid, int hi,char* sortType);



//Large helper function: getHeader
/*
Params:
header is the very first row of the csv, which is attained         outside this method
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
    char* sortType;
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
*/
Record * readFile(char *fileName, int *pNumRecords, int numFields, char* inputCol){ 

    //open the file for reading
	FILE *fp;
	fp = fopen(fileName, "r");

    //validation if real
	if(fp == NULL){
		printf("Error: File does not exist\n");
		exit(0);
	}
    //taking the header
    size_t recordsize;
    char* line = NULL;
    //get a line from the file
    size_t bytes = getline(&line, &recordsize, fp);
    int len = strlen(line);
    //pointer to numfields in order to change its value
    int* numP = &numFields;
    //getting the sortType
    char* sortType = (char*)malloc(sizeof(char) * len);
    //getSortType also gets the number of fields
    sortType = getSortType(line,inputCol,numP);
    if (sortType != inputCol || numFields != 27)
    {
        printf("csv file is not in correct format");
        exit(0);
    }
    Record* newRecords = createTable(pNumRecords, numFields, fp);
	fclose(fp);
    return newRecords;
}
//takes in the record set as "list"
void writeFile(Record list[] ,char *fileName, int numRecords, char *outDir,char* sortType){
	
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
		char *placeToWrite = (char*)malloc(strlen(fileWrite)+strlen(outDir)+2);
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
    int i;
	for(i = 0; i < numRecords; i++){
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
static void sort (char* sortType, int numStructs, Record* allrecords, int flag)
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

  
	if (flag)
        printStructs(allrecords, numStructs);
}//End mergesort function
