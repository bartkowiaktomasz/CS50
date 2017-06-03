/**
 * Reports a user’s water usage, converting minutes spent in the shower to bottles of drinking water.
 */

#include <stdio.h>
#include <cs50.h>

int main(void){
    
    printf("Minutes: ");
    int minutes = get_int();
    printf("Bottles: %i\n", 12*minutes);
}