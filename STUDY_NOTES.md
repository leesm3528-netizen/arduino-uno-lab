# 학습 정리 · 아두이노 우노

1일차(2026-10-19)에는 LED 깜빡이기부터 버튼 ON/OFF 신호등까지, 2일차(2026-10-20)에는 가변저항 전압, 소리 센서, 7세그먼트 카운터까지 배운 내용을 순서대로 정리했습니다.

## 목차

**1일차 (10/19)**

1. [MCU · OS · GPIO](#1-mcu--os--gpio)
2. [디지털과 아날로그](#2-디지털과-아날로그)
3. [delay()와 millis()](#3-delay와-millis)
4. [버튼 입력: 풀업, 풀다운, 내부 풀업](#4-버튼-입력-풀업-풀다운-내부-풀업)
5. [디바운싱과 상태 변화 감지](#5-디바운싱과-상태-변화-감지)
6. [클릭 패턴: 더블클릭, 길게 누르기](#6-클릭-패턴-더블클릭-길게-누르기)
7. [인터럽트](#7-인터럽트)
8. [가변저항과 PWM](#8-가변저항과-pwm)
9. [네오픽셀 게이지](#9-네오픽셀-게이지)
10. [깜빡임 속도 실험과 잔상 효과](#10-깜빡임-속도-실험과-잔상-효과)
11. [신호등 만들기](#11-신호등-만들기)
12. [4다리 푸시 버튼의 구조](#12-4다리-푸시-버튼의-구조)
13. [Tinkercad 컴파일 에러](#13-tinkercad-컴파일-에러)
14. [arduino-cli로 업로드하기](#14-arduino-cli로-업로드하기)
15. [피지컬 컴퓨팅이란](#15-피지컬-컴퓨팅이란)

**2일차 (10/20)**

16. [가변저항 전압 읽기](#16-가변저항-전압-읽기)
17. [소리 센서](#17-소리-센서)
18. [7세그먼트 디스플레이](#18-7세그먼트-디스플레이)
19. [Wokwi 시뮬레이터](#19-wokwi-시뮬레이터)
20. [업로드가 안 될 때](#20-업로드가-안-될-때)

---

## 1. MCU · OS · GPIO

**MCU와 OS**

| | PC | 아두이노 우노 (MCU) |
|---|---|---|
| 구성 | CPU, 메모리, 저장장치가 따로 | CPU·메모리·입출력 핀이 **칩 하나**(ATmega328P)에 들어 있음 |
| OS | Windows 같은 운영체제가 여러 프로그램을 관리 | **OS가 없음**. 업로드한 프로그램 하나만 실행 |
| 실행 방식 | 프로그램을 골라 실행하고 종료 | 전원이 들어오면 `setup()` 한 번, 그다음 `loop()`를 끝없이 반복 |

**출력과 입력**
- **출력(OUTPUT)**: MCU가 핀에 전압을 내보냅니다. LED 켜기, 부저 울리기 등에 씁니다.
  - `digitalWrite(핀, HIGH)` → 5V, `digitalWrite(핀, LOW)` → 0V(GND)
- **입력(INPUT)**: MCU가 핀과 **같은 노드**에 걸린 전압을 읽습니다. 버튼이나 센서 값을 읽을 때 씁니다.
  - `digitalRead(핀)` → `HIGH` 또는 `LOW`
- 같은 핀이라도 `pinMode()`로 방향을 정해 줘야 합니다.
- **GPIO**(General Purpose Input/Output): 코드로 입력·출력을 정할 수 있는 범용 핀입니다. 우노는 디지털 0~13번, 아날로그 A0~A5번이 있습니다.
- A0~A5도 `pinMode(A5, OUTPUT)`처럼 디지털 핀으로 쓸 수 있습니다.

## 2. 디지털과 아날로그

| | 현실 세계 | 컴퓨터 세계 (MCU) |
|---|---|---|
| 신호 | 대부분 **아날로그**: 온도, 빛, 소리처럼 연속적인 값 | **디지털**: 0과 1 (`HIGH`/`LOW`) |

- **디지털**: 경우의 수가 2가지입니다. 우노에서 `HIGH` = 5V = 1, `LOW` = 0V = 0입니다. ESP32 같은 보드는 `HIGH`가 3.3V입니다.
- **디지털 입력의 판단 기준**: 우노는 읽은 전압이 약 **3V 이상이면 1**, 약 **1.5V 이하이면 0**으로 판단합니다. 그 사이 전압은 어느 쪽으로 읽힐지 보장되지 않습니다. 그래서 핀을 어느 한쪽으로 확실히 붙잡아 두는 풀업·풀다운이 필요합니다.
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

## 4. 버튼 입력: 풀업, 풀다운, 내부 풀업

버튼만 핀에 연결하면, 버튼을 누르지 않았을 때 핀이 아무 데도 연결되지 않아 값이 들쭉날쭉합니다. 이를 **플로팅**이라고 합니다. 저항으로 핀을 한쪽 전압에 붙잡아 두면 해결됩니다.

| 방식 | 결선 | 안 누름 | 누름 | 코드 |
|---|---|---|---|---|
| **풀업** | 핀 → 10kΩ → **5V**, 버튼 → **GND** | `HIGH` | `LOW` | `pinMode(핀, INPUT)` |
| **풀다운** | 핀 → 10kΩ → **GND**, 버튼 → **5V** | `LOW` | `HIGH` | `pinMode(핀, INPUT)` |
| **내부 풀업** | 버튼 → **GND**만 연결 (저항 없음) | `HIGH` | `LOW` | `pinMode(핀, INPUT_PULLUP)` |

- **내부 풀업**은 MCU 안에 들어 있는 저항(약 20~50kΩ)을 켜는 것입니다. 부품이 줄어서 오늘 만든 회로는 모두 이 방식을 썼습니다.
- 우노에는 **내부 풀다운이 없습니다.** 풀다운이 필요하면 외부 저항을 달아야 합니다.
- 풀업 방식에서는 누르면 `LOW`라서 일반적인 생각과 반대입니다. 코드에서 `== LOW`를 눌림으로 봅니다.

## 5. 디바운싱과 상태 변화 감지

**채터링과 디바운싱**
- 버튼을 한 번 눌러도 금속 접점이 튕기면서 몇 ms 동안 `HIGH`/`LOW`가 여러 번 바뀝니다. 이를 **채터링**이라고 합니다.
- 값이 바뀐 시각을 `millis()`로 기록하고, 그 뒤 50ms 동안 값이 그대로면 진짜 입력으로 인정합니다. 이것이 **디바운싱**입니다.

**상태 변화 감지 (State Change Detection)**
- `digitalRead()`만 쓰면 버튼을 누르고 있는 동안 계속 `LOW`가 읽혀서, 한 번 눌러도 수백 번으로 셉니다.
- **현재값과 과거값을 비교**해서 `HIGH` → `LOW`로 **바뀐 순간**에만 반응해야 꾹 누르고 있어도 한 번으로 셉니다.

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

## 6. 클릭 패턴: 더블클릭, 길게 누르기

→ [button_click](button_click/button_click.ino)

| 입력 | 판별 방법 | 동작 |
|---|---|---|
| 길게 누르기 | 누른 채로 1000ms가 지남 | LED OFF |
| 따닥 (더블클릭) | 짧게 뗀 뒤 300ms 안에 또 짧은 클릭 | LED ON |
| 한 번 | 짧게 뗀 뒤 300ms 동안 다음 클릭이 없음 | 무시 |

누른 시각과 뗀 시각을 각각 `millis()`로 기록하면, 같은 버튼 하나로도 여러 명령을 구분할 수 있습니다.

## 7. 인터럽트

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

## 8. 가변저항과 PWM

→ [pot_led](pot_led/pot_led.ino)

- 가변저항 연결: 양쪽 다리는 5V와 GND, **가운데 다리는 A0**입니다.
- `analogRead(A0)` → 0~1023, `analogWrite()`는 0~255를 받으므로 `map()`으로 범위를 바꿉니다.

```cpp
int brightness = map(analogRead(A0), 0, 1023, 0, 255);
analogWrite(9, brightness);
```

- **PWM**: LED를 아주 빠르게 켰다 껐다 하면서 켜진 시간의 비율로 밝기를 조절합니다.
- PWM 핀은 보드에 `~` 표시가 있는 **3, 5, 6, 9, 10, 11번**뿐입니다. 다른 핀에서는 켜짐/꺼짐만 됩니다.

## 9. 네오픽셀 게이지

→ [neopixel_gauge](neopixel_gauge/neopixel_gauge.ino)

- 네오픽셀은 선 하나(DIN)로 여러 개의 RGB LED를 각각 제어합니다. `Adafruit_NeoPixel` 라이브러리를 씁니다.
- 신호선은 반드시 **입력(DIN) 쪽**에 연결합니다.
- 가변저항 값을 `0 ~ 8 × 255`로 바꾸면 칸마다 255단계로 부드럽게 채울 수 있습니다.
- **`int` 오버플로**: 우노의 `int`는 16비트라서 최대 32767입니다. `255 × 254 = 64770`처럼 큰 곱셈은 `(long)`으로 바꿔서 계산합니다.

## 10. 깜빡임 속도 실험과 잔상 효과

→ [blink_fast](blink_fast/blink_fast.ino), 내장 LED(D13, `LED_BUILTIN`)로 실험했습니다.

| `delay()` | 1초당 깜빡임 | 보이는 모습 |
|---|---|---|
| 100ms (0.1초) | 5번 | 또렷하게 깜빡임 |
| 50ms | 10번 | 빠르게 깜빡임 |
| 5ms | 100번 | 거의 계속 켜짐 |
| 1ms (0.001초) | 500번 | 약간 어둡게 계속 켜짐 |

사람 눈은 1초에 50~60번보다 빠른 깜빡임을 구분하지 못합니다. 그래서 평균 밝기로 보이고, 이것이 **PWM으로 밝기를 조절하는 원리**입니다.

## 11. 신호등 만들기

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

## 12. 4다리 푸시 버튼의 구조

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

## 13. Tinkercad 컴파일 에러

```
error: variable or field 'setLight' declared void
error: 'Light' was not declared in this scope
```

- **원인**: 아두이노 IDE와 Tinkercad는 컴파일 전에 함수 선언문을 코드 맨 위에 자동으로 넣습니다. 그 선언문이 직접 만든 `enum Light`보다 먼저 와서 `Light`를 모른다는 에러가 났습니다.
- **해결**: 직접 만든 타입(`enum`, `struct`)을 함수 매개변수로 쓰지 않고 `int` 상수로 바꿨습니다.

## 14. arduino-cli로 업로드하기

```powershell
arduino-cli board list                                    # 보드와 포트 확인 (COM3)
arduino-cli compile --fqbn arduino:avr:uno --upload -p COM3 blink_fast
```

- `--fqbn arduino:avr:uno`: 대상 보드를 우노로 지정합니다.
- `--upload -p COM3`: 컴파일이 끝나면 COM3 포트로 바로 업로드합니다.
- 스케치 파일 이름과 폴더 이름이 같아야 합니다. 예: `blink_fast/blink_fast.ino`
- **파일을 저장하는 것만으로는 보드가 바뀌지 않습니다.** 업로드해야 반영됩니다. 업로드한 코드는 USB를 뽑아도 보드에 남습니다.

## 15. 피지컬 컴퓨팅이란

- "피지컬"은 물리학(Physics)보다 **물리적인, 실제로 존재하는(Physical)**에 가까운 뜻입니다.
- **피지컬 컴퓨팅**: 컴퓨터가 화면 밖 실제 세상과 직접 주고받는 것입니다. 입력(버튼, 센서)으로 현실을 읽고, 출력(LED, 모터)으로 현실을 바꿉니다.
- 그래도 물리학이 실제로 쓰입니다. 220Ω 저항은 옴의 법칙(V = IR)으로 전류를 줄이고, 채터링은 접점이 물리적으로 튕기는 현상이며, PWM은 눈의 잔상 효과를 이용합니다.
- 오늘 한 일을 한 문장으로 쓰면 이렇습니다. **AI 코딩 에이전트와 함께 아두이노로 실제 신호등을 만들었다.**

---

# 2일차 (2026-10-20)

## 16. 가변저항 전압 읽기

→ [pot_pwm](pot_pwm/pot_pwm.ino)

- `analogRead()`는 0~5V를 0~1023 숫자로 바꿉니다. 거꾸로 숫자에 `5.0 / 1023`을 곱하면 실제 전압이 됩니다.

```cpp
const int raw = analogRead(POT_PIN);         // 0 ~ 1023
const int pwm = map(raw, 0, 1023, 0, 255);   // 0 ~ 255
analogWrite(LED_PIN, pwm);
Serial.println(raw * (5.0 / 1023.0), 2);     // voltage, 2 decimal places
```

- `5`가 아니라 `5.0`으로 써야 소수점 계산이 됩니다. 정수끼리 나누면 소수점이 버려집니다.
- `Serial.println(값, 2)`의 두 번째 숫자는 소수점 아래 자릿수입니다.
- `F("text")`는 문자열을 RAM이 아닌 플래시 메모리에 둡니다. 우노의 RAM은 2KB뿐이라 출력 문구가 많을 때 아낄 수 있습니다.

## 17. 소리 센서

→ [sound_toggle](sound_toggle/sound_toggle.ino), [sound_green_red](sound_green_red/sound_green_red.ino)

| 핀 | 신호 | 뜻 |
|---|---|---|
| **AO** | 아날로그 | 마이크 파형을 그대로 출력. `analogRead()`로 읽음 |
| **DO** | 디지털 | 소리가 감도 나사로 정한 기준을 넘으면 바뀜. `digitalRead()`로 읽음 |

**Peak-to-Peak (소리 크기)**
- 소리는 빠르게 흔들리는 파형이라, AO 값을 한 번 읽어서는 크기를 알 수 없습니다.
- 20ms 동안 가장 큰 값과 가장 작은 값을 기록해서 그 차이를 소리 크기로 봅니다.

```cpp
minimumAdc = min(minimumAdc, raw);
maximumAdc = max(maximumAdc, raw);
if (now - windowAt >= 20) {
  peakToPeak = maximumAdc - minimumAdc;   // loudness of the last 20ms
  minimumAdc = 1023;
  maximumAdc = 0;
  windowAt = now;
}
```

**박수 한 번을 한 번으로 세기**
- **상태 변화 감지**: 소리가 없다가 생긴 순간(`sound && !wasSound`)에만 반응합니다.
- **쿨다운**: 반응한 뒤 250ms 동안은 새 소리를 무시합니다. 박수 소리의 울림 때문에 여러 번 세지는 것을 막습니다.

**감도 올리기**
1. 하드웨어: 센서의 파란 **감도 나사**를 돌립니다. 조용할 때 센서 LED가 꺼지고 박수 칠 때만 깜빡이는 위치가 좋습니다.
2. 소프트웨어: DO만 보지 않고 **AO의 Peak가 기준(10)을 넘어도** 소리로 인정했습니다. 조용할 때 Peak는 1~8이었습니다.

```cpp
const bool sound = active || loud;   // DO comparator OR loud AO window
```

| 증상 | 조절 |
|---|---|
| 조용한데 혼자 반응함 | `PEAK_THRESHOLD`를 올림 (15~20) |
| 작은 소리에 반응이 없음 | `PEAK_THRESHOLD`를 내림 (조용할 때 Peak보다는 커야 함) |
| 한 번에 두 번 바뀜 | `COOLDOWN_MS`를 늘림 (400) |

**번갈아 켜기**: `greenTurn` 변수가 다음에 켤 LED를 기억하고, 반응할 때마다 `!`로 뒤집습니다.

## 18. 7세그먼트 디스플레이

→ [seven_segment_counter](seven_segment_counter/seven_segment_counter.ino)

**구조**
- LED 7개(A~G)와 소수점(DP)이 숫자 모양으로 배치돼 있습니다. 위쪽 가로가 A이고, 시계 방향으로 B, C, D, E, F, 가운데 가로가 G입니다.
- **공통 캐소드**(5161AS, 3161AS): LED들의 − 쪽이 COM으로 묶여 있습니다. COM을 GND에 연결하고 세그먼트 핀에 `HIGH`를 주면 켜집니다.
- **공통 애노드**(5161BS 등): 반대로 COM을 5V에 연결하고 `LOW`를 줘야 켜집니다.
- 세그먼트마다 **저항을 하나씩**(330Ω) 답니다. COM 쪽에 저항 하나만 달면 켜진 세그먼트 수에 따라 밝기가 달라집니다.

**5161AS 다리 번호** (DP가 오른쪽 아래에 오게 놓고 봤을 때)

| 위쪽 줄 | 10 G | 9 F | 8 COM | 7 A | 6 B |
|---|---|---|---|---|---|
| **아래쪽 줄** | **1 E** | **2 D** | **3 COM** | **4 C** | **5 DP** |

**숫자를 비트로 저장하기**
- 숫자마다 켤 세그먼트를 1바이트에 담습니다. 비트 0이 A, 비트 1이 B, …, 비트 6이 G입니다.

| 숫자 | 값 | 2진수 (G F E D C B A) | 켜지는 세그먼트 |
|---|---|---|---|
| 0 | `0x3F` | 0111111 | A B C D E F |
| 1 | `0x06` | 0000110 | B C |
| 8 | `0x7F` | 1111111 | 전부 |

```cpp
for (byte segment = 0; segment < 7; segment++) {
  digitalWrite(SEGMENT_PINS[segment], bitRead(DIGITS[number], segment));
}
```

- `bitRead(값, n)`은 n번째 비트(0 또는 1)를 꺼냅니다.
- `number = (number + 1) % 10`으로 9 다음에 0으로 돌아갑니다.

## 19. Wokwi 시뮬레이터

- 브라우저에서 아두이노 회로와 코드를 시뮬레이션합니다: https://wokwi.com/projects/new/arduino-uno
- `+` 버튼으로 부품을 추가하고, 다리 끝을 클릭해서 선을 잇습니다.
- 회로가 `diagram.json` 파일로 저장돼서 회로를 코드처럼 복사해 붙여넣을 수 있습니다.
- Tinkercad보다 컴파일이 빠르고 쓸 수 있는 라이브러리가 많습니다.

## 20. 업로드가 안 될 때

```
Error: cannot open port \.\COM3
```

- USB가 빠졌거나 보드가 인식되지 않은 상태입니다. `arduino-cli board list`가 `No boards found`를 보여 줍니다.
- USB 케이블을 다시 꽂고, 보드의 초록 **ON** LED가 켜졌는지 확인합니다.
- 다른 USB 포트에 꽂으면 COM 번호가 바뀔 수 있으니 `board list`로 다시 확인합니다.
- 시리얼 모니터가 COM3을 쓰고 있어도 업로드가 실패합니다. 모니터를 닫고 다시 올립니다.
