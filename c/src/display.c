// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include "display.h"

// Clears the display by sending a message with all spaces
void display_clear(unsigned char address, unsigned char command, int serial_port) {
    unsigned char* message_clear = init_message(address, command);
    if (message_clear == NULL) {
        return; 
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        message_clear[i + DATA_OFFSET] = SPACE;
    }

    int bytes_sent = serial_send(serial_port, message_clear, TOTAL_MESSAGE_SIZE);
    if (bytes_sent != TOTAL_MESSAGE_SIZE) {
        return;
    }

    free(message_clear); 
}
