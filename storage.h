/**
 * Connor Weldon
 * storage.h
 * CPE2600
 * Description: Header for storage.c. Declares the storage
 * management functions used by the UI layer.
 */

#ifndef STORAGE_H
#define STORAGE_H

#include "vector.h"

// Core storage operations
void clear_storage(void);
int add_vector(Vector v);                  // returns 1 on success, 0 on failure
int find_vector_index(const char *name);   // returns index or -1 if not found
Vector get_vector(const char *name);
void list_vectors(void);

// File IO
int load_vectors(const char *filename);    // returns number of vectors loaded, -1 on error
int save_vectors(const char *filename);    // returns number of vectors saved, -1 on error 

#endif