// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include "DHTSensor.h"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type) : dht(pin, type) {
}

void DHTSensor::begin() {
    dht.begin();
    Serial.println("DHT sensor initialized");
}

float DHTSensor::readTemperature() {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
        Serial.println("Error reading temperature!");
        return NAN;
    } else {
        return temp;
    }
}

float DHTSensor::readHumidity() {
    float hum = dht.readHumidity();
    if (isnan(hum)) {
        Serial.println("Error reading humidity!");
        return NAN;
    } else {
        return hum;
    }
}

void DHTSensor::addTemperatureToMessage(MessageHandler& msgHandler, SoftwareSerial& serial) {
    float temperature = readTemperature();
    if (!isnan(temperature)) {
        int tens = temperature / 10;
        int ones = (int)temperature % 10;
        // Set message in format "t__xx"
        msgHandler.set_letter_at_position(t, 0, 2);
        msgHandler.set_letter_at_position(get_segments_from_digit(tens), 3, 2);
        msgHandler.set_letter_at_position(get_segments_from_digit(ones), 4, 2);
    }
}

void DHTSensor::addHumidtyToMessage(MessageHandler& msgHandler, SoftwareSerial& serial) {
    float humidity = readHumidity();
    if (!isnan(humidity)) {
        int tens = humidity / 10;
        int ones = (int)humidity % 10;
        // Set message in format "hu_xx"
        msgHandler.set_letter_at_position(h, 0, 3);
        msgHandler.set_letter_at_position(u, 1, 3);
        msgHandler.set_letter_at_position(get_segments_from_digit(tens), 3, 3);
        msgHandler.set_letter_at_position(get_segments_from_digit(ones), 4, 3);
    }
}