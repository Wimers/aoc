#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define B_SIZE 4096

typedef struct {
    int problemNum;
    int lc;
} Dimensions;

void fill_problem_line(
        Dimensions* dimn, int* problemSet, char* line, int lineNum, int flag)
{
    char c;
    int i = 0;
    int state = 1;

    int valCount = 0;

    while (1) {
        c = line[i];
        if (c == '\0' || c == '\n') {
            break;
        }

        if (c == ' ') {
            if (!state) {
                state = 1;
            }
            i++;
            continue;
        }

        if (state) {
            if (!flag) {
                problemSet[(dimn->problemNum * lineNum) + valCount]
                        = atoi(&(line[i]));
            } else {
                problemSet[(dimn->problemNum * dimn->lc) + valCount] = (int)c;
            }
            valCount++;
            state = 0;
        }
        i++;
    }
}

static inline int problem_num(FILE* file, char* buffer, int bufSize)

{
    char* line = fgets(buffer, bufSize, file);
    int num = 0;
    int state = 1;

    for (int i = 0; i < bufSize; i++) {
        const char c = line[i];

        if (c == '\0' || c == '\n') {
            break;
        }

        if (c == ' ') {
            if (!state) {
                state = 1;
            }
            continue;
        }

        if (state) {
            num++;
            state = 0;
        }
    }

    return num;
}

void find_dimensions(Dimensions* dimn, FILE* file, char* buffer, int bufSize)
{
    int lc = 0;
    int problemNum = 0;
    char* line = NULL;

    while ((line = fgets(buffer, bufSize, file)) != NULL) {
        if (!problemNum) {
            problemNum = problem_num(file, buffer, bufSize);
        }
        lc++;
    }

    dimn->lc = lc;
    dimn->problemNum = problemNum;
}

long solve_problem_set(Dimensions* dimn, FILE* file, char* buffer, int bufSize)
{
    rewind(file);

    int* problemSet = malloc(sizeof(int) * (dimn->lc + 1) * dimn->problemNum);

    char* line = NULL;
    int flag = 0;
    for (int i = 0; i < dimn->lc; i++) {
        line = fgets(buffer, bufSize, file);
        fill_problem_line(dimn, problemSet, line, i, flag);
    }

    flag = 1;
    line = fgets(buffer, bufSize, file);
    fill_problem_line(dimn, problemSet, line, dimn->lc, flag);

    long totalResult = 0;
    for (int i = 0; i < dimn->problemNum; i++) {

        int progress = 0;
        long current = 0;
        const char symb = (char)(problemSet[dimn->lc * dimn->problemNum + i]);

        for (int j = 0; j < dimn->lc; j++) {
            long val = (long)(problemSet[dimn->problemNum * j + i]);

            if (!progress) {
                current = val;
                progress = 1;
                continue;
            }

            switch (symb) {

            case '*': {
                current *= val;
                break;
            }

            case '+': {
                current += val;
                break;
            }

            default:
                break;
            }
        }

        totalResult += current;
    }

    free(problemSet);
    return totalResult;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        exit(99);
    }

    char buffer[B_SIZE];
    memset(&buffer, 0, sizeof(buffer));

    FILE* file = fopen(argv[1], "r");

    Dimensions dimn;
    memset(&dimn, 0, sizeof(dimn));

    find_dimensions(&dimn, file, buffer, B_SIZE);
    const long total = solve_problem_set(&dimn, file, buffer, B_SIZE);

    fclose(file);

    // printf(">> %d x %d\n", dimn.problemNum, dimn.lc);
    printf(">> %ld\n", total);
    fflush(stdout);
}
