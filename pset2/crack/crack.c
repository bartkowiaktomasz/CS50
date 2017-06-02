// The program cracks passwords hashed with DES-based function, up to four characters, composed entirely of alphabetical characters

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>


int main(int argc, string argv[]){
    
    // If user does not prompt two arguments in the CLI - return error
    if (argc != 2){                                 
        printf("Error, only one argument (hash) is allowed\n");
        return 1;
    }
    
    string hash = argv[1];
    string salt = "50";
    char generatedPassword[5];  // String with max. 4 chars and \0 at the end. The string will be encrypted and compared to the input hash

            for (int i = 'A'; i <= 'z'; i++ ){      // Iterate through ASCII characters starting from 'A', ending at 'z'
                if (isalpha(i)){                    // If the character is alphabetical, start password-generating procedure
                    generatedPassword[0] = i;       // Generate one-letter passwords 
                    generatedPassword[1] = '\0';
                    string encryptedPassword = crypt(generatedPassword, salt);  // Encrypt password
                    if (!strcmp(encryptedPassword, hash)){                      // Check if the encrypted password is the same as input hash
                        printf ("\nThe password is: %s\n", generatedPassword);
                        return 0;
                    }
                    for (int j = 'A'; j <= 'z'; j++ ){
                        if (isalpha(j)){
                            generatedPassword[0] = i;   // Generate two-letter passwords
                            generatedPassword[1] = j;
                            generatedPassword[2] = '\0';
                            encryptedPassword = crypt(generatedPassword, salt);
                            if (!strcmp(encryptedPassword, hash)){
                                printf ("\nThe password is: %s\n", generatedPassword);
                                return 0;
                            }
                            for (int k = 'A'; k <= 'z'; k++ ){
                                if (isalpha(k)){
                                    generatedPassword[0] = i;   // Generate three-letter passwords
                                    generatedPassword[1] = j;
                                    generatedPassword[2] = k;
                                    generatedPassword[3] = '\0';
                                    encryptedPassword = crypt(generatedPassword, salt);
                                    if (!strcmp(encryptedPassword, hash)){
                                        printf ("\nThe password is: %s\n", generatedPassword);
                                        return 0;
                                    }
                                    for (int l = 'A'; l <= 'z'; l++ ){
                                        if (isalpha(l)){
                                            generatedPassword[0] = i;   //  // Generate four-letter passwords
                                            generatedPassword[1] = j;
                                            generatedPassword[2] = k;
                                            generatedPassword[3] = l;
                                            generatedPassword[4] = '\0';
                                            encryptedPassword = crypt(generatedPassword, salt);
                                            if (!strcmp(encryptedPassword, hash)){
                                                printf ("\nThe password is: %s\n", generatedPassword);
                                                return 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
}