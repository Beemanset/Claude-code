# Wiring Guide - CYD Handheld Radar

Complete wiring diagrams and pin connection guide for the M314-inspired motion tracker.

> **IMPORTANT**: This guide has been updated to match the ESP32-2432S028R (CYD) board pinout. Previous versions had GPIO conflicts with the touchscreen and display backlight pins.

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
| **GPIO 22** | UART RX | Input | LD2450 TX | I/O | Serial Port connector or Expansion IO1 |
| **GPIO 27** | UART TX | Output | LD2450 RX | I/O | Serial Port connector or Expansion IO2 |
| **GPIO 26** | DAC Audio | Output | PAM8403 IN_L or Built-in Speaker | DAC | **Corrected** - Board's audio output pin |
| **GPIO 4** | Button UP | Input | 5-way UP | I/O | Freed from RGB LED |
| **GPIO 16** | Button DOWN | Input | 5-way DOWN | I/O | Freed from RGB LED |
| **GPIO 17** | Button LEFT | Input | 5-way LEFT | I/O | Freed from RGB LED |
| **GPIO 35** | Button RIGHT | Input | 5-way RIGHT | Input-only | Expansion IO1 header |
| **GPIO 5** | Button CENTER | Input | 5-way CENTER | I/O | Available if SD card not used |

### Reserved/Used Pins (Do Not Modify)

| GPIO | Function | Used By | Notes |
|------|----------|---------|-------|
| GPIO 14 | SPI CLK | ILI9341 TFT | Display clock signal |
| GPIO 13 | SPI MOSI | ILI9341 TFT | Display data out |
| GPIO 12 | SPI MISO | ILI9341 TFT | Display data in |
| GPIO 15 | TFT CS | ILI9341 TFT | Display chip select |
| GPIO 2 | TFT DC | ILI9341 TFT | Display data/command - **NOT available for buttons** |
| GPIO 21 | Backlight LED | Display | Screen backlight control - **NOT available for buttons** |
| GPIO 25 | TP CLK | XPT2046 Touch | Touch panel clock - **NOT available for audio** |
| GPIO 33 | TP CS | XPT2046 Touch | Touchscreen chip select |
| GPIO 32 | TP DIN | XPT2046 Touch | Touch panel data in |
| GPIO 39 | TP OUT | XPT2046 Touch | Touch panel data out |
| GPIO 36 | TP IRQ | XPT2046 Touch | Touch panel interrupt |
| GPIO 0 | Boot mode | System | Programming/boot selection |
| GPIO 1 | TX0 | USB Serial | Debug UART transmit |
| GPIO 3 | RX0 | USB Serial | Debug UART receive |

### Board Connectors Overview

The ESP32-2432S028R has several connectors:

```
┌─────────────────────────────────────────────────────────────┐
│                    ESP32-2432S028R (CYD)                    │
│                                                             │
│  LEFT SIDE:                      RIGHT SIDE:                │
│  ┌─────────────────┐             ┌─────────────────────┐    │
│  │ Serial Port     │             │ Expansion IO1       │    │
│  │ 4p 1.25mm       │             │ 4p 1.25mm           │    │
│  │ VIN, TX, RX, GND│             │ GND, IO35, IO22,    │    │
│  └─────────────────┘             │ IO21                │    │
│                                  └─────────────────────┘    │
│  ┌─────────────────┐             ┌─────────────────────┐    │
│  │ Speaker         │             │ Expansion IO2       │    │
│  │ 2p 1.25mm       │             │ 4p 1.25mm           │    │
│  │ 1.5W 4Ω         │             │ GND, IO22, IO27,    │    │
│  └─────────────────┘             │ 3.3V                │    │
│                                  └─────────────────────┘    │
│  Audio: IO26                                                │
│                                  USB: Type-C & Micro        │
└─────────────────────────────────────────────────────────────┘
```

### Expansion IO1 Pinout (4 pins, 1.25mm pitch)

```
Expansion IO1 Header:
┌─────────────┐
│ 1  GND      │ ← Ground
│ 2  GPIO 35  │ ← Input only (ADC1_CH7) - Use for button
│ 3  GPIO 22  │ ← I2C SCL / UART RX for radar
│ 4  GPIO 21  │ ← I2C SDA / Backlight (DO NOT USE)
└─────────────┘
```

