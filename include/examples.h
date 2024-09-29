#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <sys/time.h> // For timeval

void example_time(unsigned char* message, int serial_port, unsigned int hours, unsigned int minutes, unsigned int seconds);
void example_timer(unsigned char* message, int serial_port, unsigned int hours, unsigned int minutes, unsigned int seconds);
void example_clock(unsigned char* message, int serial_port);
void example_current_time(unsigned char* message, int serial_port); 
void example_stopwatch(unsigned char* message, int serial_port);
void example_example1(unsigned char* message, int serial_port);
void example_example2(unsigned char* message, int serial_port, int word_at_line);
void example_example3(unsigned char* message, int serial_port);
void example_example4(unsigned char* message, int serial_port);
void example_send_and_clean(unsigned char* message, int serial_port);

#endif // EXAMPLES_H
