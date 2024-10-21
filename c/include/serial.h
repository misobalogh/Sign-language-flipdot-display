// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#ifndef SERIAL_H
#define SERIAL_H

// Open a serial port and return the file descriptor
int serial_open(const char* port_name);

// Write a message to the serial port
int serial_send(int serial_port, const unsigned char* message, size_t size);

// Close the serial port
void serial_close(int serial_port);

#endif // SERIAL_H