### Expansion IO2 Pinout (4 pins, 1.25mm pitch)

```
Expansion IO2 Header:
┌─────────────┐
│ 1  GND      │ ← Ground
│ 2  GPIO 22  │ ← I2C SCL / UART RX for radar
│ 3  GPIO 27  │ ← TOUCH7/ADC2_CH7 / UART TX for radar
│ 4  3.3V     │ ← 3.3V power output
└─────────────┘
```

### Serial Port Connector (4 pins, 1.25mm pitch)

```
Serial Port Header (LEFT SIDE):
┌─────────────┐
│ 1  VIN      │ ← 5V input
│ 2  TX       │ ← GPIO 27 (ESP32 transmits)
│ 3  RX       │ ← GPIO 22 (ESP32 receives)
│ 4  GND      │ ← Ground
└─────────────┘

This connector is ideal for the LD2450 radar connection!
```

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
          └─────→ PAM8403 (VCC)             [50-250mA] (if used)

GND Common:
     MT3608 OUT-
          │
          ├─────→ ESP32 CYD (GND)
          │
          ├─────→ LD2450 (GND)
          │
          ├─────→ PAM8403 (GND)
          │
          └─────→ Speaker (-) [if using built-in connector]
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

### Wiring to ESP32 CYD (Recommended: Use Serial Port Connector)

**Option 1: Serial Port Connector (Easiest)**

The CYD board has a built-in 4-pin serial port connector that maps directly to the radar:

```
LD2450          CYD Serial Port Connector     Wire Color (suggested)
──────          ─────────────────────────     ──────────────────────
Pin 1: GND  →   Pin 4: GND                    Black
Pin 2: TX   →   Pin 3: RX (GPIO 22)           Green or Yellow
Pin 3: RX   →   Pin 2: TX (GPIO 27)           Blue or White
Pin 4: VCC  →   Pin 1: VIN (5V)               Red

Note: May need JST 1.25mm to JST 1.25mm cable or adapter
```

**Option 2: Expansion Headers (Alternative)**

```
LD2450          ESP32 CYD Expansion           Wire Color (suggested)
──────          ───────────────────           ──────────────────────
Pin 1: GND  →   Expansion IO1 Pin 1 (GND)    Black
Pin 2: TX   →   Expansion IO1 Pin 3 (GPIO 22) Green or Yellow
Pin 3: RX   →   Expansion IO2 Pin 3 (GPIO 27) Blue or White
Pin 4: VCC  →   5V rail (external)            Red
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

### Option 1: Built-in Speaker Connector (Simplest)

The CYD board has a **built-in 2-pin speaker connector** (1.25mm pitch, 1.5W, 4Ω):

```
CYD Built-in Speaker Connector:
┌─────────────────┐
│ Speaker +       │ ← Connect to speaker positive
│ Speaker -       │ ← Connect to speaker negative (GND)
└─────────────────┘

Audio output is on GPIO 26 (internal connection)
```

**Wiring:**
```
CYD Speaker Connector     Speaker
─────────────────────     ───────
Pin 1 (+)             →   Speaker (+) Red wire
Pin 2 (-)             →   Speaker (-) Black wire

Recommended speaker: 4Ω, 1.5W or less
```

### Option 2: External PAM8403 Amplifier (Louder Audio)

Use this option if you need more volume or are using an 8Ω speaker:

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

### PAM8403 Wiring Configuration

```
PAM8403 Amp      Connection                        Notes
───────────      ──────────                        ─────
VCC          →   5V rail                           Power supply
GND          →   Common GND                        Ground reference
IN_L         →   ESP32 GPIO 26 (DAC)              Audio output (CORRECTED!)
IN_R         →   GND (or leave disconnected)      Mono audio - tie to GND
L+           →   Speaker (+) Red wire              Left output to speaker
L-           →   Speaker (-) Black wire            Left output ground
R+           →   Not connected                     Right channel unused
R-           →   Not connected                     Right channel unused
```

### Speaker Connection

```
8Ω Speaker Wiring (with PAM8403):
         PAM8403
       ┌─────────┐
       │   L+    │────[Red]────→ Speaker (+)
       │         │
       │   L-    │────[Black]──→ Speaker (-)
       └─────────┘

