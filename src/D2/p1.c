#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define B_SIZE 1024

const char* const input = "src/D2/input";

static inline int count_digits(const long long n)
{
    if (n < 10) {
        return 1;
    }

    return 1 + count_digits(n / 10);
}

static inline int nDigit_num(const int n)
{
    int out = 1;

    for (int i = 1; i < n; i++) {
        out *= 10;
    }

    return out;
}

void find_error(const long long start, const long long end);

long long sol = 0;

int main(int argc, char* argv[])
{
    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    FILE* file = fopen(input, "r");
    char* line = fgets(buffer, B_SIZE, file);
    if (line == NULL) {
        exit(99);
    }

    long long start = 0;
    long long end = 0;

    char* cur = &(buffer[0]);
    for (int i = 0; i < B_SIZE; i++) {
        const int c = buffer[i];
        switch (c) {
        case '\0': {
            end = strtoll(cur, &cur, 10);
            find_error(start, end);
            goto leave;
        }
        case '-': {
            buffer[i] = '\0';
            start = strtoll(cur, &cur, 10);
            cur = &(buffer[i + 1]);
            continue;
        }
        case ',': {
            buffer[i] = '\0';
            end = strtoll(cur, &cur, 10);
            find_error(start, end);
            cur = &(buffer[i + 1]);
            continue;
        }
        default:
            continue;
        }
    }

leave:

    printf(">> %lld\n", sol);
    fflush(stdout);
    fclose(file);
    return 0;
}

void find_error(const long long start, const long long end)
{
    for (long long i = start; i <= end; i++) {
        int n = count_digits(i);

        if (n % 2) {
            i = nDigit_num(n + 1) - 1;

        } else {
            char str[20];
            sprintf(str, "%lld", i);

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
