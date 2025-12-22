CC = gcc
CFLAGS = -std=gnu99 -Wall -Werror -pedantic
PFLAGS = -O3 -march=native -flto -funroll-loops

.PHONY: clean
.DEFAULT_GOAL: all

all: 01-p1
all: 01-p2
all: 02-p1
all: 02-p2
all: 03-p1
all: 03-p2
all: 04-p1
all: 04-p2
all: 05-p1
all: 05-p2
all: 06-p1

# DAY 1
01-p2: 01-p2.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

01-p2.o: src/D1/p2.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

01-p1: 01-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

01-p1.o: src/D1/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@



# DAY 2
02-p2: 02-p2.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

02-p2.o: src/D2/p2.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

02-p1: 02-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

02-p1.o: src/D2/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@


# DAY 3
03-p2: 03-p2.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

03-p2.o: src/D3/p2.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

03-p1: 03-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

03-p1.o: src/D3/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@




# DAY 4
04-p2: 04-p2.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

04-p2.o: src/D4/p2.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

04-p1: 04-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

04-p1.o: src/D4/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@


# DAY 5
05-p2: 05-p2.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

05-p2.o: src/D5/p2.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@

05-p1: 05-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

05-p1.o: src/D5/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@


# DAY 6

06-p1: 06-p1.o
	$(CC) $(CFLAGS) $(PFLAGS) $^ -o $@

06-p1.o: src/D6/p1.c
	$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@



clean:
	rm -f -v *.o *p1 *p2 *.out
