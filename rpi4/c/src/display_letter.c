// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include <stdlib.h>
#include "display_letter.h"

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

// Map a digit to its corresponding 7-segment display representation
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

// Map a letter to its corresponding 7-segment display representation
unsigned char get_segments_from_letter(char letter) {
    switch (letter) {
    case 'A': return A;
    case 'B': return B;
    case 'C': return C;
    case 'D': return D;
    case 'E': return E;
    case 'F': return F;
    case 'G': return G;
    case 'H': return H;
    case 'I': return I;
    case 'J': return J;
    case 'K': return K;
    case 'L': return L;
    case 'M': return M;
    case 'N': return N;
    case 'O': return O;
    case 'P': return P;
    case 'Q': return Q;
    case 'R': return R;
    case 'S': return S;
    case 'T': return T;
    case 'U': return U;
    case 'V': return V;
    case 'W': return W;
    case 'X': return X;
    case 'Y': return Y;
    case 'Z': return Z;
    case ' ': return SPACE;
    default: return 0x00;
    }
}
