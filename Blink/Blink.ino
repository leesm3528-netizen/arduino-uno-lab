// Arduino Uno - Blink
// A0 핀에 연결한 LED를 1초 간격으로 켰다 껐다 합니다.
// 배선: A0 → 저항(220Ω) → LED 긴 다리(+) / LED 짧은 다리(-) → GND

const int LED_PIN = A0;           // A0 핀을 디지털 출력으로 사용

void setup() {
  pinMode(LED_PIN, OUTPUT);       // A0 핀을 출력으로 설정
}

void loop() {
  digitalWrite(LED_PIN, HIGH);    // LED 켜기 (5V)
  delay(1000);                    // 1초 대기
  digitalWrite(LED_PIN, LOW);     // LED 끄기 (0V)
  delay(1000);                    // 1초 대기
}