Note: Polarity matters for best sound quality
```

### Audio Signal Information

- **ESP32 DAC Output Pin**: **GPIO 26** (NOT GPIO 25!)
- **GPIO 25 is reserved**: Touch panel clock (TP CLK)
- **Output Voltage**: 0-3.3V analog signal
- **Frequency Range**: 20Hz - 20kHz
- **Resolution**: 8-bit (256 levels)
- **Sample Rate**: Up to 80 kHz (use 22-44 kHz for audio)

---

## 5-Way Navigation Switch

### Updated Button Assignments

Due to pin conflicts on the CYD board, the button assignments have been revised:

| Button | GPIO | Source | Notes |
|--------|------|--------|-------|
| UP | GPIO 4 | RGB LED (freed) | Remove/disable RGB LED |
| DOWN | GPIO 16 | RGB LED (freed) | Remove/disable RGB LED |
| LEFT | GPIO 17 | RGB LED (freed) | Remove/disable RGB LED |
| RIGHT | GPIO 35 | Expansion IO1 | Input-only pin, needs 10kΩ external pull-up |
| CENTER | GPIO 0 or 5 | See below | Depends on SD card usage |

### CENTER Button Options (Important!)

The CENTER button GPIO depends on whether you're using the SD card:

| SD Card | CENTER GPIO | Notes |
|---------|-------------|-------|
| **Using SD card** | GPIO 0 (BOOT) | May need to hold during programming |
| **Not using SD** | GPIO 5 | Preferred - no programming issues |

**To configure:** Edit `config.h` and set `USE_SD_CARD`:
```cpp
#define USE_SD_CARD  true   // Using SD card → CENTER on GPIO 0
#define USE_SD_CARD  false  // No SD card → CENTER on GPIO 5
```

**GPIO 0 (BOOT button) notes:**
- This is the physical BOOT button on the CYD board
- If programming fails, hold the BOOT button while uploading
- Works reliably as a menu/select button during normal operation

**Why the changes?**
- GPIO 21: Used by screen backlight - cannot be repurposed
- GPIO 2: Used by TFT DC (data/command) - cannot be repurposed
- GPIO 25: Used by touch panel clock - cannot be repurposed
- GPIO 5: Used by SD card CD/DAT3 - only available if SD card not used

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
5-Way Switch     ESP32 GPIO              Source/Notes
────────────     ──────────              ────────────
VCC          →   3.3V (from CYD)         Power supply (NOT 5V!)
GND          →   Common GND              Ground reference
UP           →   GPIO 4                  Freed from RGB LED
DOWN         →   GPIO 16                 Freed from RGB LED
LEFT         →   GPIO 17                 Freed from RGB LED
RIGHT        →   GPIO 35                 Expansion IO1 (input-only)
CENTER       →   GPIO 5                  SD card pin (if SD unused)
```

### Alternative CENTER Button Options

If you need the SD card, use one of these alternatives for CENTER:

| Option | GPIO | Notes |
|--------|------|-------|
| SD Card unused | GPIO 5 | Best option - easy access |
| Boot button | GPIO 0 | Works but affects programming |
| External interrupt | GPIO 36 (TP IRQ) | May conflict with touch |

### Pin Access Notes

**Freed from RGB LED** (requires removal or code disable):
- GPIO 4 - Was RGB Red
- GPIO 16 - Was RGB Green
- GPIO 17 - Was RGB Blue

**From Expansion IO1:**
- GPIO 35 - Input-only pin, perfect for button input

**From SD Card (TF Card slot):**
- GPIO 5 (CD/DAT3) - Available if SD card not used
- GPIO 18, 19, 23 also available if SD not used

### Software Configuration

```cpp
// Updated button pin definitions
#define BTN_UP     4
#define BTN_DOWN   16
#define BTN_LEFT   17
#define BTN_RIGHT  35   // Input-only pin
#define BTN_CENTER 5    // SD card pin (if unused)

// Setup in code
void setup() {
  // Configure as INPUT_PULLUP
  // Note: GPIO 35 doesn't have internal pull-up, add external 10kΩ to 3.3V
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT);  // GPIO 35 - add external pull-up!
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

### External Pull-up for GPIO 35

GPIO 35 is input-only and lacks internal pull-up. Add external resistor:

```
3.3V ──[10kΩ]──┬── GPIO 35
               │
         5-way RIGHT pin
               │
         (switch to GND when pressed)
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

