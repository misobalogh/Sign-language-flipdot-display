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
        while (client.connected()) {
            if (client.available()) {
                String receivedData = client.readStringUntil('\n');  
                receivedData.trim();  

                if (receivedData.length() == 0) {
                    continue;
                }

                char letter = receivedData.charAt(0);
                msgHandler.set_letter_at_position(letter, 0, 0);

                Serial.print("Received: ");
                Serial.println(receivedData);
                msgHandler.sendMessage(SerialRS485);
            }
        }

        client.stop();
        Serial.println("Client Disconnected");
    }
}
