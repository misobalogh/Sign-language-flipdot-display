CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRC = src/main.c src/display_letter.c src/serial.c
OUTPUT = program

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

clean:
	rm -f $(OUTPUT)

run: $(OUTPUT)
	./$(OUTPUT)

.PHONY: all clean run
