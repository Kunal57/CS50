/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

// create a node structure
typedef struct node
{
    bool is_word;
    struct node* children[27];
} node;

// keep track of the first node
node* root;

// keep track of number of words in the dictionary
unsigned int dictionary_words;

// declare unloader function
bool unloader(node* node_ptr);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // create key for children nodes
    char children_key[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\''};

    // the trav pointer should point to the value of root which is also a pointer to the beginning of the dictionary
    node* trav = root;

    // loop through the word
    for (int z = 0; z > -1; z++)
    {
        // if end of the word is reached
        if (word[z] == '\0')
        {
            // if dereferencing the pointer trav to get to the is_word member of the struct is equal to true
            if (trav -> is_word == true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            // loop through the children_key to match characters
            for (int y = 0; y < 27; y++)
            {
                // if characters match, execute
                if (tolower(word[z]) == children_key[y])
                {
                    // if that space in the trav structure if NULL, the word isn't in the dictionary and it isn't a valid word
                    if (trav->children[y] == NULL)
                    {
                        return false;
                    }
                    else
                    {
                        // the space is filled so the word can still possibly be in the dictionary
                        // to advance further down the tree, trav is reassigned to the value of that space which is a pointer to another node and the process can continue
                        trav = trav->children[y];
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // give the root node a chuck of memory to begin at
    root = calloc(1, sizeof(node));
    if (root == NULL)
    {
        printf("Out of memory!\n");
        return false;
    }

    // open the dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // create key for children nodes
    char children_key[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\''};

    // assign the tav pointer to the value
    // of root which is also a pointer
    node* trav = root;

    // assign words variable to zero
    // begin keeping track of number of words in dictionary
    dictionary_words = 0;

    // char letter will hold the next letter in the document
    char letter = fgetc(fp);

    // loop till End of File
    while (letter != EOF)
    {
        // execute if letter equals new line character
        if (letter == '\n')
        {
            // dereference the trav pointer and change its is_word member to be true since it is the end of the word
            trav -> is_word = true;
            // increment words counter by 1
            dictionary_words ++;
            // trav should point to the root pointer to start a new word
            trav = root;
            // next character should be pulled
            letter = fgetc(fp);
        }
        else
        {
            // loop through the character_key
            for (int k = 0; k < 27; k++)
            {
                // execute once we find a match
                if (letter == children_key[k])
                {
                    // if the space is NULL execute
                    if (trav->children[k] == NULL)
                    {
                        // allocate space for new node
                        node* new_node = calloc(1, sizeof(node));
                        if (new_node == NULL)
                        {
                            printf("No Memory!");
                            return false;
                        }

                        // that NULL space should be given the new_node pointer & trav should now be at the new_node
                        trav->children[k] = new_node;
                        trav = new_node;
                        letter = fgetc(fp);
                        break;
                    }
                    else
                    {
                        // if a node is found, the trav pointer should point to the beginning of the node found
                        trav = trav->children[k];
                        letter = fgetc(fp);
                        break;
                    }
                }
            }
        }
    }

    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return the dictionary_words variable
    // implemented in the load function above
    if (dictionary_words == 0)
    {
        printf("No Words in Dictionary.");
        return 0;
    }
    return dictionary_words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // return the value of the unloader function
    bool loaded = unloader(root);
    if (loaded)
    {
        return true;
    }
    return false;
}

bool unloader(node* node_ptr)
{
    // iterates over the node childrens array
    for (int g = 0; g < 28; g++)
    {
        // if all of the nodes equal NULL
        if (g == 27)
        {
            // free the node
            free(node_ptr);
            // set it equal to NULL
            node_ptr = 0;
            return true;
        }
        // if a node within the struct isn't NULL
        else if (node_ptr->children[g] != 0)
        {
            // recursively call the unloader function on that node
            unloader(node_ptr->children[g]);
        }
    }

    return false;
}
