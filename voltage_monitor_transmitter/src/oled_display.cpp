#include <Arduino.h>
#include "lora_transmit.h"
#include "SSD1306.h"
#define SDA_PIN 21 // GPIO21 -> SDA
#define SCL_PIN 22 // GPIO22 -> SCL
#define SSD_ADDRESS 0x3c

SSD1306 display(SSD_ADDRESS, SDA_PIN, SCL_PIN);

void setupOLED()
{
    Serial.println("");
    Serial.println("Setting up OLED Display");
    Serial.println("");

    display.init();
    // display.flipScreenVertically();
    // display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void displayData(float voltage)
{    
    String ID = get_device_id();
    display.clear(); // clear the display
    // DISPLAY THE VOLTAVE OF BAATTERY
    display.drawString(0, 0, "  BAT | ");
    display.drawString(40, 0, String(voltage));
    display.drawString(70, 0, "V");

    //DISPLAY THE ID OF THE BOARD
    display.drawString(0, 20, "My ID | ");
    display.drawString(40, 20,ID);
    
    display.display(); // write the buffer to the display
    delay(10);
}