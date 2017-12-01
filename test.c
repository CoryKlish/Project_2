#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char string[] = "This, is , a , string.";
    char *field = strtok(string,",");
    printf("%u, %u\n",string, field);

}