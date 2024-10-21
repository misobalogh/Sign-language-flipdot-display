// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include "text_animations.h"

// Display a welcome message letter by letter
void displayWelcomeMessage(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial) {
    uint8_t pos = 0;
    uint8_t line = 0;

    // Display each letter of the welcome message
    for (size_t i = 0; i < strlen(welcomeMessage); ++i) {
        char targetLetter = welcomeMessage[i];
        animateLetter(targetLetter, pos, line, msgHandler, serial);
        pos = (pos + 1) % SYMBOLS_PER_LINE;
        if (pos == 0) {
            line = (line + 1) % NUM_LINES;
        }
    }
}

// Animate a single letter from A to targetLetter
void animateLetter(char targetLetter, uint8_t pos, uint8_t line, MessageHandler& msgHandler, SoftwareSerial& serial) {
    for (char currentLetter = 'A'; currentLetter <= targetLetter; ++currentLetter) {
        msgHandler.set_letter_at_position(get_segments_from_letter(currentLetter), pos, line);
        msgHandler.sendMessage(serial);
        delay(100);  // Animation speed
    }
}

// Animate all letters of the welcome message simultaneously
void animateAllLettersSimultaneously(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial) {
    uint8_t line = 1;

    char currentLetters[strlen(welcomeMessage)];
    for (size_t i = 0; i < strlen(welcomeMessage); ++i) {
        currentLetters[i] = 'A';
    }

    bool animationComplete = false;
    while (!animationComplete) {
        animationComplete = true;

        // Increment each letter until it reaches the target letter
        for (size_t i = 0; i < strlen(welcomeMessage); ++i) {
            char targetLetter = welcomeMessage[i];

            // If the current letter is less than the target letter, increment it
            if (currentLetters[i] < targetLetter) {
                currentLetters[i]++;
                animationComplete = false;
            }

            msgHandler.set_letter_at_position(get_segments_from_letter(currentLetters[i]), i + 1, line);
        }

        msgHandler.sendMessage(serial);

        delay(100);
    }
}


// Animate all letters of the welcome message simultaneously but with fixed number of switches of random letters
void animateAllLettersSimultaneouslyRandom(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial) {
    uint8_t line = 1;

    int num_random_letters = 0;
    int max_random_letters = 10;
    char randomLetter;

    // Display random letters until the maximum number of random letters is reached
    while (num_random_letters < max_random_letters) {

        for (size_t i = 0; i < strlen(welcomeMessage); ++i) {
            randomLetter = 'A' + random(26);
            msgHandler.set_letter_at_position(get_segments_from_letter(randomLetter), i + 1, line);
        }

        num_random_letters++;
        msgHandler.sendMessage(serial);

        delay(100);
    }

    // Finally, display the welcome message
    for (size_t i = 0; i < strlen(welcomeMessage); ++i) {
        msgHandler.set_letter_at_position(get_segments_from_letter(welcomeMessage[i]), i + 1, line);
    }
    msgHandler.sendMessage(serial);
}


// Animate all letters of the welcome message rolling from left to right, locking letters 1 by 1
void rollingLettersAnimation(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial) {
    uint8_t line = 1;
    uint8_t lockedLetters = 0;
    uint8_t totalLetters = strlen(welcomeMessage);
    char randomLetter;

    // Display random letters until all letters are locked
    while (lockedLetters < totalLetters) {
        for (size_t i = 0; i < totalLetters; ++i) {
            if (i < lockedLetters) {
                // Display the correct letter for the locked letters
                msgHandler.set_letter_at_position(get_segments_from_letter(welcomeMessage[i]), i + 1, line);
            }
            else {
                // Display a random letter for the remaining unlocked letters
                randomLetter = 'A' + random(26);
                msgHandler.set_letter_at_position(get_segments_from_letter(randomLetter), i + 1, line);
            }
        }

        msgHandler.sendMessage(serial);

        delay(100);
        static int counter = 0;
        counter++;
        if (counter >= 10) {
            lockedLetters++;
            counter = 0;
        }
    }

    // Finally, display the welcome message
    for (size_t i = 0; i < totalLetters; ++i) {
        msgHandler.set_letter_at_position(get_segments_from_letter(welcomeMessage[i]), i + 1, line);
    }
    msgHandler.sendMessage(serial);
}