**Note:** If using GPIO 35 for RIGHT button, you cannot also use it for battery monitoring. Choose one or the other.

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
     │                      │     (Expansion IO1 Pin 2)
     │                      │
     └─── 100kΩ Resistor ───┴──→ GND

Voltage Division:
  Battery 4.2V → GPIO 35 reads 2.1V (Full)
  Battery 3.7V → GPIO 35 reads 1.85V (Nominal)
  Battery 3.0V → GPIO 35 reads 1.5V (Empty)
```

**Recommendation**: Use hardware LED monitor for simplicity, add software monitoring later if desired.

---

## Complete System Diagram

### Full System Wiring Overview

```
┌──────────────────────────────────────────────────────────────────────┐
│                    CYD HANDHELD RADAR - COMPLETE WIRING               │
│                    (Updated for ESP32-2432S028R pinout)               │
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
  ┌──────────┐           ┌─────────┐  (optional)   ┌──────────┐      │
  │          │           │ Pin1:GND├──→ GND        │ VCC:3.3V │      │
  │ GPIO 22 ←├───────────┤ Pin2:TX │   │           │ GND      │      │
  │ GPIO 27 ─├───────────→ Pin3:RX │   │           │ UP   ────├──→ GPIO 4
  │ GPIO 26 ─├───────────→ IN_L    │   │           │ DOWN ────├──→ GPIO 16
  │ GPIO 4  ←├───────────┤         │   │           │ LEFT ────├──→ GPIO 17
  │ GPIO 16 ←├───────────┤         │   │           │ RIGHT────├──→ GPIO 35*
  │ GPIO 17 ←├───────────┤         │   │           │ CENTER───├──→ GPIO 5**
  │ GPIO 35 ←├───────────┤         │   │           └──────────┘
  │ GPIO 5  ←├───────────┤         │   │
  │ 5V       │           │ Pin4:VCC├──→ 5V          * Add 10kΩ pull-up
  │ GND──────┼───────────┴─────────┴───┴──────────  ** If SD card unused
  └──────────┘                         │
       │                               ├─L+ ───→ Speaker (+)
       └─ Built-in Speaker Connector ──┴─L- ───→ Speaker (-)
          (Alternative to PAM8403)
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
| MT3608 | OUT+ | PAM8403 | VCC | Red (if used) |
| MT3608 | OUT- | All | GND | Black |
| LD2450 | TX | ESP32 | GPIO 22 | Yellow |
| LD2450 | RX | ESP32 | GPIO 27 | Blue |
| ESP32 | GPIO 26 | PAM8403/Speaker | IN_L/+ | White |
| PAM8403 | L+ | Speaker | + | Red |
| PAM8403 | L- | Speaker | - | Black |
| 5-Way | UP | ESP32 | GPIO 4 | Orange |
| 5-Way | DOWN | ESP32 | GPIO 16 | Green |
| 5-Way | LEFT | ESP32 | GPIO 17 | Blue |
| 5-Way | RIGHT | ESP32 | GPIO 35 | Yellow |
| 5-Way | CENTER | ESP32 | GPIO 0 (SD) or 5 (no SD) | White |

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
[ ] Audio connected to GPIO 26 (NOT GPIO 25!)
[ ] Speaker impedance matches (4Ω for built-in, 8Ω for PAM8403)
[ ] All GND connections common
[ ] Power switch in OFF position
[ ] No loose wires or exposed conductors
[ ] External 10kΩ pull-up on GPIO 35 (if using for button)
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
2. Generate 1kHz test tone on **GPIO 26**
3. Expected: Tone from speaker
4. Adjust PAM8403 volume pot (if used)

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
| No audio output | Wrong GPIO pin! | Use **GPIO 26**, not GPIO 25 |
| Touch not working | Audio on wrong pin | GPIO 25 is touch clock - use GPIO 26 for audio |
| Buttons not responding | Pull-up issue | GPIO 35 needs external 10kΩ pull-up |
| RIGHT button stuck | Missing pull-up on GPIO 35 | Add 10kΩ resistor to 3.3V |
| Battery drains fast | Short circuit, high current | Measure current draw, check for shorts |
| Charging not working | TP4056 fault, battery issue | Check TP4056 LEDs, verify battery |
| Screen backlight off | GPIO 21 accidentally used | Don't use GPIO 21 - it's for backlight |

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
GPIO 26 (DAC)            0-3.3V                Varies with audio signal
LD2450 VCC               5.00V                 Must be 5V for operation
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

