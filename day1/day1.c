#include <stdio.h>
#include <stdlib.h>

#define B_SIZE 32
#define RIGHT 1
#define LEFT 0
#define COMBO 100

static inline int mod(const int a, const int b)
{
    return ((a % b + b) % b);
}

static inline int zeros(int* pointer, const int rot, const int dir)
{
    if (rot == 0) {
        return 0;
    }

    int out = 0;
    int val;

    if (dir == RIGHT) {
        val = mod(*pointer + rot, COMBO);
        if (val < *pointer) {
            out = 1;
        }
        goto leave;
    }

    val = mod(*pointer - rot, COMBO);
    if (val == 0 || (val > *pointer && *pointer != 0)) {
        out = 1;
    }

leave:
    *pointer = val;
    return out;
}

int main(int argc, char* argv[])
{
    // Open the input data file
    FILE* file = fopen(argv[1], "r");

    // Init combo
    int pointer = 50;

    // Init
    char* line = NULL;
    int lineNum = 0;
    int sol = 0;

    while (1) {
        char buffer[B_SIZE];

        line = fgets(buffer, B_SIZE, file);
        if (line == NULL) {
            break;
        }

        lineNum++;

        // Determine direction and number of rotations
        int dir = (line[0] == 'L') ? (LEFT) : (RIGHT);
        int rot = atoi((line + 1));

        // Account for trivial rotations
        sol += rot / COMBO;

        // Determine number of non-trival passes of zero.
        sol += zeros(&pointer, rot % COMBO, dir);
    }

    // Print the solution
    printf(">> %d\n", sol);
}
