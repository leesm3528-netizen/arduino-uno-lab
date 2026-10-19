// Arduino Uno button read: prints the A0 button state to the Serial Monitor
// Uses the internal pull-up -> wire one side of the button to A0, the other to GND

const int BUTTON_PIN = A0;

const unsigned long DEBOUNCE_TIME = 50;  // debounce time (ms)

int lastState = HIGH;
unsigned long lastChangeTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int state = digitalRead(BUTTON_PIN);  // LOW when pressed, HIGH when released

  if (state != lastState && millis() - lastChangeTime > DEBOUNCE_TIME) {
    lastChangeTime = millis();
    lastState = state;

    if (state == LOW) {
      Serial.println("Button pressed");
    } else {
      Serial.println("Button released");
    }
  }
}
