// Arduino Uno - Blink (0.001s)
// Blinks the on-board LED (marked "L", connected to D13) every 0.001 seconds.

const int LED_PIN = LED_BUILTIN;  // On-board LED (D13)

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);    // LED on
  delay(1);                       // Wait 0.001s
  digitalWrite(LED_PIN, LOW);     // LED off
  delay(1);                       // Wait 0.001s
}
