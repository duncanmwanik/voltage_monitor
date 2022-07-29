#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "lora_receiver.h"
#include "display.h"


//screen
#include <Arduino.h>
#include "OLED_Driver.h"
#include "GUI_Paint.h"
#include "DEV_Config.h"
#include "Debug.h"

// LoRa
#define ss 15
#define rst 18
#define dio0 4
#define dio1 5

byte sender;
String incoming;

const int number_of_transmitters = 10;

typedef struct 
{
  int id ;
  int voltage_data;
  long time_saved_data;
}mydata;

mydata received_data [number_of_transmitters];


// Lora packet wait period
unsigned long timeNow = 0;
bool loraReceived = false;

byte localAddress = 0xFE; // address of this device //254 IN DECIMAL

void setup()
{
  // initialize Serial Monitor
  Serial.begin(115200);

  // setup LoRa
  Serial.println("Initializing LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);

  // replace the LoRa.begin(---E-) argument with your location's frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(866E6))
  {
    Serial.print(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  // setup OLED
  setupRGBOLED();
}

void onReceive(int packetSize)
{
  if (packetSize == 0)
    return; // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();       // recipient address
  sender = LoRa.read();              // sender address
  byte incomingMsgId = LoRa.read();  // incoming msg ID
  byte incomingLength = LoRa.read(); // incoming msg length

  incoming = "";

  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length())
  { // check length for error
    Serial.println("error: message length does not match length");
    return; // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF)
  {
    Serial.println("This message is not for me.");
    return; // skip rest of function
  }

  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  loraReceived = true;

  /********************************/
  //FORMAT OF STORING THE DATA FROM DIFFERENT RECEIVERS SO AS TO ENABLE SQUENTIAL STORAGE(SAVING) AND RETRIVAL FOR DISPLAY
  //THE SENDERS ID WILL DETERMINE WHERE THE DATA IS STORED IN AN ARRAY
  //strongly recommend we use ID of the transmitter from 0x01 (1 in decimal) to 0xF0 
  //ensure to update the number of transmitter variable above
//retrieving data from lora
  int id_data = sender; //sender ID in integer form
  int voltage_received = incoming.toInt();
  long current_time = millis();

//saaving data to array

   received_data[id_data - 1] = (mydata) {id_data, voltage_received , current_time}; //data saved in the array now..position in the array == ID of the transmitter

}

void loop()
{
  onReceive(LoRa.parsePacket());
  
  String sender_ = "0x" + String(sender, HEX);

  // display data
  for (int i = 0; i<= number_of_transmitters; i++){
    display_data ();
    delay(5000); //delay for 5secs
  }
}

void display_data ()
{
  // TODO : CHECK IF THERE IS A VOLTAG MEASUREMENT ABOVE ZERO...DISPLAY CHECK CONNECTION ON THE NEXT LINE
    // TODO : CHECK THAT THE DATA PRESENTED IS NOT OVER 10MIN OLD OTHER WISE DISPLAY STALE ON THE NEXT LINE
    // TODO : IF ALL IS OKAY DISPLAY DATA
       if(received_data->voltage_data != 0){
         OLED_1in5_rgb_Clear();
         Paint_DrawString_EN(5,50, " ID  |", &Font16, BLACK, BLUE);
         Paint_DrawString_EN(5, 20, String(received_data->id).c_str() , &Font16, BLACK, BLUE);
         
         Paint_DrawString_EN(25, 5, "BAT  |", &Font16, BLACK, BLUE);
         Paint_DrawNum(25, 40, String(received_data->voltage_data).c_str(), &Font16, 2, RED, BLACK);
         Paint_DrawNum(25, 70, "V", &Font16, 2, RED, BLACK);
         // Driver_Delay_ms(100);
         if ((millis() - received_data->time_saved_data) > 600000)
         {
          Paint_DrawString_EN(40, 5, "STALE", &Font16, BLACK, BLUE);
         }
         
       }
       else {
         OLED_1in5_rgb_Clear();
         Paint_DrawString_EN(5,50, " ID  |", &Font16, BLACK, BLUE);
         Paint_DrawString_EN(5, 20, String(received_data->id).c_str() , &Font16, BLACK, BLUE);

         Paint_DrawString_EN(25, 5, " CHECK CONNECTION ", &Font16, BLACK, BLUE);
          // Driver_Delay_ms(100);       
       }
      
}