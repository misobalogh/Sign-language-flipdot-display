#include <cstdlib>
#include "display_letter.h"

// Sets letter @letter at position @pos in line @line
int set_letter_at_position(unsigned char* data, unsigned char letter, int pos, int line) {
    if (line < 0 || line >= NUM_LINES || pos < 0 || pos >= SYMBOLS_PER_LINE) {
        return -1;  
    }

    data[pos + line * SYMBOLS_PER_LINE + DATA_OFFSET] = letter; 
    return 0;  
}

unsigned char* init_message(unsigned char address, unsigned char command) {
    auto* message = new unsigned char[TOTAL_MESSAGE_SIZE]; 
    if (message == nullptr) {
        return nullptr;  
    }

    message[0] = HEADER;
    message[1] = command;
    message[2] = address;

    for (int i = 0; i < DATA_SIZE; i++) {
        message[i + DATA_OFFSET] = 0x00;  
    }

    message[TOTAL_MESSAGE_SIZE - 1] = FOOTER;
    return message;
}

unsigned char get_segments_from_digit(unsigned int digit) {
    switch (digit) {
        case 0: return ZERO;
        case 1: return ONE;
        case 2: return TWO;
        case 3: return THREE;
        case 4: return FOUR;
        case 5: return FIVE;
        case 6: return SIX;
        case 7: return SEVEN;
        case 8: return EIGHT;
        case 9: return NINE;
        default: return 0x00;  
    }
}

unsigned char get_segments_from_letter(char letter) {
    switch (letter) {
        case 'E': return E;
        case 'F': return F;
        case 'H': return H;
        case 'L': return L;
        case 'O': return O;
        case 'P': return P;
        default: return 0x00;  
    }
}
