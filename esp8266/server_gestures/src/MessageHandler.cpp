// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include "MessageHandler.h"

MessageHandler::MessageHandler(unsigned char addr, unsigned char cmd) : address(addr), command(cmd) {
    init_message();
}

void MessageHandler::init_message() {
    // Resets pointer to the message
    message.reset(create_message(address, command));
}

unsigned char* MessageHandler::create_message(unsigned char addr, unsigned char cmd) {
    auto* message = new unsigned char[TOTAL_MESSAGE_SIZE];
    if (message == nullptr) {
        return nullptr;
    }

    // Set header, command, address 
    message[0] = HEADER;
    message[1] = cmd;
    message[2] = addr;

    // Initialize data with zeros
    for (int i = 0; i < DATA_SIZE; i++) {
        message[i + DATA_OFFSET] = 0x00;
    }

    // Set footer
    message[TOTAL_MESSAGE_SIZE - 1] = FOOTER;
    return message;
}

void MessageHandler::set_letter_at_position(unsigned char letter, int pos, int line) {
    if (line < 0 || line >= NUM_LINES || pos < 0 || pos >= SYMBOLS_PER_LINE) {
        return;
    }

    // Calculate position in the message as index in 2D array
    message[pos + line * SYMBOLS_PER_LINE + DATA_OFFSET] = (letter);
}

void MessageHandler::sendMessage(SoftwareSerial& serial) {
    if (message) {
        serial.write(message.get(), TOTAL_MESSAGE_SIZE);
    }
}

void MessageHandler::clear(SoftwareSerial& serial) {
    init_message();
    sendMessage(serial);
}