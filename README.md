# Arduino Uno Lab

아두이노 우노 고급키트로 LED·버튼·가변저항·네오픽셀을 다뤄 보고, 마지막에는 **버튼으로 켜고 끄는 실물 신호등**을 만든 학습 기록입니다.
코드는 AI 코딩 에이전트(Claude Code)와 함께 작성했고, `arduino-cli`로 노트북에서 보드에 바로 업로드했습니다.

![Arduino](https://img.shields.io/badge/Arduino_Uno-00878F?style=flat-square&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Tinkercad](https://img.shields.io/badge/Tinkercad-1477D1?style=flat-square&logo=autodesk&logoColor=white)
![arduino-cli](https://img.shields.io/badge/arduino--cli-1.5-00979D?style=flat-square)

> 오늘 배운 개념은 [STUDY_NOTES.md](STUDY_NOTES.md)에 정리했습니다.

---

## 진행 과정

### 1. 아두이노 고급키트 구성품

우노 보드, 브레드보드, 점퍼선, LED, 저항, 버튼, 가변저항, LCD, 서보·스텝 모터, 리모컨, 각종 센서가 들어 있는 키트로 시작했습니다.

<img src="assets/01-kit.jpg" width="480" alt="아두이노 고급키트 구성품을 바닥에 펼쳐 놓은 사진">

### 2. 신호등 회로 연결

LED 3개(빨강·노랑·초록)를 220Ω 저항과 함께 브레드보드에 꽂고, 4다리 푸시 버튼을 가운데 홈에 걸쳐 연결했습니다.

<img src="assets/02-traffic-light.jpg" width="480" alt="아두이노 우노와 브레드보드에 LED 3개와 버튼을 연결한 신호등 회로">

### 3. 토글 기능 추가

버튼을 누를 때마다 신호등 전체가 꺼지고, 다시 누르면 초록불부터 다시 시작합니다.

<img src="assets/03-toggle-demo.gif" width="240" alt="버튼을 누르면 신호등이 꺼지고 다시 켜지는 시연 영상">

[원본 영상 보기 (mp4)](assets/03-toggle-demo.mp4)

---

## 신호등 회로

| 우노 핀 | 연결 | 코드 |
|---|---|---|
| D8 | 빨강 LED → 220Ω → GND | `RED_PIN = 8` |
| D9 | 노랑 LED → 220Ω → GND | `YELLOW_PIN = 9` |
| D10 | 초록 LED → 220Ω → GND | `GREEN_PIN = 10` |
| D2 | 푸시 버튼 → GND (대각선 다리 사용) | `BUTTON_PIN = 2`, `INPUT_PULLUP` |

- 신호 순서: 초록 → 노랑 → 빨강, 각 0.5초
- 버튼: 누를 때마다 신호등 ON/OFF 토글
- `delay()` 없이 `millis()`로 시간을 재서 버튼 입력을 놓치지 않습니다

코드: [TrafficLight/TrafficLight.ino](TrafficLight/TrafficLight.ino)

---

## 스케치 목록

배운 순서대로 정리했습니다. `blink_fast`와 `TrafficLight`는 실제 우노 보드에 업로드해서 동작을 확인했고, 나머지는 Tinkercad Circuits 시뮬레이션용으로 작성했습니다.

| 스케치 | 내용 | 핵심 개념 |
|---|---|---|
| [Blink](Blink/Blink.ino) | A0에 꽂은 LED를 1초 간격으로 깜빡임 | `pinMode`, `digitalWrite`, `delay` |
| [LED_Pin3](LED_Pin3/LED_Pin3.ino) | D3 핀으로 LED 켜고 끄기 | GPIO 출력 |
| [ButtonRead](ButtonRead/ButtonRead.ino) | 버튼 상태를 시리얼 모니터에 출력 | `INPUT_PULLUP`, `digitalRead` |
| [PotRead](PotRead/PotRead.ino) | 가변저항 값 읽기 | `analogRead` (0~1023) |
| [button_count](button_count/button_count.ino) | 누른 횟수 세기, 5번마다 LED 토글 | 현재값·과거값 비교, `millis()` 디바운스 |
| [button_click](button_click/button_click.ino) | 더블클릭 → ON, 길게 누르기 → OFF, 한 번 → 무시 | 클릭 패턴 판별 |
| [button_interrupt](button_interrupt/button_interrupt.ino) | 누를 때와 뗄 때마다 LED 토글 | `attachInterrupt`, `volatile`, ISR |
| [pot_led](pot_led/pot_led.ino) | 가변저항으로 LED 밝기 조절 | `map`, `analogWrite`, PWM |
| [neopixel_gauge](neopixel_gauge/neopixel_gauge.ino) | 가변저항으로 네오픽셀 8구 게이지 채우기 | `Adafruit_NeoPixel`, `int` 오버플로 |
| [blink_fast](blink_fast/blink_fast.ino) | 내장 LED 깜빡임 속도 실험 (0.1초 → 0.001초) | 잔상 효과, PWM 원리 |
| [TrafficLight](TrafficLight/TrafficLight.ino) | 버튼 ON/OFF 신호등 | `millis()` 상태 전환, 디바운스 |

---

## 업로드 방법

```powershell
# 연결된 보드와 포트 확인
arduino-cli board list

# 컴파일하고 바로 업로드 (우노, COM3)
arduino-cli compile --fqbn arduino:avr:uno --upload -p COM3 TrafficLight
```

코드 파일을 저장하는 것만으로는 보드가 바뀌지 않습니다. 고친 뒤에는 꼭 다시 컴파일하고 업로드해야 합니다.
