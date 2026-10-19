# 학습 정리 · 아두이노 우노 (2026-10-19)

LED 깜빡이기부터 버튼 ON/OFF 신호등까지, 하루 동안 배운 내용을 순서대로 정리했습니다.

## 목차

1. [MCU 기초: 입력, 출력, GPIO](#1-mcu-기초-입력-출력-gpio)
2. [디지털과 아날로그](#2-디지털과-아날로그)
3. [delay()와 millis()](#3-delay와-millis)
4. [버튼 입력과 디바운스](#4-버튼-입력과-디바운스)
5. [클릭 패턴: 더블클릭, 길게 누르기](#5-클릭-패턴-더블클릭-길게-누르기)
6. [인터럽트](#6-인터럽트)
7. [가변저항과 PWM](#7-가변저항과-pwm)
8. [네오픽셀 게이지](#8-네오픽셀-게이지)
9. [깜빡임 속도 실험과 잔상 효과](#9-깜빡임-속도-실험과-잔상-효과)
10. [신호등 만들기](#10-신호등-만들기)
11. [4다리 푸시 버튼의 구조](#11-4다리-푸시-버튼의-구조)
12. [Tinkercad 컴파일 에러](#12-tinkercad-컴파일-에러)
13. [arduino-cli로 업로드하기](#13-arduino-cli로-업로드하기)
14. [피지컬 컴퓨팅이란](#14-피지컬-컴퓨팅이란)

---

## 1. MCU 기초: 입력, 출력, GPIO

- **출력(OUTPUT)**: MCU가 핀에 전압을 내보냅니다. LED 켜기, 부저 울리기 등에 씁니다.
  - `digitalWrite(핀, HIGH)` → 5V, `digitalWrite(핀, LOW)` → 0V
- **입력(INPUT)**: MCU가 핀에 걸린 전압을 읽습니다. 버튼이나 센서 값을 읽을 때 씁니다.
  - `digitalRead(핀)` → `HIGH` 또는 `LOW`
- 같은 핀이라도 `pinMode()`로 방향을 정해 줘야 합니다.
- **GPIO**(General Purpose Input/Output): 코드로 입력·출력을 정할 수 있는 범용 핀입니다. 우노는 디지털 0~13번, 아날로그 A0~A5번이 있습니다.
- A0~A5도 `pinMode(A5, OUTPUT)`처럼 디지털 핀으로 쓸 수 있습니다.

## 2. 디지털과 아날로그

| | 현실 세계 | 컴퓨터 세계 (MCU) |
|---|---|---|
| 신호 | 대부분 **아날로그**: 온도, 빛, 소리처럼 연속적인 값 | **디지털**: 0과 1 (`HIGH`/`LOW`) |

- **디지털**: 경우의 수가 2가지입니다. 우노에서 `HIGH` = 5V = 1, `LOW` = 0V = 0입니다. ESP32 같은 보드는 `HIGH`가 3.3V입니다.
- **아날로그 입력**: `analogRead()`는 10비트라서 2¹⁰ = **1024가지**(0~1023)로 읽습니다. ADC가 0~5V를 숫자로 바꿉니다.
- **아날로그처럼 출력**: `analogWrite()`는 8비트라서 2⁸ = **256가지**(0~255)입니다. 실제로는 PWM으로 흉내 냅니다.

## 3. delay()와 millis()

| | `delay(ms)` | `millis()` |
|---|---|---|
| 동작 | 그 시간 동안 프로그램이 **멈춤** | 보드가 켜진 뒤 지난 시간(ms)을 **읽기만** 함 |
| 단점·장점 | 기다리는 동안 버튼 입력을 놓침 | `loop()`가 계속 돌아서 여러 일을 동시에 처리 |

`millis()`로 시간을 재는 기본 패턴입니다.

```cpp
if (millis() - startTime >= INTERVAL) {
  startTime = millis();
  // do the next step
}
```

`unsigned long`으로 저장하면 약 49일 뒤 `millis()`가 0으로 돌아가도 뺄셈 결과가 정확합니다.

## 4. 버튼 입력과 디바운스

**INPUT_PULLUP**
- 아무것도 연결되지 않은 입력 핀은 값이 들쭉날쭉합니다. 이를 **플로팅**이라고 합니다.
- `pinMode(핀, INPUT_PULLUP)`을 쓰면 아두이노 내부 저항이 핀을 5V로 붙잡아 줍니다.
- 버튼은 **핀 ↔ GND**로 연결합니다. 안 누르면 `HIGH`, 누르면 `LOW`로 읽혀서 일반적인 생각과 반대입니다.

**채터링과 디바운스**
- 버튼을 한 번 눌러도 금속 접점이 튕기면서 몇 ms 동안 `HIGH`/`LOW`가 여러 번 바뀝니다. 이를 **채터링**이라고 합니다.
- **현재값과 과거값을 비교**해서 값이 바뀐 시각을 `millis()`로 기록합니다. 그 뒤 50ms 동안 값이 그대로면 진짜 입력으로 인정합니다.
- 눌린 순간(`HIGH` → `LOW`)에만 세야 꾹 누르고 있어도 한 번으로 셉니다.

```cpp
int reading = digitalRead(BUTTON_PIN);        // current value
if (reading != lastReading) {
  lastChangeTime = millis();                  // value changed -> record time
}
if (millis() - lastChangeTime > DEBOUNCE_MS && reading != stableState) {
  stableState = reading;
  if (stableState == LOW) {
    pressCount++;                             // count only on press
  }
}
lastReading = reading;                        // save as previous value
```

- 응용: `pressCount % 5 == 0`이면 5번마다 LED를 토글합니다. → [button_count](button_count/button_count.ino)

## 5. 클릭 패턴: 더블클릭, 길게 누르기

→ [button_click](button_click/button_click.ino)

| 입력 | 판별 방법 | 동작 |
|---|---|---|
| 길게 누르기 | 누른 채로 1000ms가 지남 | LED OFF |
| 따닥 (더블클릭) | 짧게 뗀 뒤 300ms 안에 또 짧은 클릭 | LED ON |
| 한 번 | 짧게 뗀 뒤 300ms 동안 다음 클릭이 없음 | 무시 |

누른 시각과 뗀 시각을 각각 `millis()`로 기록하면, 같은 버튼 하나로도 여러 명령을 구분할 수 있습니다.

## 6. 인터럽트

→ [button_interrupt](button_interrupt/button_interrupt.ino)

- `loop()`가 다른 일을 하는 중이어도 핀 신호가 바뀌는 **그 순간** 지정한 함수(ISR)를 실행합니다.
- 우노에서 `attachInterrupt()`를 쓸 수 있는 핀은 **D2, D3뿐**입니다.

```cpp
attachInterrupt(digitalPinToInterrupt(2), onButtonChange, CHANGE);
```

| 모드 | 실행 시점 |
|---|---|
| `FALLING` | `HIGH` → `LOW` (풀업 버튼을 누르는 순간) |
| `RISING` | `LOW` → `HIGH` (떼는 순간) |
| `CHANGE` | 둘 다 |

**ISR 규칙**
- ISR과 `loop()`가 같이 쓰는 변수에는 `volatile`을 붙입니다. 안 붙이면 컴파일러 최적화 때문에 바뀐 값을 못 읽을 수 있습니다.
- ISR 안에서는 `Serial.print()`와 `delay()`를 쓰지 않습니다. ISR은 플래그만 세우고, 출력은 `loop()`에서 합니다.
- 인터럽트에서도 채터링이 생깁니다. `FALLING`만 쓰면 **뗄 때 생기는 튕김**을 누름으로 착각할 수 있습니다. `CHANGE`로 누름과 뗌을 모두 받아 걸러야 정확합니다.

## 7. 가변저항과 PWM

→ [pot_led](pot_led/pot_led.ino)

- 가변저항 연결: 양쪽 다리는 5V와 GND, **가운데 다리는 A0**입니다.
- `analogRead(A0)` → 0~1023, `analogWrite()`는 0~255를 받으므로 `map()`으로 범위를 바꿉니다.

```cpp
int brightness = map(analogRead(A0), 0, 1023, 0, 255);
analogWrite(9, brightness);
```

- **PWM**: LED를 아주 빠르게 켰다 껐다 하면서 켜진 시간의 비율로 밝기를 조절합니다.
- PWM 핀은 보드에 `~` 표시가 있는 **3, 5, 6, 9, 10, 11번**뿐입니다. 다른 핀에서는 켜짐/꺼짐만 됩니다.

## 8. 네오픽셀 게이지

→ [neopixel_gauge](neopixel_gauge/neopixel_gauge.ino)

- 네오픽셀은 선 하나(DIN)로 여러 개의 RGB LED를 각각 제어합니다. `Adafruit_NeoPixel` 라이브러리를 씁니다.
- 신호선은 반드시 **입력(DIN) 쪽**에 연결합니다.
- 가변저항 값을 `0 ~ 8 × 255`로 바꾸면 칸마다 255단계로 부드럽게 채울 수 있습니다.
- **`int` 오버플로**: 우노의 `int`는 16비트라서 최대 32767입니다. `255 × 254 = 64770`처럼 큰 곱셈은 `(long)`으로 바꿔서 계산합니다.

## 9. 깜빡임 속도 실험과 잔상 효과

→ [blink_fast](blink_fast/blink_fast.ino), 내장 LED(D13, `LED_BUILTIN`)로 실험했습니다.

| `delay()` | 1초당 깜빡임 | 보이는 모습 |
|---|---|---|
| 100ms (0.1초) | 5번 | 또렷하게 깜빡임 |
| 50ms | 10번 | 빠르게 깜빡임 |
| 5ms | 100번 | 거의 계속 켜짐 |
| 1ms (0.001초) | 500번 | 약간 어둡게 계속 켜짐 |

사람 눈은 1초에 50~60번보다 빠른 깜빡임을 구분하지 못합니다. 그래서 평균 밝기로 보이고, 이것이 **PWM으로 밝기를 조절하는 원리**입니다.

## 10. 신호등 만들기

→ [TrafficLight](TrafficLight/TrafficLight.ino)

발전 과정:
1. `delay()`로 초록 5초 → 노랑 2초 → 빨강 5초를 반복했습니다.
2. `millis()`와 **상태 변수**(`STATE_GREEN`, `STATE_YELLOW`, `STATE_RED`)로 바꿨습니다. 멈추지 않으니 버튼을 함께 처리할 수 있습니다.
3. 인터럽트 버튼을 추가해서 누르면 LED 3개가 즉시 다 켜지게 했습니다.
4. 실제 브레드보드로 옮기면서 핀을 D8·D9·D10으로 바꾸고, 간격을 0.5초로 줄였습니다.
5. 최종: **버튼을 누를 때마다 신호등 전체 ON/OFF 토글** 기능을 넣었습니다.

```cpp
if (currentState == STATE_GREEN && elapsed >= GREEN_TIME) {
  changeState(STATE_YELLOW);
} else if (currentState == STATE_YELLOW && elapsed >= YELLOW_TIME) {
  changeState(STATE_RED);
} else if (currentState == STATE_RED && elapsed >= RED_TIME) {
  changeState(STATE_GREEN);
}
```

**LED 연결**: 핀 → LED 긴 다리(+) → 짧은 다리(−) → 220Ω → GND. 저항은 LED 앞에 있어도 뒤에 있어도 똑같이 전류를 제한합니다.

## 11. 4다리 푸시 버튼의 구조

다리 4개는 안에서 **2개씩 짝지어 원래부터 연결**돼 있고, 버튼을 누르면 두 짝이 이어집니다.

```
   A1 ━━━━━━ A2      A1-A2 : always connected (inside)
       [ ● ]         B1-B2 : always connected (inside)
   B1 ━━━━━━ B2      press : A connects to B
```

| 고른 다리 두 개 | 결과 |
|---|---|
| 같은 짝 (A1-A2) | 항상 눌린 상태가 돼서 버튼이 안 먹힘 |
| 다른 짝 (A1-B1) | 정상 동작 |
| **대각선 (A1-B2)** | 어떻게 돌려 꽂아도 항상 정상 동작 |

실제로 처음에 회로가 동작하지 않았는데, **대각선 다리**로 바꿔 꽂자 해결됐습니다.

## 12. Tinkercad 컴파일 에러

```
error: variable or field 'setLight' declared void
error: 'Light' was not declared in this scope
```

- **원인**: 아두이노 IDE와 Tinkercad는 컴파일 전에 함수 선언문을 코드 맨 위에 자동으로 넣습니다. 그 선언문이 직접 만든 `enum Light`보다 먼저 와서 `Light`를 모른다는 에러가 났습니다.
- **해결**: 직접 만든 타입(`enum`, `struct`)을 함수 매개변수로 쓰지 않고 `int` 상수로 바꿨습니다.

## 13. arduino-cli로 업로드하기

```powershell
arduino-cli board list                                    # 보드와 포트 확인 (COM3)
arduino-cli compile --fqbn arduino:avr:uno --upload -p COM3 blink_fast
```

- `--fqbn arduino:avr:uno`: 대상 보드를 우노로 지정합니다.
- `--upload -p COM3`: 컴파일이 끝나면 COM3 포트로 바로 업로드합니다.
- 스케치 파일 이름과 폴더 이름이 같아야 합니다. 예: `blink_fast/blink_fast.ino`
- **파일을 저장하는 것만으로는 보드가 바뀌지 않습니다.** 업로드해야 반영됩니다. 업로드한 코드는 USB를 뽑아도 보드에 남습니다.

## 14. 피지컬 컴퓨팅이란

- "피지컬"은 물리학(Physics)보다 **물리적인, 실제로 존재하는(Physical)**에 가까운 뜻입니다.
- **피지컬 컴퓨팅**: 컴퓨터가 화면 밖 실제 세상과 직접 주고받는 것입니다. 입력(버튼, 센서)으로 현실을 읽고, 출력(LED, 모터)으로 현실을 바꿉니다.
- 그래도 물리학이 실제로 쓰입니다. 220Ω 저항은 옴의 법칙(V = IR)으로 전류를 줄이고, 채터링은 접점이 물리적으로 튕기는 현상이며, PWM은 눈의 잔상 효과를 이용합니다.
- 오늘 한 일을 한 문장으로 쓰면 이렇습니다. **AI 코딩 에이전트와 함께 아두이노로 실제 신호등을 만들었다.**
