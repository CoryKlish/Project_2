/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/


//Suggestion: define a struct that mirrors a record (row) of the data set

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
char VerifyMode(char mode);
void allocateToken(Record*, char*, int);
struct Record* evalArray(Record*,Record*,int ,int);

//Large helper function
/*
	Allocate tokens to their fields based on the index of the loop. 
	First index goes to color (first column), second index goes to director_name (second column)
	and so on for all of the fields in the csv.
	
	Most of the conversions for the numeric types are in double because the mergesort sorts
	based on doubles, not ints
	This was mainly due to the column "gross" being so large that an int cannot fit it. 
*/
void allocateToken(Record* ptrrecords, char* field, int index)
{
	int length;
	double grossConv;
	
	switch(index){
		
					
		case 0:

			strcpy(ptrrecords->color,field);
			break;
			
		case 1:

			strcpy(ptrrecords->director_name,field);
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

			strcpy(ptrrecords->actor_2_name,field);
			break;		
			
		case 7:	
			ptrrecords->actor_1_facebook_likes = atof(field);
			break;
		case 8:
			grossConv = atof(field);
			ptrrecords->gross = grossConv;
			break;
			
			
		case 9: 

			strcpy(ptrrecords->genres,field);
			break;
		
		case 10: 

			strcpy(ptrrecords->actor_1_name,field);
			break;
			
		case 11:

			strcpy(ptrrecords->movie_title,field);
			
			break;
			
		case 12:
			ptrrecords->num_voted_users = atof(field);
			break;
		case 13:
			ptrrecords->cast_total_facebook_likes = atof(field);
			break;
		case 14: 

			strcpy(ptrrecords->actor_3_name,field);
			break;
		case 15:
			ptrrecords->facenumber_in_poster = atof(field);
			break;
		case 16:

			strcpy(ptrrecords->plot_keywords,field);
			
			break;

		case 17:

			strcpy(ptrrecords->movie_imdb_link, field);
			
			break;
		case 18:
			ptrrecords->num_user_for_reviews = atof(field);
			break;
		case 19:

			strcpy(ptrrecords->language,field);
			break;


		case 20:

			strcpy(ptrrecords->country,field);
			break;
			
		case 21:

			strcpy(ptrrecords->content_rating,field);
			
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

