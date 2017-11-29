#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "sorter_thread.h"
#include <sys/mman.h>
#include <fcntl.h>

/////////////////////////////////////////////////////////////Processing Directory method/////////////////////////////////////////////


    
    
/*
Takes records from the csv one at a time from STDIN
and places the fields under the correct header.
*/
Record* createTable(int* pNumRecords, int numFields, FILE *fp)
{
//////////////////Placing records into structs -> structs into an array//////////////////////////////////////////////
	//holds initial 5000 records
	struct Record * allrecords = 
        (Record *)malloc(sizeof(Record) * 5000);
	//size of the records array in bytes
	size_t arSize = 5000* (sizeof(Record));
	//total bytes that accumulates after each getline
	int totalbytes = 0;
    char* field;
    char* line = NULL;
    size_t recordsize;
    size_t bytes;

	
	//ptr for indexing struct
	struct Record * ptrrecords = allrecords;


	//jumpstart the loop
	if(fp == NULL){
        bytes = getline(&line, &recordsize, stdin);	
	}
	else
	{
		bytes = getline(&line, &recordsize, fp);
	}
	
	while (bytes != -1)
	{
		
		//copy to row to free up the line var
		char* row = malloc(sizeof(char) * strlen(line)); 
        row = strdup(line);
		line = NULL;

		if (bytes != -1)
		{

			//increase count of records
			*pNumRecords += 1;
			//Add to total amount of bytes
			totalbytes += sizeof(Record);

//////////////////////////////////mem realloc if go over		
			//Check if total bytes goes over
			if(totalbytes > arSize)
			{
				//Add 5000 to the number of input records
				arSize = arSize + (5000 * sizeof(Record));

				//reallocate, move pointer to new memory location with more mem
				allrecords = (Record*)realloc(allrecords, arSize);

				//If this does not work, there is no more memory left to allocate
                ptrrecords = allrecords + (*pNumRecords - 1);
				if ( ptrrecords== NULL)
				{
					printf("Out of memory, exiting");
					exit(0);
				}

            }
////////////////////////////////////end realloc section
			
			//checks for a double quote in the row, which indicates there will be nested commas
			char * check = strstr(row,"\"");
			//If double quotes are present,
			char * qchecker;
			int i,commacounter = 0;			
	
			//get tokens in the line
			for(i = 0; i < numFields + 1;i++)
			{	

				//get a field
				field = strsep(&row,",");
				//If there is a quote in this line
				if (check != NULL)
				{
					//set qchecker to field to check for quote
					qchecker = check;
					//If there is a quote in the beginning of the field, this is string with " we checked for earlier 
					//then we can replace field with the 'special' var that contains contains the field
					//within the double quotes.
                    int fieldlen = strlen(field);
                    
					if (*(field) == '"')
					{
						//create new char array
						char* special = (char*)calloc(strlen(qchecker),sizeof(char));
						// move the ptr to the next char after the initial "
						qchecker++;
					
						//Read everything but the "
						//while (*(qchecker) != '"')
						while (*(qchecker) != '\"')	
						{
							//If the character is a comma, be sure to move the token ptr along with it
							//mainly so that the tokenizer keeps up with the correct field
							//rather than considering the other nested commas to be other fields
							if (*(qchecker) == ',')
							{
								commacounter += 1;
							}
							
							//add to the special str and move ptr
							special = strncat(special,qchecker,1);
							qchecker++;							
						}

                    //based on the number of commas in the 
                    //field, we skip over that many nested
                        //commas + 1 to get over the commas
                        //after the field
                        
                     while(commacounter + 1 != 0)
                    {
                        field = strsep(&row,",");
                        commacounter -=1;
                    }
                    *(special + (fieldlen-1)) = ',';
	
					//duplicate special str into field
					field = strdup(special);

					*(special + strlen(special - 1)) = '\0';
				    
					}
				}		
				
				//Based on the index, it allocates token to that field in the struct.
				allocateToken(ptrrecords, field, i);
               
			}//end token loop
		}//end if bytes != -1
		
	//get next line, move pointer of records over
		if(fp == NULL){
			bytes = getline(&line, &recordsize, stdin);
		}
		else
		{
			bytes = getline(&line, &recordsize, fp);
		}
		if (bytes != -1)
			ptrrecords++;
 
	}//end while
    return allrecords;
}//End createTable function


