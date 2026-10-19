// Arduino Uno - LED brightness control with a potentiometer
//
// Wiring:
//   Potentiometer: left leg -> 5V, middle leg (wiper) -> A0, right leg -> GND
//   LED: D9 -> 220 ohm resistor -> LED long leg (+) / LED short leg (-) -> GND
//   D9 must be a PWM pin (marked with ~ : 3, 5, 6, 9, 10, 11)

const int POT_PIN = A0;      // Potentiometer wiper
const int LED_PIN = 9;       // PWM pin

const unsigned long PRINT_INTERVAL = 200;  // Serial print interval (ms)
unsigned long lastPrintTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT_PIN);              // 0 ~ 1023
  int brightness = map(potValue, 0, 1023, 0, 255); // 0 ~ 255 for PWM

  analogWrite(LED_PIN, brightness);

  // Print without delay() so the LED responds immediately
  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis();
    Serial.print("Pot: ");
    Serial.print(potValue);
    Serial.print("  Brightness: ");
    Serial.print(brightness);
    Serial.print("  (");
    Serial.print(brightness * 100 / 255);
    Serial.println("%)");
  }
}
