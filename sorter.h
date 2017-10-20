/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/



//extern char* sortType;

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
static char* getHeader(char* header,char* colName, int* numFields);
char VerifyMode(char mode);
void mergeNum(Record list[], int left, int mid, int right);
void sortNum(Record list[], int left, int right);
void printStructs(Record list[], int numStructs);
void sortString(Record strArr[], int lo, int hi);
void mergeString(Record strArr[], int lo, int mid, int hi);


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

char* static getHeader(char* header, char* colName, int* numFields)
{
    //If colName matches with a field, colName is copied into
    //sortType
    char* sortType;
    //put ptr on first char of line
    char* field = strtok(header,",");
    if (field == NULL)
	{
		printf("ERROR, no fields");
        return NULL;
	}
    //Just in case first column is the column to be sorted
	if (strcmp(field, colName) == 0)
	{
		//Get length, allocate size and copy into 'column' variable
		len = strlen(field);
		sortType = (char*)malloc(sizeof(char) * len);
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
			*numFields += 1;
			
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
    
    return sortType;

}//end getHeader function

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


//Suggestion: prototype a mergesort function

