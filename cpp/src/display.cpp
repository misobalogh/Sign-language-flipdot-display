#include "display.h"

void display_clear(unsigned char address, unsigned char command, int serial_port) {
    unsigned char* message_clear = init_message(address, command);  // Initialize the clear message
    if (message_clear == nullptr) {
        return;  
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        message_clear[i + DATA_OFFSET] = SPACE;
    }

    int bytes_sent = serial_send(serial_port, message_clear, TOTAL_MESSAGE_SIZE);
    if (bytes_sent != TOTAL_MESSAGE_SIZE) {
        free(message_clear);  
        return;
    }

    delete[] message_clear; 
}
