
#include <Arduino.h>
#include <IRremote.h>
#include "PinDefinitionsAndMore.h"


#define RELAY_PIN  6
#define TOGGLE_PIN 8

// debouncer
bool btnUp = true;

void setup()  {
  Serial.begin(9600);

  // pin setup
  pinMode(TOGGLE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // init ir
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
}

void loop() {

  // btn is on, set debouncer
  if (digitalRead(TOGGLE_PIN) == HIGH && btnUp) {
    Serial.println("BTN DN");
    btnUp = false;
  }

  // btn deressed 1st time, perform action
  if (digitalRead(TOGGLE_PIN) == LOW && !btnUp) {
    Serial.println("BTN UP");

    // toggle on/off based on current state
    digitalWrite(RELAY_PIN, (digitalRead(RELAY_PIN) == LOW) ? HIGH : LOW);
    btnUp = true;
  }


  // picked up ir signal
  if (IrReceiver.decode()) {
    
    // buffer overflowed
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
      Serial.println("IR code too long. Edit IRremoteInt.h and increase RAW_BUFFER_LENGTH");
      
    } else {
//      Serial.print("=-= PROTOCOL: ");
//      Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
//      Serial.print(" =-= ADDR: 0x");
//      Serial.print(IrReceiver.decodedIRData.address, HEX);
//      Serial.print(" =-= CMD: 0x");
//      Serial.print(IrReceiver.decodedIRData.command, HEX);
//      Serial.print(" =-= RAW: 0x");
//      Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
//      Serial.print(" =-= ");           
//      Serial.println();
//      IrReceiver.printIRResultShort(&Serial);


      if (IrReceiver.decodedIRData.address == 0x0) {

        // toggle on/off
        if (IrReceiver.decodedIRData.command == 0x46) {
          digitalWrite(RELAY_PIN, (digitalRead(RELAY_PIN) == LOW) ? HIGH : LOW);

        // turn off
        } else if (IrReceiver.decodedIRData.command == 0x45) {
          digitalWrite(RELAY_PIN, LOW);

        // turn on
        } else if (IrReceiver.decodedIRData.command == 0x47) {
          digitalWrite(RELAY_PIN, HIGH);
        }
      }
    }

    IrReceiver.resume();
  }
}
