/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/


//Suggestion: define a struct that mirrors a record (row) of the data set

typedef struct Record{
	char* color;
	char* director_name;
	double num_critic_for_reviews;
	double duration;
	double director_facebook_likes;
	double actor_3_facebook_likes;
	char* actor_2_name;
	double actor_1_facebook_likes;
	double gross;
	char* genres;
	char* actor_1_name;
	char* movie_title;
	double num_voted_users;
	double cast_total_facebook_likes;
	char* actor_3_name;
	double facenumber_in_poster;
	char* plot_keywords;
	char* movie_imdb_link;
	double num_user_for_reviews;
	char* language;
	char* country;
	char* content_rating;
	double budget;
	double title_year;
	double actor_2_facebook_likes;
	double imdb_score;
	double aspect_ratio;
	double movie_facebook_likes;
	
	
} Record;

char VerifyMode(char mode);
void allocateToken(Record*, char*, int);
struct Record* evalArray(Record*,int ,int);


//Suggestion: prototype a mergesort function

