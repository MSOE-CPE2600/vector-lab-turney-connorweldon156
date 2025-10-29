/**
 * Connor Weldon
 * main.c
 * 
 * Compile with:
 *      gcc main.c vector.c storage.c ui.c -o minimat
 *   or
 *      make
 * 
 * Starts the program. Checks for the -h help argument, then launches the user interface.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"
#include "storage.h"

int main(int argc, char *argv[])
{
    // reg cleanup
    if (atexit(clear_storage) != 0) {
        fprintf(stderr, "Warning: could not register exit handler.\n");
    }

    if (argc > 1 && strcmp(argv[1], "-h") == 0)
    {
        printf("Usage:\n");
        printf(" Run with no args for interactive mode.\n");
        printf(" Commands: list, clear, quit, load <fname>, save <fname>\n");
        printf(" Examples:\n");
        printf(" a = 1 2 3\n");
        printf(" b = 4 5 6\n");
        printf(" c = a + b\n");
        printf(" d = c * 2\n");
        return 0;
    }
    run_ui();
    return 0;
}