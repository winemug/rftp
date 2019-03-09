/*
   RadioLib SX127x Receive Example

   This example listens for LoRa transmissions using SX127x Lora modules.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
    - preamble length

   Other modules from SX127x/RFM9x family can also be used.
*/

// include the library
#include <Arduino.h>
#include <RadioLib.h>

// SX1278 module is in slot A on the shield
SX1278 sx1278 = new Module(3, 1, 0);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings

  Serial.print(F("[SX1278] Initializing ... "));
  int state = sx1278.beginFSK(433.91F, 40.625F, 26.37F, 125.0F, 17, 200 ,0.3F);

  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
    byte byteArr[8];
    int state = sx1278.receive(byteArr, 8);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = lora.receive(byteArr, 8);
  */

  if (state == ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    // Serial.print("[SX1278] Data:\t\t");
    // Serial.println(byteArr);

    // print the RSSI (Received Signal Strength Indicator)
    // of the last received packet
    Serial.print("[SX1278] RSSI:\t\t");
    Serial.print(sx1278.getRSSI());
    Serial.println(" dBm");

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print("[SX1278] SNR:\t\t");
    Serial.print(sx1278.getSNR());
    Serial.println(" dBm");

    // print frequency error
    // of the last received packet
    Serial.print("Frequency error:\t");
    Serial.print(sx1278.getFrequencyError());
    Serial.println(" Hz");

  } else if (state == ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state == ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  }
}
