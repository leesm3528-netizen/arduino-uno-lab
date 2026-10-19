const int BUTTON_PIN = A0;   // Button (A0 <-> GND)
const int LED_PIN    = A5;   // LED (A5 -> 220 ohm resistor -> LED -> GND)

const unsigned long DEBOUNCE_MS = 50;  // Debounce time (ms)

int lastReading = HIGH;      // Previous value (read in the last loop)
int stableState = HIGH;      // Confirmed button state
unsigned long lastChangeTime = 0;  // Time of the last value change

int pressCount = 0;          // Number of presses
bool ledState = false;       // LED state

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Press the button!");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);   // Current value

  // 1) If current value differs from previous value -> record the change time
  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  // 2) If the value stays the same for DEBOUNCE_MS -> accept it as a real input
  if (millis() - lastChangeTime > DEBOUNCE_MS) {
    if (reading != stableState) {
      stableState = reading;

      // 3) Count only at the moment of pressing (HIGH -> LOW)
      if (stableState == LOW) {
        pressCount++;
        Serial.print("Button press count: ");
        Serial.println(pressCount);

        // 4) Toggle the LED every 5 presses
        if (pressCount % 5 == 0) {
          ledState = !ledState;
          digitalWrite(LED_PIN, ledState);
          Serial.print(">> LED ");
          Serial.println(ledState ? "ON" : "OFF");
        }
      }
    }
  }

  // 5) Save the current value as the previous value (for the next loop)
  lastReading = reading;
}
