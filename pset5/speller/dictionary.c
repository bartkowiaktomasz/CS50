/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

const int ARRAY_SIZE = 27;  // alphabet size of 26 plus apostrophe character
const int letter_a = 'a';   // letter a as int (97)
const int letter_z = 'z';   // letter z as int (122)

int numberOfWords = 0;  // number of words in a dictionary

// node structure
typedef struct node
{
    bool is_word;
    struct node *children[ARRAY_SIZE];
}
node;

node *root;

/**
 * Frees a node
 */
void freeNode(node *node);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *currentNode = root;
    
    int i = 0;
    while (word[i] != '\0')
    {
        char c = tolower(word[i]);    // character is small letter
        
        // apostrophe character has an index next to 'z'
        if (c == '\'')
        {
            c = letter_z + 1;
        }
        
        if (currentNode -> children[c - letter_a] != NULL)
        {
            currentNode = currentNode -> children[c - letter_a];
            i++;
        }
        else
        {
            return false;
        }
    }
    
    if (currentNode -> is_word == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    root = (node*) malloc(sizeof(node));
    node *currentNode = NULL;
    
    FILE *fp = fopen(dictionary, "r");
    
    // check file
    if (fp == NULL)
    {
        printf("Could not open");
        return 1;
    }
    
    int c = fgetc(fp);  // character
    
    // check each character till the end of file
    while (c != EOF)
    {
        currentNode = root;
        
        // check each character in a word
        while (c != '\n')
        {
            // change to lower case
            if (isalpha(c) == 1)
            {
                c = tolower(c);
            }
            
            // apostrophe character has an index next to 'z'
            if (c == '\'')
            {
                c = letter_z + 1; 
            }
            
            if (currentNode -> children[c - letter_a] == NULL)
            {
                currentNode -> children[c - letter_a] = (node*) malloc(sizeof(node));
                currentNode = currentNode -> children[c - letter_a];
            }
            else
            {
                currentNode = currentNode -> children[c - letter_a];
            }
            c = fgetc(fp);
        }
        currentNode -> is_word = true;
        numberOfWords++;
        c = fgetc(fp);
    }
    
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return numberOfWords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *currentNode = root;
    freeNode(currentNode);
    return true;
}

/**
 * Frees a node
 */
void freeNode(node *node)
{
    int i = 0;
    while(i < ARRAY_SIZE)
    {
        if (node -> children[i] != NULL)
        {
            freeNode(node -> children[i]);
        }
        i++;
    }
    free(node);
}