// Arduino Uno - Car traffic light with an ON/OFF push button
// Repeats Green (0.5s) -> Yellow (0.5s) -> Red (0.5s)
// Press the button: traffic light OFF (all LEDs off)
// Press again:      traffic light ON  (starts from green)
//
// Wiring:
//   D8: Red LED, D9: Yellow LED, D10: Green LED
//     each pin -> LED long leg (+) / LED short leg (-) -> 220 ohm resistor -> GND
//   D2: Button (one leg -> D2, diagonal leg -> GND), uses internal pull-up

const int RED_PIN    = 8;
const int YELLOW_PIN = 9;
const int GREEN_PIN  = 10;
const int BUTTON_PIN = 2;

const unsigned long GREEN_TIME  = 500;   // Green light time (0.5s)
const unsigned long YELLOW_TIME = 500;   // Yellow light time (0.5s)
const unsigned long RED_TIME    = 500;   // Red light time (0.5s)
const unsigned long DEBOUNCE_MS = 50;    // Ignore button bounces (ms)

// Light states
const int STATE_GREEN  = 0;
const int STATE_YELLOW = 1;
const int STATE_RED    = 2;

int currentState = STATE_GREEN;          // Current light
unsigned long stateStartTime = 0;        // Time the current light turned on
bool systemOn = true;                    // Traffic light ON/OFF

int lastReading = HIGH;                  // Previous button value
int stableState = HIGH;                  // Confirmed button state
unsigned long lastChangeTime = 0;        // Time of the last button value change

// Turn each LED on or off
void setLeds(int red, int yellow, int green) {
  digitalWrite(RED_PIN, red);
  digitalWrite(YELLOW_PIN, yellow);
  digitalWrite(GREEN_PIN, green);
}

// Switch to a light state and restart its timer
void changeState(int state) {
  currentState = state;
  stateStartTime = millis();

  if (state == STATE_GREEN) {
    setLeds(LOW, LOW, HIGH);
    Serial.println("GREEN  - Go");
  } else if (state == STATE_YELLOW) {
    setLeds(LOW, HIGH, LOW);
    Serial.println("YELLOW - Caution");
  } else {
    setLeds(HIGH, LOW, LOW);
    Serial.println("RED    - Stop");
  }
}

// Returns true once at the moment the button is pressed
bool buttonPressed() {
  int reading = digitalRead(BUTTON_PIN);
  bool pressed = false;

  if (reading != lastReading) {
    lastChangeTime = millis();
  }
  if (millis() - lastChangeTime > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    pressed = (stableState == LOW);
  }

  lastReading = reading;
  return pressed;
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  Serial.println("SYSTEM ON");
  changeState(STATE_GREEN);
}

void loop() {
  // Toggle the whole traffic light on each press
  if (buttonPressed()) {
    systemOn = !systemOn;
    if (systemOn) {
      Serial.println("SYSTEM ON");
      changeState(STATE_GREEN);
    } else {
      Serial.println("SYSTEM OFF");
      setLeds(LOW, LOW, LOW);
    }
  }

  // While OFF, skip the cycle
  if (!systemOn) {
    return;
  }

  unsigned long elapsed = millis() - stateStartTime;

  // Move to the next light when the current one's time is up
  if (currentState == STATE_GREEN && elapsed >= GREEN_TIME) {
    changeState(STATE_YELLOW);
  } else if (currentState == STATE_YELLOW && elapsed >= YELLOW_TIME) {
    changeState(STATE_RED);
  } else if (currentState == STATE_RED && elapsed >= RED_TIME) {
    changeState(STATE_GREEN);
  }
}
