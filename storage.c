/**
 * Connor Weldon
 * storage.c
 * CPE2600
 * Description: Manages memory for up to 10 stored vectors. 
 * Handles adding, finding, listing, and clearing vectors.
 */

 //UPDATE FOR GIT

#include <stdio.h>
#include <string.h>
#include "storage.h"

static Vector storage[MAX_VECTORS];
static int used[MAX_VECTORS];

void clear_storage() 
{
    for (int i = 0; i < MAX_VECTORS; i++) used[i] = 0;
}

int add_vector(Vector v) 
{
    for (int i = 0; i < MAX_VECTORS; i++) 
    {
        if (used[i] && strcmp(storage[i].name, v.name) == 0) 
        {
            storage[i] = v;
            return 1;
        }
    }
    for (int i = 0; i < MAX_VECTORS; i++) 
    {
        if (!used[i]) 
        {
            storage[i] = v;
            used[i] = 1;
            return 1;
        }
    }
    printf("Memory full, cannot add vector.\n");
    return 0;
}

int find_vector_index(const char *name) 
{
    for (int i = 0; i < MAX_VECTORS; i++) 
    {
        if (used[i] && strcmp(storage[i].name, name) == 0) 
        {
            return i;
        }
    }
    return -1;
}

Vector get_vector(const char *name) 
{
    Vector empty = { "", {0,0,0} };
    int idx = find_vector_index(name);
    if (idx == -1) return empty;

    return storage[idx];
}

void list_vectors() 
{
    for (int i = 0; i < MAX_VECTORS; i++) 
    {
        if (used[i]) 
        {
            printf("%s = %.2f %.2f %.2f\n", storage[i].name,
                   storage[i].val[0], storage[i].val[1], storage[i].val[2]);
        }
    }
}