
#define RECORD_GAP_MICROS 19000


#include <Arduino.h>
#include <CuteBuzzerSounds.h>
#include <IRremote.h>

#include "PinDefinitionsAndMore.h"


#define RELAY1_PIN 6
#define RELAY2_PIN 7
#define TOGGLE_PIN 8


// debouncer
bool btnUp = true;

// repeat / history
const unsigned long repeatDuration = 95250;
unsigned int prevMillis = 0;
unsigned int prevCmd = 0x0;


void setup()  {
  Serial.begin(9600);

  // pin setup
  pinMode(TOGGLE_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
//  cute.init(BUZZER_PIN);

  // init ir
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
}

void loop() {

  // btn is pressed, set debouncer
  if (digitalRead(TOGGLE_PIN) == HIGH && btnUp) {
    Serial.println("BTN DN");
    btnUp = false;
  }

  // btn depress 1st time, perform action
  if (digitalRead(TOGGLE_PIN) == LOW && !btnUp) {
    Serial.println("BTN UP");

//    cute.play(S_BUTTON_PUSHED);

    // toggle on/off based on current state
    if (digitalRead(RELAY1_PIN) == LOW) {
      digitalWrite(RELAY1_PIN, HIGH);
      digitalWrite(RELAY2_PIN, HIGH);
    
    } else {
      digitalWrite(RELAY1_PIN, LOW);
      digitalWrite(RELAY2_PIN, LOW);
    }

    
//    digitalWrite(RELAY1_PIN, (digitalRead(RELAY1_PIN) == LOW) ? HIGH : LOW);
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
      IrReceiver.printIRResultShort(&Serial);


      // BASIC remote
      if (IrReceiver.decodedIRData.address == 0x0) {
  
        // toggle on/off
        if (IrReceiver.decodedIRData.command == 0x46) {
          Serial.println(F("PWR TOGGLE"));
         
          // repeat cmd, act if interval passed
//          if (prevCmd != IrReceiver.decodedIRData.command || millis() > prevMillis + repeatDuration) {
            digitalWrite(RELAY2_PIN, (digitalRead(RELAY2_PIN) == LOW) ? HIGH : LOW);
//            cute.play((digitalRead(RELAY2_PIN) == LOW) ? S_CONNECTION : S_DISCONNECTION);
//          }
      
        // turn off
        } else if (IrReceiver.decodedIRData.command == 0x45) {
          Serial.println(F("TURN OFF"));
          digitalWrite(RELAY2_PIN, LOW);
//          cute.play(S_DISCONNECTION);
      
        // turn on
        } else if (IrReceiver.decodedIRData.command == 0x47) {
          Serial.println(F("TURN ON"));
          digitalWrite(RELAY2_PIN, HIGH);
//          cute.play(S_CONNECTION);
        }
        
  
      // SAMSUNG remote
      } else if (IrReceiver.decodedIRData.address == 0x707) {
  
        // toggle on/off (rec)
        if (IrReceiver.decodedIRData.command == 0x49) {
          Serial.println(F("PWR TOGGLE"));
          
          // repeat cmd, act if interval passed
//          if (prevCmd != IrReceiver.decodedIRData.command || millis() > prevMillis + (repeatDuration * 5)) {
            digitalWrite(RELAY2_PIN, (digitalRead(RELAY2_PIN) == LOW) ? HIGH : LOW);
//            cute.play((digitalRead(RELAY2_PIN) == LOW) ? S_CONNECTION : S_DISCONNECTION);
//          }
      
        // turn off (prev)
        } else if (IrReceiver.decodedIRData.command == 0x45) {
          Serial.println(F("TURN OFF"));
          digitalWrite(RELAY2_PIN, LOW);
//          cute.play(S_DISCONNECTION);
      
        // turn on (next)
        } else if (IrReceiver.decodedIRData.command == 0x48) {
          Serial.println(F("TURN ON"));
          digitalWrite(RELAY2_PIN, HIGH);
//          cute.play(S_CONNECTION);
        }
      }
  
      // update last
      prevCmd = IrReceiver.decodedIRData.command;
      prevMillis = millis();
    }
  }

  // re-enable receiving
  IrReceiver.resume();
} 