///////////////////////////////////////////////////////STRING SORT////////////////////////////////////////////////////////////////////////////

void mergeString(Record strArr[], int lo, int mid, int hi,char* sortType){//Merge arrays back together
	
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
			
			if(strcasecmp(LArr[i].color, RArr[x].color) == 0){
				if(strcmp(LArr[i].color, RArr[x].color) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].color, RArr[x].color) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "director_name") == 0){
			
			if(strcasecmp(LArr[i].director_name, RArr[x].director_name) == 0){
				if(strcmp(LArr[i].director_name, RArr[x].director_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].director_name, RArr[x].director_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "actor_2_name") == 0){
			
			if(strcasecmp(LArr[i].actor_2_name, RArr[x].actor_2_name) == 0){
				if(strcmp(LArr[i].actor_2_name, RArr[x].actor_2_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].actor_2_name, RArr[x].actor_2_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "genres") == 0){
			
			if(strcasecmp(LArr[i].genres, RArr[x].genres) == 0){
				if(strcmp(LArr[i].genres, RArr[x].genres) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].genres, RArr[x].genres) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "actor_1_name") == 0){
			
			if(strcasecmp(LArr[i].actor_1_name, RArr[x].actor_1_name) == 0){
				if(strcmp(LArr[i].actor_1_name, RArr[x].actor_1_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].actor_1_name, RArr[x].actor_1_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "movie_title") == 0){
			
			if(strcasecmp(LArr[i].movie_title, RArr[x].movie_title) == 0){
				if(strcmp(LArr[i].movie_title, RArr[x].movie_title) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].movie_title, RArr[x].movie_title) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "actor_3_name") == 0){
			
			if(strcasecmp(LArr[i].actor_3_name, RArr[x].actor_3_name) == 0){
				if(strcmp(LArr[i].actor_3_name, RArr[x].actor_3_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].actor_3_name, RArr[x].actor_3_name) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "plot_keywords") == 0){
			
			if(strcasecmp(LArr[i].plot_keywords, RArr[x].plot_keywords) == 0){
				if(strcmp(LArr[i].plot_keywords, RArr[x].plot_keywords) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].plot_keywords, RArr[x].plot_keywords) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "movie_imdb_link") == 0){
			
			if(strcasecmp(LArr[i].movie_imdb_link, RArr[x].movie_imdb_link) == 0){
				if(strcmp(LArr[i].movie_imdb_link, RArr[x].movie_imdb_link) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].movie_imdb_link, RArr[x].movie_imdb_link) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "language") == 0){
			
			if(strcasecmp(LArr[i].language, RArr[x].language) == 0){
				if(strcmp(LArr[i].language, RArr[x].language) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].language, RArr[x].language) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "country") == 0){
			
			if(strcasecmp(LArr[i].country, RArr[x].country) == 0){
				if(strcmp(LArr[i].country, RArr[x].country) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].country, RArr[x].country) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			
		}else if(strcmp(sortType, "content_rating") == 0){
			
			if(strcasecmp(LArr[i].content_rating, RArr[x].content_rating) == 0){
				if(strcmp(LArr[i].content_rating, RArr[x].content_rating) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
			}
			else{
				if(strcasecmp(LArr[i].content_rating, RArr[x].content_rating) < 0){
					strArr[y++] = LArr[i++];
				}
				else{
					strArr[y++] = RArr[x++];
				}
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

void sortString(Record strArr[], int lo, int hi,char* sortType){//Recursive divide and conquer sort
	
	if(lo < hi){
		int mid = (lo + hi) / 2;//Find midpoint to split at
		sortString(strArr, lo, mid,sortType);//Pass in left side to sort
		sortString(strArr, mid + 1, hi,sortType);//Pass in right side to sort
		mergeString(strArr, lo, mid, hi,sortType);
	}
	
}


//////////////////////////////////////////////////////////////NUM SORT//////////////////////////////////////////////////////////////////////////

void mergeNum(Record list[], int left, int mid, int right,char* sortType){
	
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
 
void sortNum(Record list[], int left, int right,char* sortType)
{
    if (left < right){
		
        int mid = left + (right - left) / 2;
 
        sortNum(list, left, mid,sortType);
        sortNum(list, mid + 1, right,sortType);
 
        mergeNum(list, left, mid, right,sortType);
    }
}

