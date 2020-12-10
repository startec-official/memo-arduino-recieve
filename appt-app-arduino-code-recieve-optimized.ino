/*
 * 
 * MESSAGING ENGINE FOR MEDICAL ORGANIZATIONS(MEMO) v1.0.0
 * ARDUINO CODE V1.0.0
 * 
 * 
  AUTHORED BY: GABRIEL O. PONON
  STARTEC INNOVATIONS
  
  MIT LICENSE, VISIBLE AUTHOR ATTRIBUTION
  WRITTEN ON: 10/05/2020

  CODEBASE:
  
  PROGMEM string demo
  RETRIEVED FROM: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
  ADDITIONAL REFERENCE: http://www.nongnu.org/avr-libc/user-manual/pgmspace.html

  GSM Interfacing Code
  RETRIEVED FROM: E-Gizmo Mechatronix (https://www.e-gizmo.net/)
  DERIVED FROM: SEEDSTUDIO (https://www.seeedstudio.com)
  
*/

#include <avr/pgmspace.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <GPRS_Shield_Arduino.h>
#include <sim900.h>
#include <MemoryFree.h>
#include <pgmStrToRAM.h>

char buffer[90];  // make sure this is large enough for the largest string it must hold
char inputString[100];
int input_string_char_ct = 0;
boolean stringComplete = false;

#define MESSAGE_LENGTH 160

int messageIndex = 0;
char phone[16];
char datetime[24];
char incoming_message[MESSAGE_LENGTH];

GPRS GSMDEVICE(2,3,9600);//RX,TX,BAUDRATE

void setup() {
  Serial.begin(9600);
  while (!Serial);  // wait for serial port to connect. Needed for native USB
  Serial.println(F("SCS"));
  Serial.println(F("IB"));
  while(!GSMDEVICE.init()) {
    delay(1000);
    Serial.println(F("IW"));
  }
  Serial.println(F("IS"));
}

void loop() {
  messageIndex = GSMDEVICE.isSMSunread();
  if (messageIndex > 0) {
    GSMDEVICE.readSMS(messageIndex, incoming_message, MESSAGE_LENGTH, phone, datetime);
    Serial.print("RR"); // tolerate small heap hole bytes for better performance
    Serial.print(phone);
    Serial.print(";");
    Serial.println(incoming_message); // TODO : check for spam...
    GSMDEVICE.deleteSMS(messageIndex);
    delay(200);
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    inputString[input_string_char_ct] = inChar;
    input_string_char_ct ++;
    if (inChar == '\n') {
      inputString[input_string_char_ct] = '\0';
      input_string_char_ct ++;
      stringComplete = true;
    }
  }
}
