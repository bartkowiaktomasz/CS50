#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void){
    
    float change;
    do{
        printf("O hai! How much change is owed?\n");
        change = get_float();
    }
    while(change < 0);
    
    int intChange = lroundf (100*change);
    int counter = 0;
    int i = 0;
    while (intChange >= 25){
        i+= (intChange - intChange % 25)/25;
        intChange-= 25*i;
        counter+=i;
        break;
    }
    i = 0;
    while (intChange >= 10){
        i+= (intChange - intChange % 10)/10;
        intChange-= 10*i;
        counter+=i;
        break;
    }
    i = 0;
    while (intChange >= 5){
        i+= (intChange - intChange % 5)/5;
        intChange-= 5*i;
        counter+=i;
        break;
    }
    i = 0;
    while (intChange >= 1){
        counter+=intChange;
        break;
    }
        
    printf("%i\n", counter);
}