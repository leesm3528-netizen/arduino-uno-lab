# Arduino Uno Lab

아두이노 우노 고급키트로 LED·버튼·가변저항·네오픽셀을 다뤄 보고, 마지막에는 **버튼으로 켜고 끄는 실물 신호등**을 만든 학습 기록입니다.
코드는 AI 코딩 에이전트(Claude Code)와 함께 작성했고, `arduino-cli`로 노트북에서 보드에 바로 업로드했습니다.

![Arduino](https://img.shields.io/badge/Arduino_Uno-00878F?style=flat-square&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Tinkercad](https://img.shields.io/badge/Tinkercad-1477D1?style=flat-square&logo=autodesk&logoColor=white)
![arduino-cli](https://img.shields.io/badge/arduino--cli-1.5-00979D?style=flat-square)

> 자세한 설명과 코드 예시는 [STUDY_NOTES.md](STUDY_NOTES.md)에 정리했습니다.

---

## 오늘의 학습 키워드

### MCU · OS · GPIO

| 키워드 | 한 줄 정리 |
|---|---|
| **MCU** | CPU·메모리·입출력 핀이 칩 하나에 들어 있는 작은 컴퓨터. 우노의 MCU는 ATmega328P |
| **OS** | PC는 운영체제가 여러 프로그램을 관리하지만, 우노에는 OS가 없고 업로드한 프로그램 하나만 실행 (`setup()` 한 번 → `loop()` 무한 반복) |
| **GPIO** | 코드로 입력·출력을 정할 수 있는 범용 핀. `pinMode()`로 방향을 정함 |

### 입력과 출력

| 구분 | 동작 | 함수 | 값 |
|---|---|---|---|
| **디지털 출력** | 핀을 5V 또는 GND(0V)로 만든다 | `digitalWrite()` | `HIGH` / `LOW` |
| **디지털 입력** | 같은 노드의 전압을 읽고, 5V에 가까운지 0V에 가까운지 판단한다 | `digitalRead()` | 1 / 0 |
| **아날로그 입력** | 같은 노드의 전압을 숫자로 읽는다 (ADC, 10비트) | `analogRead()` | 0 ~ 1023 |
| **아날로그 출력** | PWM(펄스 폭 변조)으로 켜진 시간 비율을 바꿔 아날로그 값을 표현한다 | `analogWrite()` | 0 ~ 255 |

### 회로 결선

| 부품 | 결선 | 스케치 |
|---|---|---|
| **가변저항** | 양 끝 다리 → 5V, GND / 가운데 다리 → A0 | [pot_led](pot_led/pot_led.ino) |
| **LED** | 핀 → 220Ω → LED 긴 다리(+) / 짧은 다리(−) → GND | [TrafficLight](TrafficLight/TrafficLight.ino) |
| **버튼 (풀업)** | 핀 → 10kΩ → 5V, 버튼 → GND · 평소 `HIGH`, 누르면 `LOW` | [STUDY_NOTES §4](STUDY_NOTES.md#4-버튼-입력-풀업-풀다운-내부-풀업) |
| **버튼 (풀다운)** | 핀 → 10kΩ → GND, 버튼 → 5V · 평소 `LOW`, 누르면 `HIGH` | [STUDY_NOTES §4](STUDY_NOTES.md#4-버튼-입력-풀업-풀다운-내부-풀업) |
| **버튼 (내부 풀업)** | `INPUT_PULLUP` + 버튼 → GND · 외부 저항 필요 없음 | [ButtonRead](ButtonRead/ButtonRead.ino) |

### 버튼을 정확하게 읽는 방법

| 키워드 | 한 줄 정리 | 스케치 |
|---|---|---|
| **디바운싱** | 접점이 튕기는 채터링을 `millis()`로 50ms 동안 기다려 걸러낸다 | [button_count](button_count/button_count.ino) |
| **상태 변화 감지** | 현재값과 과거값을 비교해 `HIGH → LOW`로 바뀐 순간만 잡는다 | [button_click](button_click/button_click.ino) |
| **인터럽트** | D2·D3 핀 신호가 바뀌는 순간 ISR을 바로 실행한다 (`attachInterrupt`) | [button_interrupt](button_interrupt/button_interrupt.ino) |

### 네오픽셀

| 키워드 | 한 줄 정리 | 스케치 |
|---|---|---|
| **네오픽셀** | 신호선 하나(DIN)로 RGB LED 여러 개를 각각 제어. `Adafruit_NeoPixel` 라이브러리 사용 | [neopixel_gauge](neopixel_gauge/neopixel_gauge.ino) |

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
