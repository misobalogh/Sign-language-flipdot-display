#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include "constants.h"
#include "message_handler.h"

const char* ssid = "UPC4822489";
const char* password = "qx3pkYzkhrj4";

SoftwareSerial SerialRS485(5, 4);
WiFiServer server(80);

unsigned char address = 0xFF;
unsigned char command = 0x83;
MessageHandler msgHandler(address, command);

unsigned long blinkInterval = 500; 
unsigned long lastBlinkTime = 0;
bool underscoreVisible = false;  

void setup() {
    Serial.begin(9600);
    SerialRS485.begin(57600);

    // Begin connection to WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("TCP server started");
    msgHandler.sendMessage(SerialRS485); // Sends empty message to clear the display
}

void loop() {
    WiFiClient client = server.available(); 

    if (client) {
        uint8_t pos = 0;
        uint8_t line = 0;
        while (client.connected()) {
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

            if (client.available()) {
                String receivedData = client.readStringUntil('\n');  
                receivedData.trim();
                
                if (receivedData.length() == 0) {
                    continue;
                }

                char letter = receivedData.charAt(0);
                msgHandler.set_letter_at_position(get_segments_from_letter(letter), pos, line);

                pos = (pos + 1) % SYMBOLS_PER_LINE;
                if (pos == 0) {
                    line = (line + 1) % NUM_LINES;
                }

                Serial.print("Received: ");
                Serial.println(receivedData);
                msgHandler.sendMessage(SerialRS485);
            }
        }

        client.stop();
        Serial.println("Client Disconnected");
    }
}
