#include <iostream>
#include <memory>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "display_letter.h"
#include "display.h"
#include "serial.h"



void example_send_and_clean(unsigned char* message, int serial_port) {
    int bytes_sent = serial_send(serial_port, message, TOTAL_MESSAGE_SIZE);
    if (bytes_sent != TOTAL_MESSAGE_SIZE) {
        fprintf(stderr, "Error: Not all bytes were sent.\n");
    }
}

void example_time(unsigned char* message, int serial_port, unsigned int hours, unsigned int minutes, unsigned int seconds) {
    unsigned char hour_units = hours % 10;
    unsigned char minute_tens = minutes / 10;
    unsigned char minute_units = minutes % 10;
    unsigned char second_tens = seconds / 10;
    unsigned char second_units = seconds % 10;

    set_letter_at_position(message, get_segments_from_digit(hour_units), 0, 0);
    set_letter_at_position(message, HYPHEN, 1, 0);
    set_letter_at_position(message, get_segments_from_digit(minute_tens), 2, 0);
    set_letter_at_position(message, get_segments_from_digit(minute_units), 3, 0);
    set_letter_at_position(message, HYPHEN, 4, 0);
    set_letter_at_position(message, get_segments_from_digit(second_tens), 5, 0);
    set_letter_at_position(message, get_segments_from_digit(second_units), 6, 0);

    example_send_and_clean(message, serial_port);
}


void example_stopwatch(unsigned char* message, int serial_port) {
    unsigned int hours = 0;
    unsigned int minutes = 0;
    unsigned int seconds = 0;

    while (1) {
        example_time(message, serial_port, hours, minutes, seconds);
        sleep(1);

        if (seconds > 59) {
            seconds = 0;
            minutes++;
            if (minutes > 59) {
                minutes = 0;
                hours++;
                if (hours > 23) {
                    hours = 0; 
                }
            }
        }
        else {
            seconds++;
        }
    }
}


int main() {
    unsigned char address = 0xFF;
    unsigned char command = 0x83;

    // Open the serial port
    int serial_port = serial_open("/dev/ttyUSB0");
    if (serial_port < 0) {
        std::cerr << "Error opening serial port." << std::endl;
        return EXIT_FAILURE;
    }

    // Clear the display
    display_clear(address, command, serial_port);
    sleep(1);

    // Initialize the message
    std::unique_ptr<unsigned char[]> message(init_message(address, command));
    if (message == nullptr) {
        std::cerr << "Error allocating memory for message." << std::endl;
        serial_close(serial_port);
        return EXIT_FAILURE;
    }

    // TODO: Receive message from model
    example_stopwatch(message.get(), serial_port);
    
    serial_close(serial_port);
    // Unique_ptr automatically cleans up mem

    return EXIT_SUCCESS;  
}
