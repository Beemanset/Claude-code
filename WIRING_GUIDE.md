# Wiring Guide - CYD Handheld Radar

Complete wiring diagrams and pin connection guide for the M314-inspired motion tracker.

---

## Table of Contents

1. [GPIO Pin Assignments](#gpio-pin-assignments)
2. [Power System Wiring](#power-system-wiring)
3. [LD2450 Radar Connection](#ld2450-radar-connection)
4. [Audio System Connection](#audio-system-connection)
5. [5-Way Navigation Switch](#5-way-navigation-switch)
6. [Battery Monitoring](#battery-monitoring)
7. [Complete System Diagram](#complete-system-diagram)
8. [Wire Color Conventions](#wire-color-conventions)
9. [Testing & Troubleshooting](#testing--troubleshooting)

---

## GPIO Pin Assignments

### ESP32 CYD Pin Usage

| GPIO | Function | Direction | Connected To | Pin Type | Notes |
|------|----------|-----------|--------------|----------|-------|
| **GPIO 22** | UART RX | Input | LD2450 TX | I/O | CN1 connector |
| **GPIO 27** | UART TX | Output | LD2450 RX | I/O | CN1 connector |
| **GPIO 25** | DAC1 Audio | Output | PAM8403 IN_L | DAC | Built-in DAC |
| **GPIO 4** | Button UP | Input | 5-way UP | I/O | Freed from RGB LED |
| **GPIO 16** | Button DOWN | Input | 5-way DOWN | I/O | Freed from RGB LED |
| **GPIO 17** | Button LEFT | Input | 5-way LEFT | I/O | Freed from RGB LED |
| **GPIO 21** | Button RIGHT | Input | 5-way RIGHT | I/O | CN1 connector |
| **GPIO 2** | Button CENTER | Input | 5-way CENTER | I/O | Internal pin |
| **GPIO 35** | Battery ADC | Input | Voltage divider | Input-only | Optional, CN1 |

### Reserved/Used Pins (Do Not Modify)

| GPIO | Function | Used By | Notes |
|------|----------|---------|-------|
| GPIO 14 | SPI CLK | ILI9341 TFT | Display clock signal |
| GPIO 13 | SPI MOSI | ILI9341 TFT | Display data out |
| GPIO 12 | SPI MISO | ILI9341 TFT | Display data in |
| GPIO 15 | TFT CS | ILI9341 TFT | Display chip select |
| GPIO 2 | TFT DC | ILI9341 TFT | Display data/command |
| GPIO 33 | Touch CS | XPT2046 | Touchscreen chip select |
| GPIO 32 | Backlight | LED PWM | Display backlight control |
| GPIO 0 | Boot mode | System | Programming/boot selection |
| GPIO 1 | TX0 | USB Serial | Debug UART transmit |
| GPIO 3 | RX0 | USB Serial | Debug UART receive |

### CN1 Connector Pinout

The CN1 connector on the ESP32 CYD provides access to additional GPIO pins:

```
CN1 Pin Header (5 pins):
┌─────────────┐
│ 1  GPIO 35  │ ← Input only (ADC1_CH7)
│ 2  GPIO 22  │ ← I/O (UART for LD2450)
│ 3  GPIO 21  │ ← I/O (5-way RIGHT button)
│ 4  GPIO 27  │ ← I/O (UART for LD2450)
│ 5  GND      │ ← Ground
└─────────────┘
```

**Action Required**: Solder a 5-pin male header (2.54mm pitch) to CN1 for easy access.

---

## Power System Wiring

### Complete Power Chain

```
┌──────────────────────────────────────────────────────────────┐
│                   POWER DISTRIBUTION SYSTEM                   │
└──────────────────────────────────────────────────────────────┘

18650 Battery
(3.7V nominal, 3.0-4.2V range)
        │
        ├───[+]───→ LED Battery Monitor IN+  (visual indicator)
        │
        └───[-]───→ LED Battery Monitor IN-
                         │
                         ↓
                    TP4056 Module
                  (Charging Circuit)
                         │
        ┌────────────────┴────────────────┐
        │                                  │
    [BAT+]  [BAT-]                  [Micro USB]
    (to 18650)                       (5V charging input)
        │
    [OUT+] ─→ POWER SWITCH (SPST) ─→ MT3608 [IN+]
    [OUT-] ─────────────────────────→ MT3608 [IN-/GND]
                                          │
                              Adjust to 5.0V output!
                                          │
                              ┌───────────┴───────────┐
                              │                       │
                          [OUT+] 5V Rail         [OUT-] GND
                              │                       │
        ┌─────────────────────┼───────────┬───────────┼─────────┐
        │                     │           │           │         │
        ↓                     ↓           ↓           ↓         ↓
    ESP32 CYD             LD2450      PAM8403     5-Way SW   Common
    (5V pin)              (VCC)       (VCC)        (VCC)      GND
```

### TP4056 Connections

```
TP4056 Module Pinout:
┌────────────────────┐
│  [BAT+]   [BAT-]   │  ← Connect to 18650 battery
│                    │
│  [OUT+]   [OUT-]   │  ← Output to power switch and MT3608
│                    │
│   [Micro USB]      │  ← 5V charging input
│                    │
│ [LED1] [LED2]      │  ← Charging status LEDs
└────────────────────┘

Connections:
  BAT+  → 18650 Positive (+)
  BAT-  → 18650 Negative (-)
  OUT+  → Power Switch → MT3608 IN+
  OUT-  → MT3608 IN- (GND)
  USB   → Micro USB cable (for charging)
```

### MT3608 Boost Converter Setup

```
MT3608 Module Pinout:
┌────────────────────────┐
│    [IN+]  [IN-]        │  ← Input from TP4056
│                        │
│    [OUT+] [OUT-]       │  ← 5V output (adjust with pot!)
│                        │
│      [POT]             │  ← Voltage adjustment potentiometer
└────────────────────────┘

Connections:
  IN+   → TP4056 OUT+ (through power switch)
  IN-   → TP4056 OUT- / GND
  OUT+  → 5V rail (distribute to all components)
  OUT-  → Common GND

IMPORTANT: Adjust output voltage to exactly 5.0V before connecting components!
```

### Voltage Adjustment Procedure

**Before connecting any components:**

1. Connect battery to TP4056
2. Connect TP4056 output to MT3608 input
3. Turn power switch ON
4. Measure MT3608 output with multimeter
5. Adjust potentiometer until output reads **5.00V ± 0.05V**
6. Verify voltage is stable
7. Turn power OFF before connecting components

### Power Distribution

```
5V Rail Distribution:
     MT3608 OUT+
          │
          ├─────→ ESP32 CYD (5V pin)        [150-250mA]
          │
          ├─────→ LD2450 (VCC)              [100-200mA]
          │
          ├─────→ PAM8403 (VCC)             [50-250mA]
          │
          └─────→ 5-way Switch (VCC)        [<1mA]

GND Common:
     MT3608 OUT-
          │
          ├─────→ ESP32 CYD (GND)
          │
          ├─────→ LD2450 (GND)
          │
          ├─────→ PAM8403 (GND)
          │
          ├─────→ 5-way Switch (GND)
          │
          └─────→ Speaker (-)  [if needed]
```

---

## LD2450 Radar Connection

### LD2450 Module Pinout

The LD2450 uses a **4-pin JST 1.25mm connector**:

```
LD2450 Connector:
┌─────────────────┐
│ 1  GND   (Black)│
│ 2  TX    (Green)│  ← LD2450 transmits to ESP32
│ 3  RX    (White)│  ← LD2450 receives from ESP32
│ 4  VCC   (Red)  │  ← 5V power supply
└─────────────────┘
```

### Wiring to ESP32 CYD

```
LD2450          ESP32 CYD (CN1)         Wire Color (suggested)
──────          ───────────────         ──────────────────────
Pin 1: GND  →   CN1 Pin 5 (GND)        Black
Pin 2: TX   →   CN1 Pin 2 (GPIO 22)    Green or Yellow
Pin 3: RX   →   CN1 Pin 4 (GPIO 27)    Blue or White
Pin 4: VCC  →   5V rail                Red
```

### UART Configuration

**Hardware Serial Setup:**
```cpp
// ESP32 Arduino
HardwareSerial LD2450Serial(1);  // Use UART1
LD2450Serial.begin(256000, SERIAL_8N1, 22, 27);
//                  Baud    Config    RX  TX
```

**Key Points:**
- LD2450 TX → ESP32 RX (GPIO 22) - **Radar sends data to ESP32**
- LD2450 RX → ESP32 TX (GPIO 27) - **ESP32 sends commands to radar**
- Default baud rate: **256000 bps**
- Protocol: 8 data bits, no parity, 1 stop bit (8N1)

### Physical Mounting

- Keep LD2450 away from metal objects (affects radar performance)
- Mount with antenna facing forward (detection direction)
- Minimum 5mm clearance around antenna area
- Secure with double-sided foam tape or M2 screws

---

## Audio System Connection

### PAM8403 Amplifier Module

```
PAM8403 Module Pinout:
┌─────────────────────────────┐
│  [L+] [L-]  [R+] [R-]       │  ← Speaker outputs
│                             │
│  [IN_L] [IN_R]              │  ← Audio inputs
│                             │
│  [VCC]  [GND]               │  ← Power supply
│                             │
│  [Volume Potentiometer]     │  ← Volume control (onboard)
└─────────────────────────────┘
```

### Wiring Configuration

```
PAM8403 Amp      Connection                        Notes
───────────      ──────────                        ─────
VCC          →   5V rail                           Power supply
GND          →   Common GND                        Ground reference
IN_L         →   ESP32 GPIO 25 (DAC1)             Left channel audio input
IN_R         →   GND (or leave disconnected)      Mono audio - tie to GND
L+           →   Speaker (+) Red wire              Left output to speaker
L-           →   Speaker (-) Black wire            Left output ground
R+           →   Not connected                     Right channel unused
R-           →   Not connected                     Right channel unused
```

### Speaker Connection

```
8Ω Speaker Wiring:
         PAM8403
       ┌─────────┐
       │   L+    │────[Red]────→ Speaker (+)
       │         │
       │   L-    │────[Black]──→ Speaker (-)
       └─────────┘

Note: Polarity matters for best sound quality
```

### Optional Volume Control

If PAM8403 doesn't have onboard pot, add external 10kΩ potentiometer:

```
ESP32 GPIO 25 ──[10kΩ Pot]── PAM8403 IN_L
                    │
                   GND

Pot connections:
  Pin 1: ESP32 GPIO 25 (DAC1 output)
  Pin 2: PAM8403 IN_L (wiper/center pin)
  Pin 3: GND
```

### Audio Signal Information

- **ESP32 DAC Output**: 0-3.3V analog signal
- **Frequency Range**: 20Hz - 20kHz
- **Resolution**: 8-bit (256 levels)
- **Sample Rate**: Up to 80 kHz (use 22-44 kHz for audio)

---

## 5-Way Navigation Switch

### Switch Breakout Board Pinout

```
5-Way Switch Breakout:
┌──────────────────┐
│  [VCC]   [GND]   │  ← Power supply
│                  │
│  [UP]   [DOWN]   │  ← Direction outputs
│  [LEFT] [RIGHT]  │
│  [CENTER]        │  ← Push/select button
└──────────────────┘

All outputs are active-LOW with pull-ups:
  - Released (default): HIGH (3.3V)
  - Pressed: LOW (0V/GND)
```

### Wiring to ESP32

```
5-Way Switch     ESP32 GPIO              Function
────────────     ──────────              ────────
VCC          →   3.3V (from CYD)         Power supply (NOT 5V!)
GND          →   Common GND              Ground reference
UP           →   GPIO 4                  Navigate up / increase
DOWN         →   GPIO 16                 Navigate down / decrease
LEFT         →   GPIO 17                 Navigate left / previous
RIGHT        →   GPIO 21 (CN1 Pin 3)     Navigate right / next
CENTER       →   GPIO 2                  Select / confirm / push
```

### Pin Access Notes

**Freed from RGB LED** (after removal):
- GPIO 4 - Was RGB Red
- GPIO 16 - Was RGB Green
- GPIO 17 - Was RGB Blue

**From CN1 Connector:**
- GPIO 21 - CN1 Pin 3
- GPIO 2 - Available internal pin (may need wire soldering)

### Software Configuration

```cpp
// Button pin definitions
#define BTN_UP     4
#define BTN_DOWN   16
#define BTN_LEFT   17
#define BTN_RIGHT  21
#define BTN_CENTER 2

// Setup in code
void setup() {
  // Configure as INPUT_PULLUP (redundant with hardware pullups, but safe)
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
}

// Reading buttons
void loop() {
  if (digitalRead(BTN_UP) == LOW) {
    // UP button pressed
  }
  // etc...
}
```

### Debouncing

Mechanical switches require debouncing. Options:

**Option 1: Software delay**
```cpp
if (digitalRead(BTN_UP) == LOW) {
  delay(50);  // 50ms debounce
  if (digitalRead(BTN_UP) == LOW) {
    // Confirmed button press
  }
}
```

**Option 2: ezButton library** (recommended)
```cpp
#include <ezButton.h>

ezButton btnUp(4);
btnUp.setDebounceTime(50);  // 50ms

void loop() {
  btnUp.loop();
  if (btnUp.isPressed()) {
    // Button pressed
  }
}
```

---

## Battery Monitoring

Two options for battery level monitoring:

### Option 1: Hardware LED Monitor (Recommended)

**No GPIO pins required** - standalone display.

```
LED Battery Monitor Wiring:
┌──────────────────┐
│  [IN+]   [IN-]   │
│                  │
│  [LED1] [LED2]   │  ← Visual indicators
│  [LED3] [LED4]   │     (3-4 LEDs typical)
└──────────────────┘

Connections:
  IN+  →  18650 Battery Positive (before TP4056)
  IN-  →  18650 Battery Negative / GND

LED Indication (typical):
  4.2-4.0V: 4 LEDs (Full)
  4.0-3.8V: 3 LEDs (Good)
  3.8-3.6V: 2 LEDs (Medium)
  3.6-3.4V: 1 LED  (Low)
  <3.4V:    0 LEDs (Empty)
```

**Advantages:**
- No GPIO pins used
- Always visible (no software needed)
- Simple wiring
- Real battery voltage (not boosted 5V)

### Option 2: Software ADC Monitoring (Optional)

Use ESP32 GPIO 35 (ADC) for software-based monitoring.

```
Voltage Divider Circuit:
┌────────────────────────────────┐
│   Battery Voltage Monitoring    │
└────────────────────────────────┘

Battery + (3.0-4.2V)
     │
     ├─── 100kΩ Resistor ───┐
     │                      │
     │                      ├──→ ESP32 GPIO 35 (ADC)
     │                      │     (CN1 Pin 1)
     │                      │
     └─── 100kΩ Resistor ───┴──→ GND

Voltage Division:
  Battery 4.2V → GPIO 35 reads 2.1V (Full)
  Battery 3.7V → GPIO 35 reads 1.85V (Nominal)
  Battery 3.0V → GPIO 35 reads 1.5V (Empty)
```

**Software Reading:**
```cpp
#define BATTERY_PIN 35
#define VREF 3.3
#define ADC_RES 4096

float readBatteryVoltage() {
  int adcValue = analogRead(BATTERY_PIN);
  float voltage = (adcValue / (float)ADC_RES) * VREF * 2.0;  // ×2 for divider
  return voltage;
}

int getBatteryPercent() {
  float voltage = readBatteryVoltage();
  // Map 3.0V-4.2V to 0%-100%
  int percent = (voltage - 3.0) / (4.2 - 3.0) * 100;
  return constrain(percent, 0, 100);
}
```

**Advantages:**
- Display battery % on screen
- Low battery warnings
- Data logging capability

**Disadvantages:**
- Uses GPIO 35
- Requires calibration
- Software overhead

**Recommendation**: Use hardware LED monitor for simplicity, add software monitoring later if desired.

---

## Complete System Diagram

### Full System Wiring Overview

```
┌──────────────────────────────────────────────────────────────────────┐
│                    CYD HANDHELD RADAR - COMPLETE WIRING               │
└──────────────────────────────────────────────────────────────────────┘

                          18650 Battery
                          (3.7V 3000mAh)
                                │
                    ┌───────────┴───────────┐
                    │                       │
              LED Monitor              TP4056 Charger
              (Visual only)            (with protection)
                    │                       │
                    │                  Micro USB ← Charging
                    │                       │
                    │                  Power Switch
                    │                       │
                    │                  MT3608 Boost
                    │                  (3.7V → 5V)
                    │                       │
                    └───────────┬───────────┴─────────────┐
                                │                         │
                            5V Rail                    Common GND
                                │                         │
        ┌───────────────────────┼─────────┬───────────────┼──────────┐
        │                       │         │               │          │
        ↓                       ↓         ↓               ↓          ↓
   ESP32 CYD              HLK-LD2450   PAM8403      5-Way Switch  Speaker
  ┌──────────┐           ┌─────────┐  Amplifier    ┌──────────┐      │
  │          │           │ Pin1:GND├──→ GND        │ VCC:3.3V │      │
  │ GPIO 22 ←├───────────┤ Pin2:TX │   │           │ GND      │      │
  │ GPIO 27 ├───────────→│ Pin3:RX │   │           │ UP   ────├──→ GPIO 4
  │ GPIO 25 ├───────────→│ Pin4:VCC├──→ 5V         │ DOWN ────├──→ GPIO 16
  │ GPIO 4  ←├───────────┤         │   │           │ LEFT ────├──→ GPIO 17
  │ GPIO 16 ←├───────────┤         │   │           │ RIGHT────├──→ GPIO 21
  │ GPIO 17 ←├───────────┤         │   │           │ CENTER───├──→ GPIO 2
  │ GPIO 21 ←├───────────┤         │   │           └──────────┘
  │ GPIO 2  ←├───────────┤         │   │
  │ GPIO 35 ←├─ Optional │         │   ├─IN_L ← GPIO 25
  │ 5V       │            │         │   ├─GND
  │ GND──────┼────────────┴─────────┴───┴─────────────────────────────┘
  └──────────┘                         ├─L+ ───→ Speaker (+)
                                       └─L- ───→ Speaker (-)
```

### Connection Summary Table

| From Component | From Pin | To Component | To Pin | Wire Color |
|---------------|----------|--------------|--------|------------|
| Battery | + | TP4056 | BAT+ | Red |
| Battery | - | TP4056 | BAT- | Black |
| Battery | + | LED Monitor | IN+ | Red |
| Battery | - | LED Monitor | IN- | Black |
| TP4056 | OUT+ | Power Switch | IN | Red |
| Switch | OUT | MT3608 | IN+ | Red |
| TP4056 | OUT- | MT3608 | IN- | Black |
| MT3608 | OUT+ | ESP32 CYD | 5V | Red |
| MT3608 | OUT+ | LD2450 | VCC | Red |
| MT3608 | OUT+ | PAM8403 | VCC | Red |
| MT3608 | OUT+ | 5-Way | VCC | Red (3.3V from CYD!) |
| MT3608 | OUT- | All | GND | Black |
| LD2450 | TX | ESP32 | GPIO 22 | Yellow |
| LD2450 | RX | ESP32 | GPIO 27 | Blue |
| ESP32 | GPIO 25 | PAM8403 | IN_L | White |
| PAM8403 | L+ | Speaker | + | Red |
| PAM8403 | L- | Speaker | - | Black |
| 5-Way | UP | ESP32 | GPIO 4 | Orange |
| 5-Way | DOWN | ESP32 | GPIO 16 | Green |
| 5-Way | LEFT | ESP32 | GPIO 17 | Blue |
| 5-Way | RIGHT | ESP32 | GPIO 21 | Yellow |
| 5-Way | CENTER | ESP32 | GPIO 2 | White |

---

## Wire Color Conventions

### Recommended Color Scheme

**Power:**
- Red: Positive voltage (battery +, 5V, 3.3V)
- Black: Ground (GND, negative -)
- Yellow: Regulated 5V distribution

**UART/Serial:**
- Yellow/Green: TX (transmit)
- Blue/White: RX (receive)

**Digital I/O:**
- Orange: Button 1 (UP)
- Green: Button 2 (DOWN)
- Blue: Button 3 (LEFT)
- Yellow: Button 4 (RIGHT)
- White: Button 5 (CENTER)

**Analog:**
- Purple/Violet: Analog signals (ADC, DAC, audio)

---

## Testing & Troubleshooting

### Pre-Power Checklist

Before applying power, verify:

```
[ ] Battery polarity correct (+ to +, - to -)
[ ] TP4056 BAT+/BAT- connected correctly
[ ] MT3608 output adjusted to 5.0V
[ ] No short circuits between 5V and GND (use multimeter continuity)
[ ] LD2450 TX → ESP32 RX (GPIO 22)
[ ] LD2450 RX → ESP32 TX (GPIO 27)
[ ] Speaker impedance is 8Ω (not 4Ω)
[ ] All GND connections common
[ ] Power switch in OFF position
[ ] No loose wires or exposed conductors
```

### Power-On Testing Sequence

**Step 1: Battery & Boost Converter**
1. Insert charged 18650 battery
2. Turn power switch ON
3. Measure 5V rail with multimeter
4. Expected: 5.00V ± 0.05V
5. If incorrect, adjust MT3608 potentiometer

**Step 2: ESP32 CYD**
1. Connect USB cable to CYD (for serial monitoring)
2. CYD display should light up
3. Upload basic test sketch
4. Monitor serial output (115200 baud)

**Step 3: LD2450 Radar**
1. Upload LD2450 test code
2. Open serial monitor (256000 baud for LD2450 data)
3. Wave hand in front of sensor
4. Expected: Distance and position data

**Step 4: Audio System**
1. Upload tone generation test code
2. Generate 1kHz test tone on GPIO 25
3. Expected: Tone from speaker
4. Adjust PAM8403 volume pot

**Step 5: Navigation Buttons**
1. Upload button test code
2. Press each button
3. Expected: Serial output shows button presses
4. Verify all 5 directions work

### Common Issues & Solutions

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| CYD doesn't power on | Wrong voltage, no power | Check MT3608 output is 5.0V |
| Display is dim | Low battery, wrong voltage | Charge battery, verify 5V rail |
| LD2450 no data | Wrong UART pins, baud rate | Verify GPIO 22/27, use 256000 baud |
| No audio output | DAC not configured, amp issue | Check GPIO 25 connection, PAM8403 power |
| Buttons not responding | Pull-up not configured, bad connections | Check INPUT_PULLUP mode, verify wiring |
| Battery drains fast | Short circuit, high current | Measure current draw, check for shorts |
| Charging not working | TP4056 fault, battery issue | Check TP4056 LEDs, verify battery |

### Voltage Measurement Points

Use multimeter to check:

```
Test Point                Expected Voltage      Notes
──────────────────────    ────────────────      ─────
Battery terminals         3.0-4.2V              Depends on charge state
TP4056 OUT+              3.0-4.2V              Same as battery (protection on)
MT3608 IN+               3.0-4.2V              Same as TP4056 output
MT3608 OUT+              5.00V ± 0.05V         CRITICAL - adjust if needed
ESP32 5V pin             5.00V                 Must match MT3608 output
ESP32 3.3V pin           3.30V ± 0.05V         Regulated by CYD onboard
GPIO 25 (DAC)            0-3.3V                Varies with audio signal
GPIO 35 (battery)        1.5-2.1V              Half of battery voltage
LD2450 VCC               5.00V                 Must be 5V for operation
```

### Current Draw Measurement

Measure current at different states:

```
State                    Expected Current      Action if Exceeded
─────────────────────    ────────────────      ──────────────────
Idle (screen off)        150-200mA             Check for shorts
Display on               200-300mA             Normal
Radar scanning           300-400mA             Normal
Audio playing            400-600mA             Normal
Peak (all active)        600-800mA             Check speaker impedance
Sleep mode               50-100mA              Future optimization
```

### Signal Testing

**UART Communication (LD2450):**
```cpp
// Test code
void setup() {
  Serial.begin(115200);
  Serial1.begin(256000, SERIAL_8N1, 22, 27);
}

void loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());  // Echo LD2450 data to USB serial
  }
}
```

**Button Testing:**
```cpp
void loop() {
  Serial.print("UP:");    Serial.print(digitalRead(4));
  Serial.print(" DOWN:"); Serial.print(digitalRead(16));
  Serial.print(" LEFT:"); Serial.print(digitalRead(17));
  Serial.print(" RIGHT:");Serial.print(digitalRead(21));
  Serial.print(" CTR:");  Serial.println(digitalRead(2));
  delay(100);
}
```

### Safety Notes

- ⚠️ Never exceed 5.5V on ESP32 or LD2450 (will damage components)
- ⚠️ Do not reverse battery polarity (will damage TP4056)
- ⚠️ Check for shorts before applying power
- ⚠️ Use protected 18650 cells only
- ⚠️ Do not charge damaged or swollen batteries

---

## Modification Notes

### RGB LED Removal

The RGB LED on ESP32 CYD uses GPIO 4, 16, 17. Remove it to free these pins:

**Location:** Small SMD LED near the display connector

**Removal procedure:**
1. Heat all three LED pads simultaneously with soldering iron
2. Gently lift LED with tweezers
3. Clean pads with solder wick
4. Verify pins are no longer connected to LED (continuity test)

**Alternative:** Cut traces to LED instead of removing component (more difficult)

### CN1 Pin Header Installation

**Location:** 5-pin footprint labeled "CN1" on CYD board

**Installation:**
1. Insert 5-pin male header (2.54mm spacing) into CN1 holes
2. Ensure header is perpendicular to board
3. Solder all 5 pins
4. Verify no shorts between adjacent pins

**Recommended:** Right-angle header for easier access in enclosure

---

**Last Updated**: 2026-01-13
**Revision**: 1.0
