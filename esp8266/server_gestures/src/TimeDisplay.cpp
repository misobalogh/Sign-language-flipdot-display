// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include "TimeDisplay.h"
#include <Arduino.h>

// Initializes the time client with the provided NTP server, time offset (time zone), and interval for updating the time
TimeDisplay::TimeDisplay(const char* ntpServer, long timeOffset, unsigned long updateInterval)
    : timeClient(ntpUDP, ntpServer, timeOffset, updateInterval), lastMinute(-1) {}

void TimeDisplay::begin() {
    timeClient.begin(); // Start the client to begin fetching time
}

void TimeDisplay::addTimeToMessage(MessageHandler& msgHandler) {
    timeClient.update(); 
    auto formattedDate = timeClient.getFormattedTime();

    // Debug print
    Serial.println(formattedDate);

    // Get index of the T in the formatted date string to split the date and time
    int splitT = formattedDate.indexOf("T");
    auto timeStamp = formattedDate.substring(splitT + 1, splitT + 6);

    // Get the tens and ones digit of the hours and minutes
    int hoursTens = timeStamp.charAt(0) - '0';
    int hoursOnes = timeStamp.charAt(1) - '0';
    int minutesTens = timeStamp.charAt(3) - '0';
    int minutesOnes = timeStamp.charAt(4) - '0';

    // Set the time on the message handler
    msgHandler.set_letter_at_position(get_segments_from_digit(hoursTens), 2, 0);
    msgHandler.set_letter_at_position(get_segments_from_digit(hoursOnes), 3, 0);
    msgHandler.set_letter_at_position(COLON, 4, 0);
    msgHandler.set_letter_at_position(get_segments_from_digit(minutesTens), 5, 0);
    msgHandler.set_letter_at_position(get_segments_from_digit(minutesOnes), 6, 0);
}

bool TimeDisplay::needUpdate() {
    timeClient.update();
    int currentMinute = timeClient.getMinutes();
    // Check if the minute has changed
    if (currentMinute != lastMinute) {
        lastMinute = currentMinute;
        return true;
    }
    return false;
}