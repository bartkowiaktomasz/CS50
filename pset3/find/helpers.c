/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

#define MAX 65536

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // O(logn) time searching algorithm (binary search). If "value" is within the array "values[] return true. Else return false
    int lowerBoundary = 0;
    int higherBoundary = n-1;
    int center;
    
    if (n <= 0){
        return false;
    }

    while (higherBoundary >= lowerBoundary)
    {
        center = (lowerBoundary+higherBoundary)/2;  // center is of type int, so it is an integer even if the outcome (RHS) itelf is not int
        if (value == values[center])
        {
            return true;
        }
        else if (value > values[center])
        {
            lowerBoundary = center + 1;
        }
        else
        {
            higherBoundary = center - 1;    
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // sorting using counting sort
    int counter[MAX] = {0};
    int sortedValues[n];
    
    // for each "value" from "values" array, add one to i-th place in "counter" array, where "i" is the "value"
    for (int i = 0; i < n; i++)
    {
        counter[values[i]]++;
    }
    
    // put sorted numbers back in the "values" array
    int j = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (counter[i] > 0){
            sortedValues[j] = i;
            counter[i]--;
            i--;
            values[j] = sortedValues[j];
            j++;
        }
    }
    
/*
    // print sorted array of numbers
    printf("Sorted values:\n");
    for (int i = 0; i < n; i++)
    {
        printf ("%i \t", values[i]);
    }
    printf("\n");
*/
    return;
}
