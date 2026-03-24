# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Targets
ALL = process question1 question2

all: $(ALL)

# Compile sigtrap.c into 'process' executable
process: sigtrap.c
	$(CC) sigtrap.c -o process

# Compile question1.c
question1: question1.c
	$(CC) $(CFLAGS) question1.c -o question1

# Compile question2.c (ensure 'process' exists first)
question2: question2.c process
	$(CC) $(CFLAGS) question2.c -o question2

# Clean up binaries
clean:
	rm -f $(ALL)