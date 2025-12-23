#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define B_SIZE 1024

const char* const input = "src/D2/input";

static inline int count_digits(const long n)
{
    if (n < 10) {
        return 1;
    }

    return 1 + count_digits(n / 10);
}

void find_error(const long start, const long end);

long sol = 0;

int main(int argc, char* argv[])
{
    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    FILE* file = fopen(input, "r");
    char* line = fgets(buffer, B_SIZE, file);
    if (line == NULL) {
        exit(99);
    }

    long start = 0;
    long end = 0;

    char* cur = &(buffer[0]);
    for (int i = 0; i < B_SIZE; i++) {
        const int c = buffer[i];
        switch (c) {
        case '\0': {
            end = strtol(cur, &cur, 10);
            find_error(start, end);
            goto leave;
        }
        case '-': {
            buffer[i] = '\0';
            start = strtol(cur, &cur, 10);
            cur = &(buffer[i + 1]);
            continue;
        }
        case ',': {
            buffer[i] = '\0';
            end = strtol(cur, &cur, 10);
            find_error(start, end);
            cur = &(buffer[i + 1]);
            continue;
        }
        default:
            continue;
        }
    }

leave:

    printf(">> %ld\n", sol);
    fflush(stdout);
    fclose(file);
    return 0;
}

void find_error(const long start, const long end)
{
    // For each number from start to end
    for (long num = start; num <= end; num++) {

        // Calculate number of digits in number
        int n = count_digits(num);

        // Convert number to a char array
        char str[20];
        sprintf(str, "%ld", num);

        // Check for pattern in pairs of 1, 2, ... n / 2
        for (int rep = 1; rep <= n >> 1; rep++) {

            // If pattern width doesn't neatly fit into number length
            // ignore and move on
            if (n % rep != 0) {
                continue;
            }

            // Create temporary buffers
            char ref[rep];
            char temp[rep];
            memset(&ref, 0, sizeof(ref));
            memset(&temp, 0, sizeof(temp));

            // Populate reference buffer
            for (int i = 0; i < rep; i++) {
                ref[i] = str[i];
            }

            // For each windows that need to be checked
            for (int loop = 1; loop < (n / rep); loop++) {

                // Populate temp buffer
                for (int i = 0; i < rep; i++) {
                    temp[i] = str[rep * loop + i];
                }

                // If reference and temp buffers don't match,
                // go to the next pattern size.
                if (memcmp(ref, temp, rep) != 0) {
                    goto next;
                }
            }

            // All checks passed, pattern detected
            sol += num;
            goto leave;

        next:
            continue;
        }

        // To next number
    leave:
        continue;
    }
}
