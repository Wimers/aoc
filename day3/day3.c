#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 1024

static inline int search_buffer(char buffer[B_SIZE])
{
    int top = -1;
    int second = -1;
    int nAfterTop = -1;

    for (int i = 0; i < B_SIZE; i++) {
        const char c = buffer[i];
        if (c == 0) {
            break;
        }

        const int val = c - '0';

        if (val > top) {
            second = top;
            top = val;
            nAfterTop = -1;
        } else {
            if (val > nAfterTop) {
                nAfterTop = val;
            }
        }
    }

    if (nAfterTop == -1) {
        return (second * 10 + top);
    }

    return (top * 10 + nAfterTop);
}

int main(int argc, char* argv[])
{
    // Check input file specified
    if (argc != 2) {
        exit(1);
    }

    // Initialise char buffer
    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    // Open input file
    FILE* file = fopen(argv[1], "r");
    char* line = NULL;

    int joltage = 0;

    // Calculate max joltage for each line and add to total
    while ((line = fgets(buffer, B_SIZE, file)) != NULL) {
        joltage += search_buffer(buffer);
    }

    printf(">> %d Jolts\n", joltage);
    fflush(stdout);
    fclose(file);
}
