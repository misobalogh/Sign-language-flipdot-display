#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/time.h>
#include <time.h>

#include "display_letter.h"
#include "display.h"
#include "serial.h"

#include "examples.h"

int main() {
    unsigned char address = 0xFF;
    unsigned char command = 0x83;

    int serial_port = serial_open("/dev/ttyUSB0");
    if (serial_port < 0) {
        fprintf(stderr, "Error opening serial port.\n");
        return 1;
    }

	display_clear(address, command, serial_port);

    unsigned char* message = init_message(address, command);
    if (message == NULL) {
        fprintf(stderr, "Error allocating memory for message.\n");
        serial_close(serial_port); 
        return 1;
    }

	example_clock(message, serial_port);	

    // Clean up
    serial_close(serial_port);
    free(message);

    return 0;
}
