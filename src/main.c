#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/time.h>
#include <time.h>

#include "display_letter.h"
#include "serial.h"

void display_stopwatch(unsigned char* message, unsigned int hours, unsigned int minutes, unsigned int seconds) {
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
}


void display_time(unsigned char* message) {
    struct timeval tv;
    gettimeofday(&tv, NULL); 
    int hours = (tv.tv_sec / 3600) % 24; 
    int minutes = (tv.tv_sec / 60) % 60;
    int seconds = tv.tv_sec % 60;
    int milliseconds = tv.tv_usec / 1000;

    unsigned char hour_tens = hours / 10;
    unsigned char hour_units = hours % 10;

    unsigned char minute_tens = minutes / 10;
    unsigned char minute_units = minutes % 10;

    unsigned char second_tens = seconds / 10;
    unsigned char second_units = seconds % 10;


    set_letter_at_position(message, get_segments_from_digit(hour_tens), 5, 0);
    set_letter_at_position(message, get_segments_from_digit(hour_units), 6, 0);
    
    set_letter_at_position(message, get_segments_from_digit(minute_tens), 5, 1);
    set_letter_at_position(message, get_segments_from_digit(minute_units), 6, 1);
    
    set_letter_at_position(message, get_segments_from_digit(second_tens), 5, 2);
    set_letter_at_position(message, get_segments_from_digit(second_units), 6, 2);

    // unsigned char millisecond_tens = milliseconds / 100; 
	// set_letter_at_position(message, get_segments_from_digit(millisecond_tens), 6, 3); 
}



void display_clear(unsigned char address, unsigned char command, int serial_port) {
    unsigned char* message_clear = init_message(address, command);
    if (message_clear == NULL) {
        fprintf(stderr, "Error allocating memory for message.\n");
        return; 
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        message_clear[i + DATA_OFFSET] = SPACE;
    }

    int bytes_sent = serial_send(serial_port, message_clear, TOTAL_MESSAGE_SIZE);
    if (bytes_sent != TOTAL_MESSAGE_SIZE) {
        fprintf(stderr, "Error: Not all bytes were sent.\n");
    }

    free(message_clear); 
}

void display_example1(unsigned char* message) { 
    set_letter_at_position(message, H, 1, 0);
    set_letter_at_position(message, E, 2, 0);
    set_letter_at_position(message, L, 3, 0);
    set_letter_at_position(message, L, 4, 0);
    set_letter_at_position(message, O, 5, 0);

    set_letter_at_position(message, t, 0, 1);
    set_letter_at_position(message, SPACE, 1, 1);
    set_letter_at_position(message, FIVE, 2, 1);
    set_letter_at_position(message, SEVEN, 3, 1);

    set_letter_at_position(message, r, 0, 2);
    set_letter_at_position(message, h, 1, 2);
    set_letter_at_position(message, SPACE, 2, 2);
    set_letter_at_position(message, THREE, 3, 2);
    set_letter_at_position(message, ONE, 4, 2);

    set_letter_at_position(message, ZERO, 0, 3);
    set_letter_at_position(message, ONE, 1, 3);
    set_letter_at_position(message, TWO, 2, 3);
    set_letter_at_position(message, THREE, 3, 3);
    set_letter_at_position(message, FOUR, 4, 3);
    set_letter_at_position(message, FIVE, 5, 3);
    set_letter_at_position(message, SIX, 6, 3);
}

void display_example2(unsigned char* message, int word_at_line) { 
    set_letter_at_position(message, P, 0, word_at_line);
    set_letter_at_position(message, F, 1, word_at_line);
    set_letter_at_position(message, SPACE, 2, word_at_line);
    set_letter_at_position(message, TWO, 3, word_at_line);
    set_letter_at_position(message, ZERO, 4, word_at_line);
    set_letter_at_position(message, TWO, 5, word_at_line);
    set_letter_at_position(message, FOUR, 6, word_at_line);
}

int main() {
    unsigned char address = 0xFF;
    unsigned char command = 0x83;

    int serial_port = serial_open("/dev/ttyUSB0");
    if (serial_port < 0) {
        fprintf(stderr, "Error opening serial port.\n");
        return 1;
    }

    unsigned char* message = init_message(address, command);
    if (message == NULL) {
        fprintf(stderr, "Error allocating memory for message.\n");
        serial_close(serial_port); 
        return 1;
    }

	display_clear(address, command, serial_port);

	int hours = 0;
    int minutes = 2;
    int seconds = 1;

	while (1) {
		display_stopwatch(message, hours, minutes, seconds);

		int bytes_sent = serial_send(serial_port, message, TOTAL_MESSAGE_SIZE);
		if (bytes_sent != TOTAL_MESSAGE_SIZE) {
			fprintf(stderr, "Error: Not all bytes were sent.\n");
		}
		sleep(1); 

        if (seconds == 0) {
            seconds = 59;
            minutes--;
            if (minutes == 0) {
                minutes = 59;
                hours--;
                if (hours > 9) {
                    hours = 0;
                }
            }
		}
		else {
			seconds--;
		}
	}
	

    // Clean up
    serial_close(serial_port);
    free(message);

    return 0;
}
