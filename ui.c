/**
 * Connor Weldon
 * ui.c
 * 
 * Description: Handles all user input and output. 
 * Parses commands like a = 1 2 3, a + b, or clear, and calls the correct functions.
 * load *filename*
 * save *filename*
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vector.h"
#include "storage.h"

static void print_vector(const char *name, Vector v)
{
    printf("%s = %.2f %.2f %.2f\n", name, v.val[0], v.val[1], v.val[2]);
}

// helper to trim leading spaces for parsing checks
static char *ltrim(char *s)
{
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

void run_ui()
{
    char line[256];
    clear_storage();

    while (1)
    {
        printf("minimat> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        // remove trailing newline
        size_t ln = strlen(line);
        if (ln && line[ln - 1] == '\n') line[ln - 1] = '\0';

        char *cmd = ltrim(line);

        if (strncmp(cmd, "quit", 4) == 0 && (cmd[4] == '\0' || isspace((unsigned char)cmd[4])))
        {
            break;
        }
        else if (strncmp(cmd, "clear", 5) == 0 && (cmd[5] == '\0' || isspace((unsigned char)cmd[5])))
        {
            clear_storage();
            printf("Cleared.\n");
        }
        else if ((strncmp(cmd, "list", 4) == 0 && (cmd[4] == '\0' || isspace((unsigned char)cmd[4])))
                || (strncmp(cmd, "ls", 2) == 0   && (cmd[2] == '\0' || isspace((unsigned char)cmd[2]))))
        {
            list_vectors();
        }
        else if (strncmp(cmd, "load", 4) == 0 && isspace((unsigned char)cmd[4]))
        {
            char fname[200];
            if (sscanf(cmd + 4, "%199s", fname) == 1) {
                load_vectors(fname);
            } else {
                printf("Usage: load <filename>\n");
            }
        }
        else if (strncmp(cmd, "save", 4) == 0 && isspace((unsigned char)cmd[4]))
        {
            char fname[200];
            if (sscanf(cmd + 4, "%199s", fname) == 1) {
                save_vectors(fname);
            } else {
                printf("Usage: save <filename>\n");
            }
        }
        // addition: res = a + b
        else if (strchr(cmd, '=') && strchr(cmd, '+'))
        {
            char res[20] = {0}, a[20] = {0}, b[20] = {0};
            if (sscanf(cmd, "%19s = %19s + %19s", res, a, b) >= 3) {
                if (find_vector_index(a) != -1 && find_vector_index(b) != -1) 
                {
                    Vector va = get_vector(a), vb = get_vector(b);
                    Vector c = add(va, vb);
                    strncpy(c.name, res, sizeof(c.name) - 1);
                    c.name[sizeof(c.name) - 1] = '\0';
                    add_vector(c);
                    print_vector(res, c);
                } else {
                    printf("Error: one or both vectors missing.\n");
                }
            } else {
                printf("Invalid expression.\n");
            }
        }
        // subtraction: res = a - b 
        else if (strchr(cmd, '=') && strchr(cmd, '-'))
        {
            char res[20] = {0}, a[20] = {0}, b[20] = {0};
            if (sscanf(cmd, "%19s = %19s - %19s", res, a, b) >= 3) {
                if (find_vector_index(a) != -1 && find_vector_index(b) != -1) {
                    Vector va = get_vector(a), vb = get_vector(b);
                    Vector c = subtract(va, vb);
                    strncpy(c.name, res, sizeof(c.name) - 1);
                    c.name[sizeof(c.name) - 1] = '\0';
                    add_vector(c);
                    print_vector(res, c);
                } else {
                    printf("Error: one or both vectors missing.\n");
                }
            } else {
                printf("Invalid expression.\n");
            }
        }
        // scalar multiply: res = a * scalar  OR res = scalar * a
        else if (strchr(cmd, '=') && strchr(cmd, '*'))
        {
            char res[20] = {0}, a[20] = {0};
            double scalar;
            int matched = 0;
            if (sscanf(cmd, "%19s = %19s * %lf", res, a, &scalar) == 3) matched = 1;
            else if (sscanf(cmd, "%19s = %lf * %19s", res, &scalar, a) == 3) matched = 1;

            if (matched) 
            {
                if (find_vector_index(a) != -1) 
                {
                    Vector va = get_vector(a);
                    Vector c = scalar_multiply(va, scalar);
                    strncpy(c.name, res, sizeof(c.name) - 1);
                    c.name[sizeof(c.name) - 1] = '\0';
                    add_vector(c);
                    print_vector(res, c);
                } else {
                    printf("Error: vector '%s' missing.\n", a);
                }
            } else {
                printf("Invalid scalar multiply.\n");
            }
        }
        // expression without assignment: a + b
        else if (strchr(cmd, '+'))
        {
            char a[20] = {0}, b[20] = {0};
            if (sscanf(cmd, "%19s + %19s", a, b) >= 2) 
            {
                if (find_vector_index(a) != -1 && find_vector_index(b) != -1) 
                {
                    Vector c = add(get_vector(a), get_vector(b));
                    print_vector("ans", c);
                } else {
                    printf("Missing vectors.\n");
                }
            } else {
                printf("Invalid expression.\n");
            }
        }
        // expression without assignment: a - b
        else if (strchr(cmd, '-') && (strchr(cmd, '=') == NULL))
        {
            char a[20] = {0}, b[20] = {0};
            if (sscanf(cmd, "%19s - %19s", a, b) >= 2) 
            {
                if (find_vector_index(a) != -1 && find_vector_index(b) != -1) 
                {
                    Vector c = subtract(get_vector(a), get_vector(b));
                    print_vector("ans", c);
                } else {
                    printf("Missing vectors.\n");
                }
            } else {
                printf("Invalid expression.\n");
            }
        }
        // scalar multiply without assignment: a * scalar OR scalar * a
        else if (strchr(cmd, '*') && (strchr(cmd, '=') == NULL))
        {
            char a[20] = {0};
            double scalar;
            if (sscanf(cmd, "%19s * %lf", a, &scalar) == 2 && find_vector_index(a) != -1) 
            {
                Vector c = scalar_multiply(get_vector(a), scalar);
                print_vector("ans", c);
            } else if (sscanf(cmd, "%lf * %19s", &scalar, a) == 2 && find_vector_index(a) != -1) 
            {
                Vector c = scalar_multiply(get_vector(a), scalar);
                print_vector("ans", c);
            } else {
                printf("Missing vector or invalid input.\n");
            }
        }
        // assignment of literal vector: name = x y z  OR name = x,y,z
        else if (strchr(cmd, '='))
        {
            char name[20] = {0};
            double x, y, z;
            
            if (sscanf(cmd, "%19s = %lf %lf %lf", name, &x, &y, &z) == 4 ||
                sscanf(cmd, "%19s = %lf,%lf,%lf", name, &x, &y, &z) == 4) 
            {
                Vector v;
                strncpy(v.name, name, sizeof(v.name) - 1);
                v.name[sizeof(v.name) - 1] = '\0';
                v.val[0] = x; v.val[1] = y; v.val[2] = z;
                add_vector(v);
                print_vector(name, v);
            } else {
                printf("Invalid assignment format.\n");
            }
        }
        // bare word: print vector if exists
        else
        {
            char name[20] = {0};
            if (sscanf(cmd, "%19s", name) == 1) {
                if (find_vector_index(name) != -1) {
                    print_vector(name, get_vector(name));
                } else {
                    printf("Unknown command or vector.\n");
                }
            } else {
                printf("Unknown command or vector.\n");
            }
        }
    }
}