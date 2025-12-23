#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define B_SIZE 512
#define MAX_ROLLS 4
#define RADIUS 1
#define FIRST 1
#define SECOND 2
#define LAST (-1)

const char rollSymb = '@';
const char removeSymb = 'x';

const char* const input = "src/D4/input";
const char* const tmpIn = "in.tmp";
const char* const tmpOut = "out.tmp";
const char* const finalOut = "final.out";

int loop(FILE* file, FILE* tmp);

typedef struct {
    char r1[B_SIZE];
    char r2[B_SIZE];
    char r3[B_SIZE];
    int size;
} Rows;

void move_rows(FILE* tmp, Rows* rows, char buffer[B_SIZE], int flag)
{
    if (flag != FIRST && flag != SECOND) {
        fwrite(&(rows->r1), sizeof(char), rows->size + 1, tmp);
    }

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
    int updated = 0;

tryAgain:

    for (int i = 0; i < rows->size; i++) {
        if ((rows->r2)[i] == rollSymb) {
            int count = normal_checks(rows, i, RADIUS, flag);

            if (count < MAX_ROLLS + 1) {
                accessable++;
                (rows->r2)[i] = removeSymb;
                updated = 1;
            }
        }
    }

    if (updated) {
        updated = 0;
        goto tryAgain;
    }

    return accessable;
}

int main(const int argc, char* argv[])
{
    int nRolls = 0;

    // Open input file
    FILE* file = fopen(input, "r");

    // Open file for storage
    FILE* tmp = fopen(tmpOut, "w");

    int yield = loop(file, tmp);
    fclose(file);
    fclose(tmp);

    int exitCode = 0;
magic:

    if (yield != 0) {

        nRolls += yield;
        pid_t pid = fork();

        if (!pid) {
            execlp("cp", "cp", tmpOut, tmpIn, NULL);
            _exit(99);
        } else {
            waitpid(pid, 0, 0);

            FILE* newIn = fopen(tmpIn, "r");
            FILE* newOut = fopen(tmpOut, "w");

            yield = loop(newIn, newOut);
            fclose(newIn);
            fclose(newOut);

            // exitCode++; // Returns numbers of file iterations

            goto magic;
        }
    }

    printf(">> %d\n", nRolls);

    rename(tmpOut, finalOut);
    remove(tmpIn);
    return exitCode;
}

int loop(FILE* file, FILE* tmp)
{
    Rows rows;
    memset(&rows, 0, sizeof(rows));

    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    // Read first line and copy to Rows struct
    int flag = FIRST;
    char* line = fgets(buffer, B_SIZE, file);

    if (line != NULL) {
        memcpy(&(rows.r3), &buffer, B_SIZE);

        // Set size of row (-1 to account for new line)
        if (rows.size == 0) {
            rows.size = strlen(line) - 1;
        }
    }

    int yield = 0;

    while (1) {

        line = fgets(buffer, B_SIZE, file);

        if (line == NULL) {
            flag = LAST;
        }

        // Rearrange rows to fit the newly read line
        move_rows(tmp, &rows, buffer, flag);

        // Calc number of accessable rolls in middle row, given the previous and
        // next rows.
        yield += forklift(&rows, flag);

        // All rows read
        if (flag == LAST) {
            fwrite(&(rows.r1), sizeof(char), rows.size + 1, tmp);
            fwrite(&(rows.r2), sizeof(char), rows.size + 1, tmp);
            break;
        }

        if (flag == FIRST) {
            flag = SECOND;
            continue;
        }

        // Reset flag to normal mode
        flag = 0;
    }

    return yield;
}
