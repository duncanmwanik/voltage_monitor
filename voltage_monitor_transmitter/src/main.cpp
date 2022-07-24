#include <Arduino.h>
#include "lora_transmit.h"
#include "monitor.h"
#include "oled_display.h"

void setup()
{
  // initialize Serial Monitor
  Serial.begin(115200);

  // setup OLED
  setupOLED();

  // setup the INA226 battery monitor
  setupINA226();

  // setup Lora
  setupLora();

}

void loop()
{
  // get voltage value
  float voltage = checkVoltage();

  // send voltage value
  sendData(voltage);
  
  // show voltage value
  displayData(voltage);

  // delay(100);
}