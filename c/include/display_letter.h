#ifndef DISPLAY_LETTER_H
#define DISPLAY_LETTER_H

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
#define SPACE 0x00
#define HYPHEN 0x01


#define h 0x17
#define o 0x1D
#define r 0x05
#define t 0x0F

#define A 0x77
#define B 0x7F
#define C 0x4E
#define D 0x7E
#define E 0x4F
#define F 0x47
#define G 0x5E
#define H 0x37
#define I 0x06
#define J 0x3C
#define K 0x37
#define L 0x0E
#define M 0x55
#define N 0x76
#define O 0x7E
#define P 0x67
#define Q 0x6F
#define R 0x05
#define S 0x5B
#define T 0x0F
#define U 0x3E
#define V 0x3E
#define W 0x5C
#define X 0x49
#define Y 0x3B
#define Z 0x6D

#define ZERO 0x7E
#define ONE 0x30
#define TWO 0x6D
#define THREE 0x79
#define FOUR 0x33
#define FIVE 0x5B
#define SIX 0x5F
#define SEVEN 0x70
#define EIGHT 0x7F
#define NINE 0x7B


int set_letter_at_position(unsigned char* data, unsigned char letter, int pos, int line);
unsigned char* init_message(unsigned char address, unsigned char command);

unsigned char get_segments_from_digit(unsigned int digit);
unsigned char get_segments_from_letter(char letter);

#endif // DISPLAY_LETTER_H