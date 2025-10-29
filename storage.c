/**
 * Connor Weldon
 * storage.c
 * CPE2600
 * Description: Manages memory for up to 10 stored vectors. 
 * Handles adding, finding, listing, and clearing vectors.
 */

#define _POSIX_C_SOURCE 200809L
#define INITIAL_CAPACITY 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "storage.h"

static Vector *storage = NULL;
static int count = 0;
static int capacity = 0;

static int ensure_capacity(void)
{
    if (count < capacity) return 1;

    int newcap = (capacity == 0) ? INITIAL_CAPACITY : capacity * 2;
    Vector *tmp = realloc(storage, newcap * sizeof(Vector));

    if (!tmp) 
    {
        fprintf(stderr, "Memory allocation failed while resizing storage.\n");
        return 0;
    }

    storage = tmp;
    capacity = newcap;
    return 1;
}

void clear_storage() 
{
    if (storage) 
    {
        free(storage);
        storage = NULL;
    }
    count = 0;
    capacity = 0;
}

int add_vector(Vector v) 
{
    // If name exists, overwrite
    for (int i = 0; i < count; i++) 
    {
        if (strncmp(storage[i].name, v.name, sizeof(storage[i].name)) == 0) 
        {
            storage[i] = v;
            return 1;
        }
    }

    // Append
    if (!ensure_capacity()) return 0;
    // Safe copy
    strncpy(storage[count].name, v.name, sizeof(storage[count].name) - 1);
    storage[count].name[sizeof(storage[count].name) - 1] = '\0';
    storage[count].val[0] = v.val[0];
    storage[count].val[1] = v.val[1];
    storage[count].val[2] = v.val[2];
    count++;
    return 1;
}

int find_vector_index(const char *name) 
{
    if (!storage) return -1;
    for (int i = 0; i < count; i++) 
    {
        if (strncmp(storage[i].name, name, sizeof(storage[i].name)) == 0) return i;
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
    for (int i = 0; i < count; i++) 
    {
        printf("%s = %.2f %.2f %.2f\n", storage[i].name,
           storage[i].val[0], storage[i].val[1], storage[i].val[2]);
    }
}

static void trim_whitespace(char *s)
{
    if (!s) return;

    // trim leading
    char *p = s;
    while (isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    // trim trailing
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

// Remove quotes
static void clean_name(char *s)
{
    if (!s || !*s) return;

    // Strip UTF-8 BOM (EF BB BF)
    unsigned char *u = (unsigned char *)s;
    if (u[0] == 0xEF && u[1] == 0xBB && u[2] == 0xBF) 
    {
        memmove(s, s + 3, strlen(s + 3) + 1);
    }

    // Remove leading quote
    if (s[0] == '"' || s[0] == '\'')
    {
        memmove(s, s + 1, strlen(s));
    }
    // Remove end quote
    size_t len = strlen(s);
    if (len > 0 && (s[len - 1] == '"' || s[len - 1] == '\''))
    {
        s[len - 1] = '\0';
    }
}

int load_vectors(const char *filename)
{
    if (!filename || filename[0] == '\0') 
    {
        fprintf(stderr, "load: missing filename.\n");
        return -1;
    }

    FILE *f = fopen(filename, "r");
    if (!f) 
    {
        perror("load");
        return -1;
    }

    char line[512];
    int loaded = 0;
    int lineno = 0;

    while (fgets(line, sizeof(line), f)) 
    {
        lineno++;
        trim_whitespace(line);
        if (line[0] == '\0') continue; // skip blank lines

        for (char *p = line; *p; p++) if (*p == ',') *p = ' ';

        char namebuf[20];
        double x, y, z;
        // sscanf skips extra spaces
        int n = sscanf(line, "%19s %lf %lf %lf", namebuf, &x, &y, &z);
        clean_name(namebuf);
        if (n != 4) 
        {
            // bad line: report and skip
            fprintf(stderr, "load: skipping bad line %d: %s\n", lineno, line);
            continue;
        }

        Vector v;
        strncpy(v.name, namebuf, sizeof(v.name) - 1);
        v.name[sizeof(v.name) - 1] = '\0';
        v.val[0] = x; v.val[1] = y; v.val[2] = z;

        if (!add_vector(v)) 
        {
            fprintf(stderr, "load: failed to add vector from line %d\n", lineno);
            // continue trying to load others
        } 
        else {
            loaded++;
        }
    }

    fclose(f);
    printf("Loaded %d vectors from '%s'.\n", loaded, filename);
    return loaded;
}

int save_vectors(const char *filename)
{
    if (!filename || filename[0] == '\0') 
    {
        fprintf(stderr, "save: missing filename.\n");
        return -1;
    }

    FILE *f = fopen(filename, "w");
    if (!f) 
    {
        perror("save");
        return -1;
    }

    int wrote = 0;
    for (int i = 0; i < count; i++) 
    {
        // write as CSV
        if (fprintf(f, "%s,%.10g,%.10g,%.10g\n",
                    storage[i].name,
                    storage[i].val[0],
                    storage[i].val[1],
                    storage[i].val[2]) < 0) 
        {
            fprintf(stderr, "save: write error.\n");
            fclose(f);
            return -1;
        }
        wrote++;
    }

    fclose(f);
    printf("Saved %d vectors to '%s'.\n", wrote, filename);
    return wrote;
}