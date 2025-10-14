/**
 * Connor Weldon
 * ui.c
 * 
 * Description: Handles all user input and output. 
 * Parses commands like a = 1 2 3, a + b, or clear, and calls the correct functions.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"
#include "storage.h"

static void print_vector(const char *name, Vector v) 
{
    printf("%s = %.2f %.2f %.2f\n", name, v.val[0], v.val[1], v.val[2]);
}

void run_ui() 
{
    char line[100];
    clear_storage();

    while (1) 
    {
        printf("minimat> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        if (strncmp(line, "quit", 4) == 0) 
        {
            break;
        }
        else if (strncmp(line, "clear", 5) == 0) 
        { 
            clear_storage(); printf("Cleared.\n"); 
        }
        else if (strncmp(line, "list", 4) == 0) 
        {
            list_vectors();
        }
        else if (strchr(line, '=') && strchr(line, '+')) 
        {
            char res[20], a[20], b[20];
            sscanf(line, "%s = %s + %s", res, a, b);
            Vector va = get_vector(a), vb = get_vector(b);

            if (a[0] && b[0] && find_vector_index(a)!=-1 && find_vector_index(b)!=-1) 
            {
                Vector c = add(va, vb);
                strcpy(c.name, res);
                add_vector(c);
                print_vector(res, c);
            } 
            else printf("Error: one or both vectors missing.\n");
        }

        else if (strchr(line, '=') && strchr(line, '-'))
         {
            char res[20], a[20], b[20];
            sscanf(line, "%s = %s - %s", res, a, b);
            Vector va = get_vector(a), vb = get_vector(b);

            if (find_vector_index(a)!=-1 && find_vector_index(b)!=-1)
            {
                Vector c = subtract(va, vb);
                strcpy(c.name, res);
                add_vector(c);
                print_vector(res, c);
            } 
            else printf("Error: one or both vectors missing.\n");
        }

        else if (strchr(line, '=') && strchr(line, '*')) 
        {
            char res[20], a[20];
            double scalar;

            if (sscanf(line, "%s = %s * %lf", res, a, &scalar) == 3) 
            {
                if (find_vector_index(a)!=-1) 
                {
                    Vector va = get_vector(a);
                    Vector c = scalar_multiply(va, scalar);
                    strcpy(c.name, res);
                    add_vector(c);
                    print_vector(res, c);
                }
            } 
            else if (sscanf(line, "%s = %lf * %s", res, &scalar, a) == 3) 
            {
                if (find_vector_index(a)!=-1) 
                {
                    Vector va = get_vector(a);
                    Vector c = scalar_multiply(va, scalar);
                    strcpy(c.name, res);
                    add_vector(c);
                    print_vector(res, c);
                }
            } 
            else printf("Invalid scalar multiply.\n");
        }

        else if (strchr(line, '+')) 
        {
            char a[20], b[20];
            sscanf(line, "%s + %s", a, b);

            if (find_vector_index(a)!=-1 && find_vector_index(b)!=-1) 
            {
                Vector c = add(get_vector(a), get_vector(b));
                print_vector("ans", c);
            } 
            else printf("Missing vectors.\n");
        }

        else if (strchr(line, '-')) 
        {
            char a[20], b[20];
            sscanf(line, "%s - %s", a, b);

            if (find_vector_index(a)!=-1 && find_vector_index(b)!=-1) 
            {
                Vector c = subtract(get_vector(a), get_vector(b));
                print_vector("ans", c);
            } 
            else printf("Missing vectors.\n");
        }

        else if (strchr(line, '*')) 
        {
            char a[20];
            double scalar;

            if (sscanf(line, "%s * %lf", a, &scalar) == 2 && find_vector_index(a)!=-1) 
            {
                Vector c = scalar_multiply(get_vector(a), scalar);
                print_vector("ans", c);
            } 
            else if (sscanf(line, "%lf * %s", &scalar, a) == 2 && find_vector_index(a)!=-1) 
            {
                Vector c = scalar_multiply(get_vector(a), scalar);
                print_vector("ans", c);
            } 
            else printf("Missing vector or invalid input.\n");
        }

        else if (strchr(line, '=')) 
        {
            char name[20];
            double x,y,z;

            if (sscanf(line, "%s = %lf %lf %lf", name, &x, &y, &z) == 4 ||
                sscanf(line, "%s = %lf,%lf,%lf", name, &x, &y, &z) == 4) 
            {
                Vector v;
                strcpy(v.name, name);
                v.val[0]=x; v.val[1]=y; v.val[2]=z;
                add_vector(v);
                print_vector(name, v);
            } 
            else printf("Invalid assignment format.\n");
        }

        else 
        {
            char name[20];
            sscanf(line, "%s", name);
            if (find_vector_index(name)!=-1) 
            {
                print_vector(name, get_vector(name));
            } 
            else printf("Unknown command or vector.\n");
        }
    }
}