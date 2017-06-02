// The program prints a person's initials, given their name as an input.

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void){
    string name = get_string(); // Get string as an input
    if (name[0] != ' '){        // Print the first letter if it's not a whitespace
        printf ("%c", name[0]);
    }
    
    // Iterate through a string and print the first letter that appears after a series of whitespaces
    for (int i = 0; i < strlen(name); i++){
        if (name[i] == ' '){
            if (name[i+1] != ' '){
                printf ("%c", name[i+1]);
            }
        }
    }
    printf ("\n"); // New line
}