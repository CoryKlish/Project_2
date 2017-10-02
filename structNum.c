#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"


/*
// **********THINGS TO FIX***********
			LINES 316 & 349
*/

//Cory Klish
//Joshua Pineda

void mergeNum(Record list[], int left, int mid, int right){
	
    int LSize = mid - left + 1;
    int RSize =  right - mid;
 
	/*
    //Temp array dynamic allocation
    Record LArr[LSize], RArr[RSize];
	*/
	
	Record* LArr = malloc(sizeof(Record) * LSize);
	Record* RArr = malloc(sizeof(Record) * RSize);
	
	int l; //Left index counter
	int r; //Right index counter
	int m; //Merged index
 
    for (l = 0; l < LSize; l++){
        LArr[l] = list[left + l];
	}
	
    for (r = 0; r < RSize; r++){
        RArr[r] = list[mid + 1 + r];
	}
 
	//Merge subarrays
    l = 0; 
    r = 0; 
    m = left; 
    while (l < LSize && r < RSize){
		
		if(strcmp(sortType, "num_critic_for_reviews") == 0){
			
			if (LArr[l].num_critic_for_reviews <= RArr[r].num_critic_for_reviews){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "duration") == 0){
			
			if (LArr[l].duration <= RArr[r].duration){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "director_facebook_likes") == 0){
			
			if (LArr[l].director_facebook_likes <= RArr[r].director_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "actor_3_facebook_likes") == 0){
			
			if (LArr[l].actor_3_facebook_likes <= RArr[r].actor_3_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "actor_1_facebook_likes") == 0){
			
			if (LArr[l].actor_1_facebook_likes <= RArr[r].actor_1_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "gross") == 0){
			
			if (LArr[l].gross <= RArr[r].gross){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "num_voted_users") == 0){
			
			if (LArr[l].num_voted_users <= RArr[r].num_voted_users){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "cast_total_facebook_likes") == 0){
			
			if (LArr[l].cast_total_facebook_likes <= RArr[r].cast_total_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "facenumber_in_poster") == 0){
			
			if (LArr[l].facenumber_in_poster <= RArr[r].facenumber_in_poster){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "num_user_for_reviews") == 0){
			
			if (LArr[l].num_user_for_reviews <= RArr[r].num_user_for_reviews){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "budget") == 0){
			
			if (LArr[l].budget <= RArr[r].budget){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "title_year") == 0){
			
			if (LArr[l].title_year <= RArr[r].title_year){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "actor_2_facebook_likes") == 0){
			
			if (LArr[l].actor_2_facebook_likes <= RArr[r].actor_2_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "imdb_score") == 0){
			
			if (LArr[l].imdb_score <= RArr[r].imdb_score){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "aspect_ratio") == 0){
			
			if (LArr[l].aspect_ratio <= RArr[r].aspect_ratio){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}else if(strcmp(sortType, "movie_facebook_likes") == 0){
			
			if (LArr[l].movie_facebook_likes <= RArr[r].movie_facebook_likes){
			
				list[m] = LArr[l];
				l++;
			}
			else{
			
				list[m] = RArr[r];
				r++;
			}
			m++;
			
		}
			
	}
 
    while (l < LSize){
		
        list[m] = LArr[l];
        l++;
        m++;
    }
 
    while (r < RSize){
		
        list[m] = RArr[r];
        r++;
        m++;
    }
	
	free(LArr);
	LArr = NULL;
	free(RArr);
	RArr = NULL;
}
 
void sortNum(Record list[], int left, int right)
{
    if (left < right){
		
        int mid = left + (right - left) / 2;
 
        sortNum(list, left, mid);
        sortNum(list, mid + 1, right);
 
        mergeNum(list, left, mid, right);
    }
}

 
