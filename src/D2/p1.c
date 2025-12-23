#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define B_SIZE 1024

const char* const input = "src/D2/input";

static const long lookup[]
        = {0, 9, 0, 999, 0, 99999, 0, 9999999, 0, 999999999, 0, 99999999999};

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

    for (long i = start; i <= end; i++) {
        int n = count_digits(i);

        if (n % 2) {
            i = lookup[n];

        } else {
            char str[20];
            sprintf(str, "%ld", i);

            int half = n >> 1;

            for (int k = 0; k < half; k++) {

                if (str[k] != str[k + half]) {
                    goto leave;
                }
            }
            sol += i;
        }
    leave:
        continue;
    }
}
