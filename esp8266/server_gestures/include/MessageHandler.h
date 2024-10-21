// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <memory>
#include <SoftwareSerial.h>
#include "constants.h"

// Class for initializing messages, setting letters at specific positions and sending them
class MessageHandler {
public:
    // Message to be sent
    std::unique_ptr<unsigned char[]> message;

    // Address and command of the message from the flipdot specification
    unsigned char address;
    unsigned char command;

    // Constructor
    MessageHandler(unsigned char addr, unsigned char cmd);

    // Initialize message that can be later set with letters
    void init_message();

    // Creates new empty message
    unsigned char* create_message(unsigned char addr, unsigned char cmd);

    // Set letter at specific position
    void set_letter_at_position(unsigned char letter, int pos, int line);

    // Send message via serial
    void sendMessage(SoftwareSerial& serial);

    // Clear message
    void clear(SoftwareSerial& serial);
};

#endif // MESSAGEHANDLER_H