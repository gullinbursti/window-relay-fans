
#define RELAY_PIN  6
#define TOGGLE_PIN 8

// debouncer
bool btnUp = true;

void setup()  {
  Serial.begin(9600);

  // pin setup
  pinMode(TOGGLE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {

  // btn is on, set debouncer
  if (digitalRead(TOGGLE_PIN) == HIGH && btnUp) {
    Serial.println("BTN DN");
    Serial.println(digitalRead(RELAY_PIN));
    btnUp = false;
  }

  // btn deressed 1st time, perform action
  if (digitalRead(TOGGLE_PIN) == LOW && !btnUp) {
    Serial.println("BTN UP");

    // toggle on/off based on current state
    digitalWrite(RELAY_PIN, (digitalRead(RELAY_PIN) == LOW) ? HIGH : LOW);
    Serial.println(digitalRead(RELAY_PIN));
    btnUp = true;
  }
}
