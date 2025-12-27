#include <stdio.h>
#include <stdlib.h>

#define B_SIZE 64
#define COMBO 100

static inline int mod(int a, const int b)
{
    int r = a % b;
    return r + (b & (r >> 31));
}

int main(const int argc, char* argv[])
{
    FILE* file = fopen("src/D1/input", "r");

    int pointer = 50;

    char* line = NULL;
    int lineNum = 0;
    int sol = 0;

    while (1) {
        if (pointer == 0) {
            sol++;
        }

        char buffer[B_SIZE];

        line = fgets(buffer, B_SIZE, file);
        if (line == NULL) {
            break;
        }

        lineNum++;
        int val = atoi((line + 1));

        if (line[0] == 'L') {
            pointer = mod(pointer - val, COMBO);
            continue;
        }

        if (line[0] == 'R') {
            pointer = mod(pointer + val, COMBO);
            continue;
        }
    }

    printf(">> %d\n", sol);
}
