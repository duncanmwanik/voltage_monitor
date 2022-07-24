#include <Arduino.h>
#include "lora_receiver.h"
#include "display.h"

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  // setup LoRa
  setupLoRa();
}

void loop() {
  // get received data
  String voltage = getData();

  // display data
}