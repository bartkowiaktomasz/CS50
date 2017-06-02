// The program encrypts messages using Caesar’s cipher.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // Allows for isalpha
#include <cs50.h>
#include <string.h>

int main(int argc, string argv[]){
    
    int key = atoi(argv[1]);
    if (argc != 2 || key <= 0){     // If user does not prompt two arguments in the CLI - return error
        printf("Error, only one argument allowed\n");
        return 1;
    }
    
    printf("plaintext: ");
    string plaintext = get_string();    // Get plaintext from the user
    string ciphertext = plaintext;      // Initialize ciphertext
    
    /*
    Iterate through the string, shift letters by "key",
    distinguishing between lower- and uppercase
    */
    
    for (int i = 0; i < strlen(plaintext); i++){    
        if (isalpha(plaintext[i]) && islower(plaintext[i])){
            ciphertext[i] = 'a' + (plaintext[i] - 'a' + key)%26;
        }
        else if (isalpha(plaintext[i]) && isupper(plaintext[i])){
            ciphertext[i] = 'A' + (plaintext[i] - 'A' + key)%26;
        }
        else{
            ciphertext[i] = plaintext[i];
        }
    }
    
    printf("ciphertext: %s\n", ciphertext); // Print ciphertext
}