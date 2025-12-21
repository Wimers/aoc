#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 64

static inline int within_range(long start, long end, long val)
{
    return (start <= val && val <= end);
}

int file_lc(FILE* file, char* buffer, int bufSize)
{
    int lc = 0;

    char* line = NULL;
    while ((line = fgets(buffer, bufSize, file)) != NULL) {
        lc++;
    }

    rewind(file);
    return lc;
}

void parse_id_ranges(
        FILE* file, int lc, long (*ranges)[2], char* buffer, int bufSize)
{
    char* line = NULL;
    int i = 0;

    for (; i < lc; i++) {
        line = fgets(buffer, bufSize, file);

        for (int j = 0; j < bufSize; j++) {
            char* endptr = NULL;
            ranges[i][0] = strtol(line, &endptr, 10);
            ranges[i][1] = strtol(endptr + 1, NULL, 10);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: ./day5 <idRanges> <ingredientIDs>\n");
        exit(99);
    }

    int freshIDs = 0;
    int lc;

    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    FILE* idRanges = fopen(argv[1], "r");
    lc = file_lc(idRanges, buffer, B_SIZE);

    long ranges[lc][2];
    memset(&ranges, 0, sizeof(ranges));

    parse_id_ranges(idRanges, lc, ranges, buffer, B_SIZE);
    fclose(idRanges);

    FILE* ingredients = fopen(argv[2], "r");
    char* line = NULL;

    while ((line = fgets(buffer, B_SIZE, ingredients)) != NULL) {
        const long product = strtol(line, NULL, 10);

        for (int k = 0; k < lc; k++) {
            if (within_range(ranges[k][0], ranges[k][1], product)) {
                freshIDs++;
                break;
            }
        }
    }

    printf(">> %d\n", freshIDs);
    fflush(stdout);
    fclose(ingredients);
}
