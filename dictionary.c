// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;


int inde(char a)
{

    if(isalpha(a))
    {
        a = tolower(a);
        return a - 97;
    }
    else
        return 26;
}

node *root;
node *node_chil;
unsigned int siz;
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    root = calloc(1, sizeof(node));
    FILE *dict = fopen(dictionary, "r");
    if (!dictionary)
        return false;

    char buffer;

    while(true)
    {
        node_chil = root;
        fread(&buffer, sizeof(char), 1, dict);
        if (feof(dict))
        {
            fclose(dict);
            return true;
        }
        while(node_chil != NULL && node_chil -> children[inde(buffer)] != NULL)
        {
            node_chil = node_chil -> children[inde(buffer)];
            fread(&buffer, sizeof(char), 1, dict);
        }

        do
        {
            if (feof(dict))
            {
                fclose(dict);
                return true;
            }
            node *temp_node = calloc(1, sizeof(node));
            node_chil -> children[inde(buffer)] = temp_node;
            node_chil = temp_node;
            fread(&buffer, sizeof(char), 1, dict);
        }
        while(strcmp(&buffer, "\n") != 0);

        node_chil -> is_word = true;
        siz += 1;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO

    int i = 0;
    if (strlen(word) == 1 && root -> children[inde(tolower(word[i]))] != NULL)
    {
        node_chil = root -> children[inde(tolower(word[i]))];
        if (node_chil -> is_word == true)
            return true;
        else
            return false;
    }
    else
    {
        node_chil = root;
        while(node_chil -> children[inde(tolower(word[i]))] != NULL && i < strlen(word))
        {
            node_chil = node_chil -> children[inde(tolower(word[i]))];
            i++;
        }

        if (node_chil != NULL && node_chil -> is_word == true && i == strlen(word))
            return true;
        else
            return false;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (siz)
        return siz;
    else
        return 0;
}
bool eq(node *nod)
{
    for (int j =0; j < 26; j++)
    {
        if (nod -> children[j] != nod -> children[j + 1])
        return false;
    }
    return true;
}

bool un_load(node *root0)
{
    if (!root0)
        return false;
    node *temp = root0;
    if (temp -> children[0] == NULL && eq(temp))
        free(temp);
    else
        for(int n = 0; n < 27; n++)
        {
            un_load(temp -> children[n]);
        }
    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    if (un_load(root))
        return true;
    else
        return false;
}

