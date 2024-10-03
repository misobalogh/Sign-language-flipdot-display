#include <cstdio>
#include <cstring>
#include <fcntl.h>  // Contains file controls like O_RDWR
#include <cerrno>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include "serial.h"

// Opens a serial port and configures it
int serial_open(const char* port_name) {
    int serial_port = open(port_name, O_RDWR);  // Open the serial port for reading and writing

    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    struct termios tty;  // Create a termios struct for serial port settings

    // Read in existing settings and handle any error
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    // Configure serial port settings
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, using one stop bit
    tty.c_cflag &= ~CSIZE;   // Clear size bits
    tty.c_cflag |= CS8;      // 8 bits per byte
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines
    tty.c_lflag &= ~ICANON;  // Set to raw input
    tty.c_lflag &= ~ECHO;    // Disable echo
    tty.c_lflag &= ~ECHOE;   // Disable erasure
    tty.c_lflag &= ~ECHONL;  // Disable new-line echo

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable special handling
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes

    // Set in/out baud rate to be 57600
    cfsetispeed(&tty, B57600);
    cfsetospeed(&tty, B57600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    return serial_port;  // Return the serial port file descriptor
}

// Closes the serial port
void serial_close(int serial_port) {
    close(serial_port);  // Close the serial port
}

// Sends a message to the serial port
int serial_send(int serial_port, const unsigned char* message, size_t size) {
    int bytes_sent = write(serial_port, message, size);  // Write the message to the serial port

    if (bytes_sent == -1) {
        perror("Error writing to serial port");  // Print error message if write fails
        return -1;
    }

    return bytes_sent;  // Return the number of bytes sent
}
