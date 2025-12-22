#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 64

const char* const rangesInput = "src/D5/idRanges";

static inline long in_range(const long start, const long end)
{
    if (end == 0 && start == 0) {
        return 0;
    }

    return (end - start + 1);
}

static inline void remove_redundant(long* sR, long* eR, long* start, long* end)
{
    // Subset
    if (*start <= *sR && *eR <= *end) {
        *sR = 0;
        *eR = 0;
        return;
    }

    // Disjoint
    if ((*sR > *end) || (*eR < *start)) {
        return;
    }

    // Super set
    if (*sR <= *start && *eR >= *end) {
        *start = 0;
        *end = 0;
        return;
    }

    // Left within range, right is outside
    if ((*start <= *sR) && (*eR > *end)) {
        *end = *eR;
        *eR = 0;
        *sR = 0;
        return;
    }

    // Right within range, left is outside
    if ((*eR <= *end) && (*sR < *start)) {
        *start = *sR;
        *eR = 0;
        *sR = 0;
        return;
    }
}

void check_ranges(int lc, long (*range)[2])
{
    int i = 0;
    for (; i < lc; i++) {

        long* sSrt = &(range[i][0]);
        long* sEnd = &(range[i][1]);

        if (*sSrt == 0 && *sEnd == 0) {
            continue;
        }

        // Swap so sSrt < sEnd
        if (*sEnd < *sSrt) {
            long tmp = *sSrt;
            *sSrt = *sEnd;
            *sEnd = tmp;
        }

        for (int j = 0; j < lc; j++) {

            if (i == j) {
                continue;
            }

            long* start = &(range[j][0]);
            long* end = &(range[j][1]);

            if (*start == 0 && *end == 0) {
                continue;
            }

            // Swap so start < end
            if (*end < *start) {
                long tmp = *start;
                *start = *end;
                *end = tmp;
            }

            remove_redundant(sSrt, sEnd, start, end);
        }
    }
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
    for (int i = 0; i < lc; i++) {
        char* line = fgets(buffer, bufSize, file);
        char* endptr = NULL;

        ranges[i][0] = strtol(line, &endptr, 10);
        ranges[i][1] = strtol(endptr + 1, NULL, 10);
    }
}

long calc_total_fresh_ids(const char* path)
{
    long totalFreshIDs = 0;

    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    FILE* idRanges = fopen(path, "r");
    int lc = file_lc(idRanges, buffer, B_SIZE);

    long ranges[lc][2];
    memset(&ranges, 0, sizeof(ranges));

    parse_id_ranges(idRanges, lc, ranges, buffer, B_SIZE);
    fclose(idRanges);

    check_ranges(lc, ranges);

    for (int i = 0; i < lc; i++) {
        totalFreshIDs += in_range(ranges[i][0], ranges[i][1]);
    }

    return totalFreshIDs;
}

int main(const int argc, char* argv[])
{
    const long totFreshIDs = calc_total_fresh_ids(rangesInput);

    printf(">> %ld\n", totFreshIDs);
    fflush(stdout);
    return 0;
}
