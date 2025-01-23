// Author: Michal Balogh, xbalog06
// Date: 14-10-2024

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include "constants.h"
#include "MessageHandler.h"
#include "TimeDisplay.h"
#include "text_animations.h"
#include "DHTSensor.h"

const char* ssid = ""; // Add WiFi SSID
const char* password = ""; // Add WiFi password

#define SERIAL_RX D3
#define SERIAL_TX D2

SoftwareSerial SerialRS485(SERIAL_RX, SERIAL_TX);
WiFiServer server(80);

unsigned char address = 0xFF;
unsigned char command = 0x83;
MessageHandler msgHandler(address, command);

unsigned long blinkInterval = 500; 
unsigned long lastBlinkTime = 0;
bool underscoreVisible = false;

TimeDisplay timeDisplay("pool.ntp.org", 7200, 60000);

#define DHT_PIN D5
#define DHT_TYPE DHT11

DHTSensor dhtSensor(DHT_PIN, DHT_TYPE);

void setup() {
    // Serial for debugging
    Serial.begin(9600);

    // Serial for RS485 communication (connected to 4x7 segment display)
    SerialRS485.begin(57600);

    // Begin connection to WiFi
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Debug prints
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("TCP server started");

    // Clear the display
    msgHandler.clear(SerialRS485);

    // Disply welcome message and initialize IDLE state
    rollingLettersAnimation("HELLO", msgHandler, SerialRS485);
    delay(2000);
    msgHandler.clear(SerialRS485);
    timeDisplay.begin();
}

void loop() {
    // Check if a client is connected
    WiFiClient client = server.accept();

    if (client) {
        msgHandler.clear(SerialRS485);

        // Start at the beginning of the display
        uint8_t pos = 0;
        uint8_t line = 0;
        while (client.connected()) {
            // Blinking underscore animation
            unsigned long currentTime = millis();
                if (currentTime - lastBlinkTime >= blinkInterval) {
                    lastBlinkTime = currentTime; 
                    underscoreVisible = !underscoreVisible; 
        
                    if (underscoreVisible) {
                        msgHandler.set_letter_at_position(UNDERSCORE, pos, line);
                    } else {
                        msgHandler.set_letter_at_position(SPACE, pos, line);
                    }

                    msgHandler.sendMessage(SerialRS485);
                }

            // Check if there is data available to read from the client
            if (client.available()) {
                String receivedData = client.readStringUntil('\n');  
                receivedData.trim();
                
                if (receivedData.length() == 0) {
                    continue;
                }

               // Set the message
                for (unsigned i = 0; i < receivedData.length(); i++) {
                    char letter = receivedData.charAt(i);
                    // msgHandler.set_letter_at_position(get_segments_from_letter(letter), pos, line);
                    msgHandler.animate_letter(get_segments_from_letter(letter), pos, line, SerialRS485);

                    // Move to the next position on the display
                    pos = (pos + 1) % SYMBOLS_PER_LINE;
                    if (pos == 0) {
                        line = (line + 1) % NUM_LINES;
                    }

                    // Debug prints
                    Serial.print("Writing: ");
                    Serial.println(letter);

                    // msgHandler.sendMessage(SerialRS485);
                    // delay(200);
                }
        }
        
        client.stop();
        Serial.println("Client Disconnected");

        msgHandler.clear(SerialRS485);

        // Initialize IDLE state
        timeDisplay.addTimeToMessage(msgHandler);
        dhtSensor.addTemperatureToMessage(msgHandler, SerialRS485);
        dhtSensor.addHumidtyToMessage(msgHandler, SerialRS485);
        msgHandler.sendMessage(SerialRS485);
    }
    else { // IDLE state
        dhtSensor.addTemperatureToMessage(msgHandler, SerialRS485);
        dhtSensor.addHumidtyToMessage(msgHandler, SerialRS485);
        if (timeDisplay.needUpdate()) {
            timeDisplay.addTimeToMessage(msgHandler);
        }
        msgHandler.sendMessage(SerialRS485);
    }
    delay(250);
}
