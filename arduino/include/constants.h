#ifndef CONSTANTS_H
#define CONSTANTS_H

// 4x7 segment display
constexpr int NUM_LINES = 4;
constexpr int SYMBOLS_PER_LINE = 7;

// Frame for serial communication
constexpr int DATA_SIZE = NUM_LINES * SYMBOLS_PER_LINE;
constexpr int TOTAL_MESSAGE_SIZE = DATA_SIZE + 4;
constexpr int DATA_OFFSET = 3; // header, command, address
constexpr unsigned char HEADER = 0x80; 
constexpr unsigned char FOOTER = 0x8F;

// 7 segment display symbols
constexpr unsigned char SPACE = 0x00;
constexpr unsigned char HYPHEN = 0x01;

constexpr unsigned char h = 0x17;
constexpr unsigned char o = 0x1D;
constexpr unsigned char r = 0x05;
constexpr unsigned char t = 0x0F;

constexpr unsigned char A = 0x77;
constexpr unsigned char B = 0x7F;
constexpr unsigned char C = 0x4E;
constexpr unsigned char D = 0x7E;
constexpr unsigned char E = 0x4F;
constexpr unsigned char F = 0x47;
constexpr unsigned char G = 0x5E;
constexpr unsigned char H = 0x37;
constexpr unsigned char I = 0x06;
constexpr unsigned char J = 0x3C;
constexpr unsigned char K = 0x37;
constexpr unsigned char L = 0x0E;
constexpr unsigned char M = 0x55;
constexpr unsigned char N = 0x76;
constexpr unsigned char O = 0x7E;
constexpr unsigned char P = 0x67;
constexpr unsigned char Q = 0x6F;
constexpr unsigned char R = 0x05;
constexpr unsigned char S = 0x5B;
constexpr unsigned char T = 0x0F;
constexpr unsigned char U = 0x3E;
constexpr unsigned char V = 0x3E;
constexpr unsigned char W = 0x5C;
constexpr unsigned char X = 0x49;
constexpr unsigned char Y = 0x3B;
constexpr unsigned char Z = 0x6D;

constexpr unsigned char ZERO = 0x7E;
constexpr unsigned char ONE = 0x30;
constexpr unsigned char TWO = 0x6D;
constexpr unsigned char THREE = 0x79;
constexpr unsigned char FOUR = 0x33;
constexpr unsigned char FIVE = 0x5B;
constexpr unsigned char SIX = 0x5F;
constexpr unsigned char SEVEN = 0x70;
constexpr unsigned char EIGHT = 0x7F;
constexpr unsigned char NINE = 0x7B;

unsigned char get_segments_from_digit(unsigned int digit);
unsigned char get_segments_from_letter(char letter);

#endif // CONSTANTS_H
