// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <WiFiUdp.h>
#include <NTPClient.h>
#include "MessageHandler.h"  

// Class for displaying time on the flipdot display
class TimeDisplay {
public:
    // Constructor
    TimeDisplay(const char* ntpServer, long timeOffset, unsigned long updateInterval);

    // Initialize NTP client
    void begin();

    // Set the message to the current time
    void addTimeToMessage(MessageHandler& msgHandler);

    // Check if the time has changed
    bool needUpdate();

private:
    // UDP client for NTP
    WiFiUDP ntpUDP;

    // NTP client for getting the time
    NTPClient timeClient;

    // Last minute that was displayed for keeping track of time changes
    int lastMinute;
};

#endif // TIMEDISPLAY_H
