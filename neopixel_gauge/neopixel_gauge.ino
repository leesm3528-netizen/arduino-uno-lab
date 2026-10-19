// Arduino Uno - NeoPixel gauge with a potentiometer
// Turn the knob to fill the 8-pixel gauge (green -> yellow -> red)
//
// Wiring:
//   NeoPixel (8): DIN -> D10, 5V (+) -> 5V, GND (-) -> GND
//   Potentiometer: left leg -> 5V, middle leg (wiper) -> A0, right leg -> GND

#include <Adafruit_NeoPixel.h>

const int PIXEL_PIN  = 10;   // NeoPixel data pin
const int NUM_PIXELS = 8;    // Number of pixels
const int POT_PIN    = A0;   // Potentiometer wiper

const unsigned long PRINT_INTERVAL = 200;  // Serial print interval (ms)

Adafruit_NeoPixel strip(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int lastLevel = -1;                // Previous gauge level (to update only on change)
unsigned long lastPrintTime = 0;

void setup() {
  strip.begin();
  strip.setBrightness(80);         // Overall brightness (0 ~ 255)
  strip.show();                    // Start with all pixels off
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT_PIN);  // 0 ~ 1023

  // Gauge level: 0 ~ NUM_PIXELS * 255 (each pixel has 255 steps)
  int level = map(potValue, 0, 1023, 0, NUM_PIXELS * 255);

  // Redraw only when the level changes
  if (level != lastLevel) {
    lastLevel = level;

    int fullPixels = level / 255;   // Fully lit pixels
    int partial    = level % 255;   // Brightness of the next pixel (0 ~ 254)

    for (int i = 0; i < NUM_PIXELS; i++) {
      // Color by position: pixel 0 = green, last pixel = red
      int red   = i * 255 / (NUM_PIXELS - 1);
      int green = 255 - red;

      if (i < fullPixels) {
        strip.setPixelColor(i, strip.Color(red, green, 0));
      } else if (i == fullPixels) {
        // Cast to long to avoid int overflow: 255 * 254 > 32767
        strip.setPixelColor(i, strip.Color(red * (long)partial / 255, green * (long)partial / 255, 0));
      } else {
        strip.setPixelColor(i, 0);  // Off
      }
    }
    strip.show();
  }

  if (millis() - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = millis();
    Serial.print("Pot: ");
    Serial.print(potValue);
    Serial.print("  Gauge: ");
    Serial.print(level * 100L / (NUM_PIXELS * 255));
    Serial.println("%");
  }
}
