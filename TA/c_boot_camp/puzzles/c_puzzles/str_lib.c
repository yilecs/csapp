#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* For all these problems, you may not use any 
 * string library functions; you may, however, 
 * define any helper functions that you may need */ 

/* Concatenates the src string to the end of the dest
 * string, returns the dest string
 *
 * You can assume that src/dest are '\0' terminated, 
 * that dest hold enough space to store src+dest and 
 * src/dest do not overlap in memory */
char* strcat_m(char* dest, char* src) { 
    /* TODO: Implement this function */
    char* d = dest;
    while(*d != '\0') {
        d++;
    }
    while((*(d++) = *(src++)) != '\0') {
        ;
    }
    return dest; 
}

/* Reverses a string and returns the a new string 
 * containing the reversed string 
 *
 * You can assume that str is '\0' terminated */
char* strrev(char *str) {
    /* TODO: Implement this function */
    size_t len = strlen(str);
    char* dest = (char *) malloc(len+1);

    for (size_t i=0; i<len; i++) {
        dest[i] = str[len-i-1];
    }
    dest[len] = '\0';
    return dest; 
}

int main()
{
    /* TODO: Implement test cases to check your implemenation
     * You MAY use string library functions to test your code */
    char* dest = (char*) malloc(sizeof(char) * 20);
    strcpy(dest, "hello");
    char src[] = "word";

    dest = strcat_m(dest, src);

    char* rev = strrev(dest);

    printf("%s\n", dest);
    printf("%s\n", rev);

    free(rev);

    return 0;
}
