// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#ifndef TEXT_ANIMATIONS_H
#define TEXT_ANIMATIONS_H

#include <SoftwareSerial.h>
#include "constants.h"
#include "MessageHandler.h"

// Different text animations for displaying message on the 7-segment display
void displayWelcomeMessage(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial);
void animateLetter(char targetLetter, uint8_t pos, uint8_t line, MessageHandler& msgHandler, SoftwareSerial& serial);
void animateAllLettersSimultaneously(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial);
void animateAllLettersSimultaneouslyRandom(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial);
void rollingLettersAnimation(const char* welcomeMessage, MessageHandler& msgHandler, SoftwareSerial& serial);

#endif // TEXT_ANIMATIONS_H