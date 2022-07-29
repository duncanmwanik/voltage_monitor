
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// LoRa
#define ss 15
#define rst 18
#define dio0 4
#define dio1 5

byte msgCount = 0;            // count of outgoing messages
//ENSURE TO CUSTOMIZE THIS VARIABLE FOR EACH TRANSMITTER BOARD
byte localAddress = 0x01;     // address of this device //ALL TRSMITTING DEICES WILL HOLD ADDRESSES 0X01 - 0XF9
///////////////////////

byte mainBoardAddress = 0xFE; // destination to send to // ADDRESS OF THE RECIVER(MAIN BOARD) IS 0XFE (254 IN DECIMAL)

void setupLora()
{
  Serial.println("Initializing LoRa Transmitter");

  // setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  // replace the LoRa.begin(---E-) argument with your location's frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(866E6))
  {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void sendData(int voltage)
{
  Serial.print("Sending packet: ");
  Serial.print(voltage);

  // Send LoRa packet to receiver
  LoRa.beginPacket();                   // start packetl
  LoRa.write(mainBoardAddress);         // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(String(voltage).length()); // add payload length
  LoRa.print(voltage);                  // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;

}

String get_device_id(){
  return String(localAddress);
}
