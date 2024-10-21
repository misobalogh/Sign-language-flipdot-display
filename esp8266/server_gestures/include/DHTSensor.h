// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>

#include "MessageHandler.h"
#include "constants.h"

class DHTSensor {
public:
    // Constructor for the DHTSensor class. Initializes the DHT object with the given pin and type.
    DHTSensor(uint8_t pin, uint8_t type);

    // Initializes the DHT sensor.
    void begin();

    // Reads the temperature and adds it to a message of the MessageHandler and SoftwareSerial.
    void addTemperatureToMessage(MessageHandler& msgHandler, SoftwareSerial& serial);

    // Reads the humidity and adds it to a message of the MessageHandler and SoftwareSerial.
    void addHumidtyToMessage(MessageHandler& msgHandler, SoftwareSerial& serial);
    
private:
    // Reads the temperature from the DHT sensor.
    float readTemperature();

    // Reads the humidity from the DHT sensor.
    float readHumidity();
    DHT dht;
    sensors_event_t event;
};

#endif
