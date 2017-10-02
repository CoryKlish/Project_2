#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/*
//For testing
typedef struct
{
	int gross;
	int rating;
	char* movie_title;
}Record;
*/

char* sortType;

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

void mergeString(Record strArr[], int lo, int mid, int hi){//Merge arrays back together
	
	int left = mid - lo + 1;
	int right = hi - mid;
	
	
	//Dynamic mem allocation
	Record* LArr = malloc(sizeof(Record) * left);//Allocate left array
	Record* RArr = malloc(sizeof(Record) * right);//Allocate right array
	
	/*
	//For Testing
	Record LArr[left];
	Record RArr[right];
	*/
	int i = 0;
	
	for(i = 0; i < left; i++){
		//LArr[i] = (char*)malloc(sizeof(strArr[lo + i]));
		LArr[i] = strArr[lo + i];
	}
	
	for(i = 0; i < right; i++){
		//RArr[i] = (char*)malloc(sizeof(strArr[mid + 1 + i]));
		RArr[i] = strArr[mid + 1 + i];
	}
	
	int x = 0;//Initial Index of first subarray
	int y = 0;//Initial Index of second subarray
	i = 0;
	y = lo;//Initial Index of merged subarray
	
	while(i < left && x < right){
		
		if(strcmp(sortType, "color") == 0){
			
			if(strcmp(LArr[i].color, RArr[x].color) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "director_name") == 0){
			
			if(strcmp(LArr[i].director_name, RArr[x].director_name) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "actor_2_name") == 0){
			
			if(strcmp(LArr[i].actor_2_name, RArr[x].actor_2_name) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "genres") == 0){
			
			if(strcmp(LArr[i].genres, RArr[x].genres) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "actor_1_name") == 0){
			
			if(strcmp(LArr[i].actor_1_name, RArr[x].actor_1_name) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "movie_title") == 0){
			
			if(strcmp(LArr[i].movie_title, RArr[x].movie_title) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "actor_3_name") == 0){
			
			if(strcmp(LArr[i].actor_3_name, RArr[x].actor_3_name) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "plot_keywords") == 0){
			
			if(strcmp(LArr[i].plot_keywords, RArr[x].plot_keywords) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "movie_imdb_link") == 0){
			
			if(strcmp(LArr[i].movie_imdb_link, RArr[x].movie_imdb_link) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "language") == 0){
			
			if(strcmp(LArr[i].language, RArr[x].language) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "country") == 0){
			
			if(strcmp(LArr[i].country, RArr[x].country) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}else if(strcmp(sortType, "content_rating") == 0){
			
			if(strcmp(LArr[i].content_rating, RArr[x].content_rating) < 0){
				strArr[y++] = LArr[i++];
			}
			else{
				strArr[y++] = RArr[x++];
			}
			
		}
		
	}
	
	while(i < left){
		strArr[y++] = LArr[i++];
	}
	
	while(x < right){
		strArr[y++] = RArr[x++];
	}
	
	free(LArr);
	LArr = NULL;
	free(RArr);
	RArr = NULL;
}

void sortString(Record strArr[], int lo, int hi){//Recursive divide and conquer sort
	
	if(lo < hi){
		int mid = (lo + hi) / 2;//Find midpoint to split at
		sortString(strArr, lo, mid);//Pass in left side to sort
		sortString(strArr, mid + 1, hi);//Pass in right side to sort
		mergeString(strArr, lo, mid, hi);
	}
	
}

void printArray(Record list[], int numStructs)
{
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
	
    /*
	//For testing
	for (i=0; i < size; i++){
        printf("%d %d %s\n", A[i].gross, A[i].rating, A[i].movie_title);
		printf("\n");
	}
	*/
}

int main(){
	
	Record arr[3];
    
	/*
	//For testing
	arr[0].gross = 200;
	arr[0].rating = 5;
	arr[0].movie_title = "The Dark Knight";
	arr[1].gross = 100;
	arr[1].rating = 4;
	arr[1].movie_title = "It";
	arr[2].gross = 150;
	arr[2].rating = 3;
	arr[2].movie_title = "Transformers";
	*/
	
    int numStructs = 3;
	
    printf("Beginning Array is: \n");
	printArray(arr, numStructs);
 
    sortString(arr, 0, numStructs - 1);
 
    printf("\nSorted array is \n");
    printArray(arr, numStructs);
    return 0;
	
	//////////////////////////////////////////////////////
	
	/*
	int i = 0;
	
	printf("\nEnter the size of the array desired: ");
    int sizeStr;  //This is the String array size
    scanf("%d", &sizeStr);

    Record** testStrArr = (char**)malloc(sizeof(char *)* sizeStr); //Creating required string array
	
   for(i = 0; i < sizeStr; i++)
    {
        testStrArr[i] = malloc(sizeof(char)* MAX);
        printf("\nEnter String: ");
        fgets(testStrArr[i], 100, stdin);
    }
	
    sortString(testStrArr, 0, sizeStr-1);
    printf("\nThe Sorted Array is: ");
	
	for(i = 0; i < sizeStr; i++){
	   printf("%s -> ",testStrArr[i]);
   }
   
    return 0;
	*/
}