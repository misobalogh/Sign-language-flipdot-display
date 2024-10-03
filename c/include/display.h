#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include "display_letter.h"
#include "serial.h"

void display_clear(unsigned char address, unsigned char command, int serial_port);

#endif // DISPLAY_H
