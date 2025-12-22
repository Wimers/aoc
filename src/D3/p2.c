#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 128
#define BATTERY_T 12

const char* const input = "src/D3/input";

static inline long nDigit_num(const int n)
{
    long out = 1;

    for (int i = 1; i < n; i++) {
        out *= 10;
    }

    return out;
}

static inline int best_choice(
        char buffer[B_SIZE], const int offset, int* index, const int lineSize)
{
    int best = -1;

    for (int i = *index; i < B_SIZE; i++) {

        const char c = buffer[i];
        if (c == 0) {
            break;
        }

        const int val = c - '0';

        if ((val > best) && (lineSize >= i + offset)) {
            best = val;
            *index = i;

            if (best == 9) {
                break;
            }
        }
    }

    return best;
}

long search_buffer(char buffer[B_SIZE])
{
    int array[BATTERY_T];
    memset(&array, -1, sizeof(array));

    size_t lineSize = strlen(buffer) - 1;

    long total = 0;
    int index = -1;

    for (int i = 0; i < BATTERY_T; i++) {

        const int offset = BATTERY_T - i;
        index++;

        array[i] = best_choice(buffer, offset, &index, lineSize);
        total += array[i] * nDigit_num(offset);
    }

    return total;
}

int main(const int argc, char* argv[])
{
    // Initialise char buffer
    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    // Open input file
    FILE* file = fopen(input, "r");
    char* line = NULL;

    long joltage = 0;

    // Calculate max joltage for each line and add to total
    while ((line = fgets(buffer, B_SIZE, file)) != NULL) {
        joltage += search_buffer(buffer);
    }

    printf(">> %ld Jolts\n", joltage);
    fflush(stdout);
    fclose(file);
}
