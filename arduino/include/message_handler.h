#include <memory>
#include "constants.h"

class MessageHandler {
public:
    std::unique_ptr<unsigned char[]> message;
    unsigned char address;
    unsigned char command;

    MessageHandler(unsigned char addr, unsigned char cmd) : address(addr), command(cmd) {
        init_message();
    }

    void init_message() {
        message.reset(create_message(address, command));
    }

    unsigned char* create_message(unsigned char addr, unsigned char cmd) {
        auto* message = new unsigned char[TOTAL_MESSAGE_SIZE]; 
        if (message == nullptr) {
            return nullptr;  
        }

        message[0] = HEADER;
        message[1] = cmd;
        message[2] = addr;

        for (int i = 0; i < DATA_SIZE; i++) {
            message[i + DATA_OFFSET] = 0x00;  
        }

        message[TOTAL_MESSAGE_SIZE - 1] = FOOTER;
        return message;
    }

    void set_letter_at_position(unsigned char letter, int pos, int line) {
        if (line < 0 || line >= NUM_LINES || pos < 0 || pos >= SYMBOLS_PER_LINE) {
            return;
        }

        message[pos + line * SYMBOLS_PER_LINE + DATA_OFFSET] = letter;
    }

    void sendMessage(SoftwareSerial &serial) {
        if (message) {
            serial.write(message.get(), TOTAL_MESSAGE_SIZE); 
        }
    }
};