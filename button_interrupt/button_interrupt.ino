const int BUTTON_PIN = 2;    // Button (D2 <-> GND), D2 = interrupt pin INT0
const int LED_PIN    = A5;   // LED (A5 -> 220 ohm resistor -> LED -> GND)

const unsigned long DEBOUNCE_MS = 50;  // Debounce time (ms)

// Variables shared with the ISR must be volatile
volatile bool ledState = false;           // LED state
volatile int buttonState = HIGH;          // Last accepted button state
volatile unsigned long lastIsrTime = 0;   // Time of the last accepted edge
volatile bool changed = false;            // Set by the ISR, cleared by loop()

// ISR: runs on every edge (press and release)
void onButtonChange() {
  unsigned long now = millis();
  int state = digitalRead(BUTTON_PIN);

  // Ignore bounces and edges that did not change the state
  if (now - lastIsrTime > DEBOUNCE_MS && state != buttonState) {
    lastIsrTime = now;
    buttonState = state;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    changed = true;
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Press or release the button to toggle the LED");

  // CHANGE = trigger on both FALLING (press) and RISING (release)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonChange, CHANGE);
}

void loop() {
  // Serial must not be used inside the ISR, so print here
  if (changed) {
    noInterrupts();
    changed = false;
    int state = buttonState;
    bool led = ledState;
    interrupts();

    Serial.print(state == LOW ? "Pressed" : "Released");
    Serial.print(" -> LED ");
    Serial.println(led ? "ON" : "OFF");
  }
}
