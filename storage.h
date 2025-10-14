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

#define MAX_VECTORS 10

void clear_storage();
int add_vector(Vector v);
int find_vector_index(const char *name);
Vector get_vector(const char *name);
void list_vectors();

#endif