**Button Testing (Updated pins):**
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(4, INPUT_PULLUP);   // UP
  pinMode(16, INPUT_PULLUP);  // DOWN
  pinMode(17, INPUT_PULLUP);  // LEFT
  pinMode(35, INPUT);         // RIGHT - add external pull-up!
  pinMode(5, INPUT_PULLUP);   // CENTER
}

void loop() {
  Serial.print("UP:");    Serial.print(digitalRead(4));
  Serial.print(" DOWN:"); Serial.print(digitalRead(16));
  Serial.print(" LEFT:"); Serial.print(digitalRead(17));
  Serial.print(" RIGHT:");Serial.print(digitalRead(35));
  Serial.print(" CTR:");  Serial.println(digitalRead(5));
  delay(100);
}
```

**Audio Testing (Corrected pin):**
```cpp
void setup() {
  // Use GPIO 26 for audio - NOT GPIO 25!
  ledcSetup(0, 1000, 8);      // Channel 0, 1kHz, 8-bit
  ledcAttachPin(26, 0);       // GPIO 26 for audio
}

void loop() {
  ledcWrite(0, 127);          // 50% duty = tone
  delay(500);
  ledcWrite(0, 0);            // Silence
  delay(500);
}
```

### Safety Notes

- Do not exceed 5.5V on ESP32 or LD2450 (will damage components)
- Do not reverse battery polarity (will damage TP4056)
- Check for shorts before applying power
- Use protected 18650 cells only
- Do not charge damaged or swollen batteries

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

**Alternative:** Disable RGB LED in software (simpler, but pins still have LED load)

### Expansion Header Installation

If headers aren't pre-installed:

**Expansion IO1 & IO2:** 4-pin 1.25mm pitch headers
**Serial Port:** 4-pin 1.25mm pitch header

**Installation:**
1. Insert header into holes
2. Ensure header is perpendicular to board
3. Solder all pins
4. Verify no shorts between adjacent pins

---

## Quick Reference Card

```
┌────────────────────────────────────────┐
│     CYD MOTION TRACKER - QUICK REF     │
├────────────────────────────────────────┤
│ RADAR:                                 │
│   RX ← GPIO 22 (Serial Port Pin 3)     │
│   TX → GPIO 27 (Serial Port Pin 2)     │
│   Baud: 256000                         │
├────────────────────────────────────────┤
│ AUDIO:                                 │
│   DAC → GPIO 26 (NOT 25!)              │
│   Built-in speaker: 4Ω 1.5W            │
├────────────────────────────────────────┤
│ BUTTONS:                               │
│   UP     → GPIO 4  (RGB freed)         │
│   DOWN   → GPIO 16 (RGB freed)         │
│   LEFT   → GPIO 17 (RGB freed)         │
│   RIGHT  → GPIO 35 (+ 10kΩ pullup!)    │
│   CENTER → GPIO 0  (if using SD card)  │
│         → GPIO 5  (if NO SD card)      │
├────────────────────────────────────────┤
│ CONFIG (config.h):                     │
│   USE_SD_CARD = true  → CENTER=GPIO 0  │
│   USE_SD_CARD = false → CENTER=GPIO 5  │
├────────────────────────────────────────┤
│ DO NOT USE:                            │
│   GPIO 21 - Backlight                  │
│   GPIO 25 - Touch CLK                  │
│   GPIO 2  - TFT DC                     │
└────────────────────────────────────────┘
```

---

**Last Updated**: 2026-01-20
**Revision**: 2.0 - Updated for actual ESP32-2432S028R pinout
