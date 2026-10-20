// Arduino Uno - Sound sensor alternates green and red LEDs
// Each sound (clap): GREEN ON / RED OFF -> next sound: RED ON / GREEN OFF -> repeat
//
// Wiring:
//   Sound sensor: AO -> A0, DO -> D2, VCC -> 5V, GND -> GND
//   D11: Green LED, D9: Red LED
//     each pin -> LED long leg (+) / LED short leg (-) -> 220 ohm resistor -> GND

const byte SOUND_AO  = A0;
const byte SOUND_DO  = 2;
const byte GREEN_PIN = 11;
const byte RED_PIN   = 9;

const byte ACTIVE_LEVEL = LOW;          // Change to HIGH if your module is the opposite
const unsigned long COOLDOWN_MS = 250;  // Ignore sounds right after an event (ms)
const int PEAK_THRESHOLD = 10;          // AO loudness to count as a sound (lower = more sensitive)

bool greenTurn = true;                  // Which LED the next sound turns on
bool wasSound = false;                  // Previous sound state (DO or AO peak)
bool loud = false;                      // True when the last 20ms window was loud
unsigned long lastEvent = 0;            // Time of the last sound event
unsigned long eventCount = 0;           // Number of sound events

unsigned long windowAt = 0;             // Start of the current 20ms window
unsigned long lastReport = 0;           // Last Serial print time
int minimumAdc = 1023;
int maximumAdc = 0;
int peakToPeak = 0;                     // Loudness of the last 20ms window

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_DO, INPUT_PULLUP);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(RED_PIN, LOW);

  lastEvent = millis() - COOLDOWN_MS;
  windowAt = millis();
  Serial.println(F("Sound ready: clap to switch GREEN / RED"));
}

void loop() {
  const unsigned long now = millis();

  // Track min/max of the analog waveform
  const int raw = analogRead(SOUND_AO);
  minimumAdc = min(minimumAdc, raw);
  maximumAdc = max(maximumAdc, raw);

  // Peak-to-peak loudness every 20ms
  if (now - windowAt >= 20) {
    peakToPeak = maximumAdc - minimumAdc;
    loud = peakToPeak >= PEAK_THRESHOLD;
    minimumAdc = 1023;
    maximumAdc = 0;
    windowAt = now;
  }

  // A sound is either the DO comparator or a loud AO window
  const bool active = digitalRead(SOUND_DO) == ACTIVE_LEVEL;
  const bool sound = active || loud;

  // State change detection: react only when a sound starts
  if (sound && !wasSound && now - lastEvent >= COOLDOWN_MS) {
    lastEvent = now;
    eventCount++;

    digitalWrite(GREEN_PIN, greenTurn ? HIGH : LOW);
    digitalWrite(RED_PIN,   greenTurn ? LOW : HIGH);
    Serial.println(greenTurn ? F(">> GREEN ON, RED OFF") : F(">> RED ON, GREEN OFF"));
    greenTurn = !greenTurn;
  }
  wasSound = sound;

  if (now - lastReport >= 100) {
    lastReport = now;
    Serial.print(F("AO:"));     Serial.print(raw);
    Serial.print(F(" Peak:"));  Serial.print(peakToPeak);
    Serial.print(F(" DO:"));    Serial.print(active ? 1 : 0);
    Serial.print(F(" Count:")); Serial.println(eventCount);
  }
}
