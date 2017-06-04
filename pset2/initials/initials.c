/**
 * Given a person’s name, prints a person’s initials. Does not accept characters other than alphabetical and whitespaces.
 */

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void){
    printf("Your name: ");
    string name = get_string();     // Get string as an input
    string word = strtok(name, " ");
    
    while (word != NULL)
    {
        printf("%c", toupper(word[0]));
        word  = strtok(NULL, " ");
    }
    
    printf("\n");
    return 0;
}
