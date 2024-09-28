#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h> 
#include <unistd.h>

#include "display_letter.h"
#include "serial.h"
#include "examples.h"

void example_send_and_clean(unsigned char* message, int serial_port) {
    int bytes_sent = serial_send(serial_port, message, TOTAL_MESSAGE_SIZE);
    if (bytes_sent != TOTAL_MESSAGE_SIZE) {
        fprintf(stderr, "Error: Not all bytes were sent.\n");
    }
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

void example_timer(unsigned char* message, int serial_port, unsigned int hours, unsigned int minutes, unsigned int seconds) {
	while (1) {
		example_time(message, serial_port, hours, minutes, seconds);

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
}

// Display a stopwatch in H-MM-SS format
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

void example_clock(unsigned char* message, int serial_port) {
    while (1) {
        example_current_time(message, serial_port);
        sleep(1);
    }
}

// Display the current time in HH-MM-SS format
void example_current_time(unsigned char* message, int serial_port) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    int hours = (tv.tv_sec / 3600) % 24;
    int minutes = (tv.tv_sec / 60) % 60;
    int seconds = tv.tv_sec % 60;

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

    example_send_and_clean(message, serial_port);
}

// Example 1: Display a fixed message
void example_example1(unsigned char* message, int serial_port) {
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

    example_send_and_clean(message, serial_port);
}

// Example 2: Display a fixed message with a line number
void example_example2(unsigned char* message, int serial_port, int word_at_line) {
    set_letter_at_position(message, P, 0, word_at_line);
    set_letter_at_position(message, F, 1, word_at_line);
    set_letter_at_position(message, SPACE, 2, word_at_line);
    set_letter_at_position(message, TWO, 3, word_at_line);
    set_letter_at_position(message, ZERO, 4, word_at_line);
    set_letter_at_position(message, TWO, 5, word_at_line);
    set_letter_at_position(message, FOUR, 6, word_at_line);

    example_send_and_clean(message, serial_port);
}
