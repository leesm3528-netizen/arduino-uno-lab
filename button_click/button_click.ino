const int BUTTON_PIN = A0;   // Button (A0 <-> GND)
const int LED_PIN    = A5;   // LED (A5 -> 220 ohm resistor -> LED -> GND)

const unsigned long DEBOUNCE_MS     = 50;    // Debounce time (ms)
const unsigned long DOUBLE_CLICK_MS = 300;   // Max gap between two clicks (ms)
const unsigned long LONG_PRESS_MS   = 1000;  // Hold time for a long press (ms)

int lastReading = HIGH;      // Previous value (read in the last loop)
int stableState = HIGH;      // Confirmed button state
unsigned long lastChangeTime = 0;  // Time of the last value change

unsigned long pressStartTime = 0;   // Time the button was pressed
unsigned long lastReleaseTime = 0;  // Time the button was released
int clickCount = 0;                 // Short clicks waiting to be judged
bool longPressHandled = false;      // True once the current press became a long press

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Double click: LED ON / Long press: LED OFF / Single click: nothing");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);   // Current value
  unsigned long now = millis();

  // 1) If current value differs from previous value -> record the change time
  if (reading != lastReading) {
    lastChangeTime = now;
  }

  // 2) If the value stays the same for DEBOUNCE_MS -> accept it as a real input
  if (now - lastChangeTime > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;

    if (stableState == LOW) {
      // 3) Pressed (HIGH -> LOW): start measuring the hold time
      pressStartTime = now;
      longPressHandled = false;
    } else if (!longPressHandled) {
      // 4) Released (LOW -> HIGH) before a long press: count a short click
      clickCount++;
      lastReleaseTime = now;

      if (clickCount == 2) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Double click -> LED ON");
        clickCount = 0;
      }
    }
  }

  // 5) Still held for LONG_PRESS_MS -> long press (fires once per press)
  if (stableState == LOW && !longPressHandled && now - pressStartTime >= LONG_PRESS_MS) {
    longPressHandled = true;
    clickCount = 0;
    digitalWrite(LED_PIN, LOW);
    Serial.println("Long press -> LED OFF");
  }

  // 6) One click and no second click in time -> single click, do nothing
  if (clickCount == 1 && stableState == HIGH && now - lastReleaseTime > DOUBLE_CLICK_MS) {
    clickCount = 0;
    Serial.println("Single click -> ignored");
  }

  // 7) Save the current value as the previous value (for the next loop)
  lastReading = reading;
}
