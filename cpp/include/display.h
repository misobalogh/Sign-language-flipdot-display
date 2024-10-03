#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdlib>  
#include "display_letter.h"
#include "serial.h"

// Function to clear the display
void display_clear(unsigned char address, unsigned char command, int serial_port);

#endif // DISPLAY_H
