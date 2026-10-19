// Arduino Uno - GPIO 3번 핀으로 LED 켜고 끄기
// LED를 1초 간격으로 켰다 껐다 합니다.
// 배선: D3 → 저항(220Ω) → LED 긴 다리(+) / LED 짧은 다리(-) → GND

const int LED_PIN = 3;            // 디지털 3번 핀

void setup() {
  pinMode(LED_PIN, OUTPUT);       // 3번 핀을 출력으로 설정
}

void loop() {
  digitalWrite(LED_PIN, HIGH);    // LED 켜기 (5V)
  delay(1000);                    // 1초 대기
  digitalWrite(LED_PIN, LOW);     // LED 끄기 (0V)
  delay(1000);                    // 1초 대기
}
