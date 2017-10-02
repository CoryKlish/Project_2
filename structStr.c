#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"




void mergeString(Record strArr[], int lo, int mid, int hi){//Merge arrays back together
	
	int left = mid - lo + 1;
	int right = hi - mid;
	
	
	//Dynamic mem allocation
	Record* LArr = malloc(sizeof(Record) * left);//Allocate left array
	Record* RArr = malloc(sizeof(Record) * right);//Allocate right array
	
	
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
