#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 256
#define MAX_ROLLS 4
#define RADIUS 1
#define FIRST 1
#define LAST (-1)

const char rollSymb = '@';

typedef struct {
    char r1[B_SIZE];
    char r2[B_SIZE];
    char r3[B_SIZE];
    int size;
} Rows;

void move_rows(Rows* rows, char buffer[B_SIZE])
{
    memcpy(rows->r1, rows->r2, B_SIZE);
    memcpy(rows->r2, rows->r3, B_SIZE);
    memcpy(rows->r3, buffer, B_SIZE);
}

static inline int calc_min(const int index, const int radius)
{
    const int min = (index > radius) ? (index - radius) : 0;
    return min;
}

static inline int calc_max(
        const int index, const int radius, const int lineSize)
{
    int max = index + radius;
    max = (max >= lineSize) ? (lineSize - 1) : max;
    return max;
}

static inline int row_checker(char buffer[B_SIZE], int min, int max)
{
    int rollCount = 0;

    for (int i = min; i <= max; i++) {
        if (buffer[i] == rollSymb) {
            rollCount++;
        }
    }

    return rollCount;
}

int normal_checks(Rows* rows, const int index, const int radius, const int flag)
{
    int count = 0;

    const int min = calc_min(index, radius);
    const int max = calc_max(index, radius, rows->size);

    if (flag != FIRST) {
        count += row_checker(rows->r1, min, max);
    }

    count += row_checker(rows->r2, min, max);

    if (flag != LAST) {
        count += row_checker(rows->r3, min, max);
    }

    return count;
}

int forklift(Rows* rows, const int flag)
{
    int accessable = 0;

    for (int i = 0; i < rows->size; i++) {
        if ((rows->r2)[i] == rollSymb) {
            int count = normal_checks(rows, i, RADIUS, flag);

            if (count < MAX_ROLLS + 1) {
                accessable++;
            }
        }
    }

    return accessable;
}

int main(const int argc, char* argv[])
{
    // Check input file specified
    if (argc != 2) {
        exit(1);
    }

    Rows rows;
    memset(&rows, 0, sizeof(rows));

    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    // Open input file
    FILE* file = fopen(argv[1], "r");

    int nRolls = 0;

    // Read first line and copy to Rows struct
    int flag = FIRST;
    char* line = fgets(buffer, B_SIZE, file);
    memcpy(&(rows.r3), &buffer, B_SIZE);

    // Set size of row (-1 to account for new line)
    rows.size = strlen(line) - 1;

    while (1) {

        line = fgets(buffer, B_SIZE, file);

        if (line == NULL) {
            flag = LAST;
        }

        // Rearrange rows to fit the newly read line
        move_rows(&rows, buffer);

        // Calc number of accessable rolls in middle row, given the previous and
        // next rows.
        nRolls += forklift(&rows, flag);

        // All rows read
        if (flag == LAST) {
            break;
        }

        // Reset flag to normal mode
        flag = 0;
    }

    printf(">> %d\n", nRolls);
    fflush(stdout);
    fclose(file);
}
