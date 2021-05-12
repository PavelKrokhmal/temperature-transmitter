#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <GyverTM1637.h>

#define CLK 2
#define DIO 3

GyverTM1637 disp(CLK, DIO);

RF24 radio(9, 10); // For UNO, NANO
//RF24 radio(9,53); // For MEGA

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // Pipes

byte counter;

void setup() {
  // Serial.begin(9600);

  disp.clear();
  disp.brightness(2);

  radio.begin();

  // Confirm reception mode: 1 - on, 0 - off
  radio.setAutoAck(1); 

  // Time between the attempt to reach out and the number of attempts
  radio.setRetries(0, 15); 
  
  // Allow sending data in response to an incoming signal
  radio.enableAckPayload(); 
  
  // Package bity size   
  radio.setPayloadSize(32); 

  radio.openWritingPipe(address[0]); // Pipe
  radio.setChannel(0x60); // Channel

  // Transiver power level: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setPALevel (RF24_PA_HIGH);  
  
  // Exchange rate: RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // At the lowest - hight sensitivity and distant 
  // Must be the same s on the receiver
  radio.setDataRate (RF24_1MBPS); 

  radio.powerUp(); // Start
  radio.stopListening();
}

void loop() {
  radio.write(&counter, sizeof(counter));
  disp.displayInt(counter);
  counter++;
  
  delay(1000);
}
