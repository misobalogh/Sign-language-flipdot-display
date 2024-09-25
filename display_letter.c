#include <stdio.h>
#include <stdlib.h>

// 4x7 segment display
#define NUM_LINES 4
#define SYMBOLS_PER_LINE 7

// frame for serial communication
#define DATA_SIZE (NUM_LINES * SYMBOLS_PER_LINE)
#define TOTAL_MESSAGE_SIZE (DATA_SIZE + 4)
#define DATA_OFFSET 3 // header, command, address
#define HEADER 0x80 
#define FOOTER 0x8F


// 7 segment display symbols
#define P 0x67
#define F 0x47
#define SPACE 0x00
#define TWO 0x6D
#define ZERO 0x7E
#define FOUR 0x33


// sets letter @leter at position @pos in line @line
int set_letter_at_position(unsigned char* data, unsigned char letter, int pos, int line) {
	if (line < 0 || line >= NUM_LINES || pos < 0 || pos >= SYMBOLS_PER_LINE) {
		return -1;
	}

    data[pos + line * SYMBOLS_PER_LINE + DATA_OFFSET] = letter;
    return 0;
}

// initializes message with address and command
unsigned char* init_message(unsigned char address, unsigned char command) {
    unsigned char* message = (unsigned char*)malloc(TOTAL_MESSAGE_SIZE * sizeof(unsigned char));
    if (message == NULL) {
        return NULL; 
    }

    message[0] = HEADER;
    message[1] = command;
	message[2] = address;
	for (int i = 0; i < DATA_SIZE; i++) {
        message[i+3] = 0x00; 
    }

    message[TOTAL_MESSAGE_SIZE - 1] = FOOTER;

    return message;
}

int main() {
	unsigned char address = 0xFF;
	unsigned char command = 0x83;

	unsigned char* message = init_message(address, command);
    if (message == NULL) {
        fprintf(stderr, "Error allocating memmory for message.\n");
        return 1;
    }

	int word_at_line = 1;
	set_letter_at_position(message, P, 0, word_at_line);
	set_letter_at_position(message, F, 1, word_at_line);
	set_letter_at_position(message, SPACE, 2, word_at_line);
	set_letter_at_position(message, TWO, 3, word_at_line);
	set_letter_at_position(message, ZERO, 4, word_at_line);
	set_letter_at_position(message, TWO, 5, word_at_line);
	set_letter_at_position(message, FOUR, 6, word_at_line);

	// todo: send message

	printf("Message:\n");
	for (int i = 0; i < DATA_SIZE + 4; i++) {
        printf("0x%02X ", message[i]);
    }
    printf("\n");

    // clean up
    free(message);

    return 0;
}
