# Assembly Guide - CYD Handheld Radar

Step-by-step instructions for building the M314-inspired motion tracker.

---

## Table of Contents

1. [Before You Begin](#before-you-begin)
2. [Phase 1: Board Modifications](#phase-1-board-modifications)
3. [Phase 2: Power System Assembly](#phase-2-power-system-assembly)
4. [Phase 3: Component Wiring](#phase-3-component-wiring)
5. [Phase 4: Testing & Calibration](#phase-4-testing--calibration)
6. [Phase 5: Enclosure Assembly](#phase-5-enclosure-assembly)
6. [Phase 6: Final Assembly](#phase-6-final-assembly)
7. [Troubleshooting](#troubleshooting)

---

## Before You Begin

### Required Tools

**Essential:**
- [ ] Soldering iron (30-60W with fine tip)
- [ ] Solder (60/40 or lead-free)
- [ ] Wire strippers (20-30 AWG)
- [ ] Flush cutters
- [ ] Multimeter (voltage/continuity testing)
- [ ] Small Phillips screwdriver
- [ ] Tweezers (for small components)

**Recommended:**
- [ ] Desoldering pump or wick
- [ ] Helping hands/PCB holder
- [ ] Hot glue gun
- [ ] Heat shrink tubing kit
- [ ] Label maker or masking tape (for wire labels)

### Safety Equipment

- [ ] Safety glasses
- [ ] Well-ventilated workspace
- [ ] Fire-safe work surface
- [ ] First aid kit nearby

### Pre-Assembly Checklist

- [ ] All parts received and verified (see PARTS_LIST.md)
- [ ] Read through entire assembly guide first
- [ ] Workspace is clean and organized
- [ ] All tools are ready
- [ ] Wiring diagram printed or accessible (WIRING_GUIDE.md)

**Estimated Assembly Time**: 3-5 hours (first build)

---

## Phase 1: Board Modifications

### Step 1.1: Remove RGB LED from ESP32 CYD

**Purpose**: Free GPIO 4, 16, 17 for button inputs

**Required:**
- ESP32 CYD board
- Soldering iron
- Desoldering pump or wick
- Tweezers
- Multimeter

**Procedure:**

1. **Locate the RGB LED**
   - Small SMD component near the display connector
   - Usually labeled or near GPIO markings
   - Has 4 pads (common anode + R, G, B)

2. **Heat and Remove**
   ```
   Method 1 (Recommended):
   - Apply flux to all LED pads
   - Heat all three color pads simultaneously
   - Gently lift LED with tweezers once solder melts
   - Remove LED completely

   Method 2 (If difficult):
   - Heat each pad individually
   - Rock LED gently side to side
   - Continue until LED releases
   ```

3. **Clean the Pads**
   - Use desoldering wick to remove excess solder
   - Clean with isopropyl alcohol
   - Pads should be flat and clean

4. **Verify Removal**
   - Use multimeter in continuity mode
   - Check GPIO 4, 16, 17 are NOT connected to each other
   - Verify no shorts to GND or VCC

**Checkpoint**: RGB LED removed, pads clean, no shorts detected

### Step 1.2: Install CN1 Pin Header

**Purpose**: Access GPIO 21, 22, 27, 35 for LD2450 and buttons

**Required:**
- ESP32 CYD board
- 5-pin male header (2.54mm pitch)
- Soldering iron
- Solder

**Procedure:**

1. **Locate CN1 Footprint**
   - 5 holes labeled "CN1" on the board
   - Usually near the edge of the PCB
   - Pinout: GPIO 35, 22, 21, 27, GND (top to bottom)

2. **Choose Header Orientation**
   ```
   Straight Header:     Right-Angle Header:
   ════════════════     ════════════════════
        ║ ║ ║ ║ ║            ┌─┬─┬─┬─┬─┐
        ║ ║ ║ ║ ║            │ │ │ │ │ │
       ═╩═╩═╩═╩═╩═           ═╧═╧═╧═╧═╧═
       CYD Board              CYD Board

   Recommendation: Right-angle for easier access in enclosure
   ```

3. **Install Header**
   - Insert pins through CN1 holes from top side
   - Ensure header is perpendicular (use breadboard to hold straight)
   - Flip board and solder all 5 pins from bottom
   - Apply sufficient solder for good connection
   - Check for cold solder joints (should be shiny, not dull)

4. **Test Installation**
   - Gently tug on header to verify mechanical strength
   - Check no solder bridges between adjacent pins
   - Verify continuity from header pin to corresponding GPIO pad

**Checkpoint**: CN1 header installed, all pins soldered, no bridges

### Step 1.3: Optional GPIO Access

If GPIO 2 is not easily accessible, you may need to solder a wire directly:

1. **Locate GPIO 2 pad** on ESP32 module
2. **Tin a short wire** (5-10cm, 24 AWG)
3. **Carefully solder wire** to GPIO 2 pad
4. **Route wire** to accessible location
5. **Add heat shrink** for insulation

**Checkpoint**: All required GPIO pins accessible

---

## Phase 2: Power System Assembly

### Step 2.1: Prepare Battery Holder

**Procedure:**

1. **Inspect 18650 Holder**
   - Check spring contacts are clean
   - Verify polarity markings (+ and -)

2. **Attach Wires to Holder**
   ```
   If holder has solder tabs:
   - Red wire to positive tab
   - Black wire to negative tab
   - Use 22 AWG stranded wire
   - Solder securely
   - Add heat shrink

   If holder has screw terminals:
   - Strip wire ends 5mm
   - Insert into terminals
   - Tighten screws firmly
   ```

3. **Verify Polarity**
   - Insert test battery (do NOT connect to circuit yet)
   - Measure voltage with multimeter
   - Red wire should be positive (+3.7V)
   - Black wire should be negative (0V/GND)

**Checkpoint**: Battery holder wired with correct polarity

### Step 2.2: Connect TP4056 Charger Module

**Procedure:**

1. **Identify TP4056 Pads**
   ```
   TP4056 Module Layout:
   ┌─────────────────────┐
   │  B+   B-            │  ← Battery input
   │  OUT+ OUT-          │  ← Protected output
   │  [Micro USB Port]   │  ← Charging input
   │  LED1 LED2          │  ← Status indicators
   └─────────────────────┘
   ```

2. **Wire Battery to TP4056**
   - Cut two 10cm wires (red and black, 22 AWG)
   - Tin both ends
   - Solder red wire from battery holder (+) to TP4056 B+
   - Solder black wire from battery holder (-) to TP4056 B-
   - Add heat shrink to connections

3. **Test TP4056**
   - Insert battery into holder (without connecting output yet)
   - Plug micro USB cable into TP4056
   - LED should indicate charging status:
     - Red LED: Charging
     - Blue/Green LED: Charge complete
   - Measure BAT+ to BAT-: Should read battery voltage
   - Measure OUT+ to OUT-: Should read battery voltage (if battery > 3.0V)

**Checkpoint**: TP4056 connected to battery, charging verified

### Step 2.3: Install Power Switch

**Procedure:**

1. **Choose Switch Location**
   - Accessible from outside enclosure
   - Along positive power path (between TP4056 OUT+ and MT3608 IN+)

2. **Wire Power Switch**
   ```
   TP4056 OUT+ ──[Red wire]──→ Switch Terminal 1
   Switch Terminal 2 ──[Red wire]──→ MT3608 IN+
   ```

3. **Connect Wires**
   - Cut two red wires, ~10cm each
   - Strip ends 5mm
   - Solder to switch terminals (or use crimp connectors)
   - Add heat shrink tubing
   - Label: "IN" and "OUT"

4. **Test Switch**
   - Toggle switch ON and OFF
   - Use multimeter to verify continuity when ON
   - Verify open circuit when OFF

**Checkpoint**: Power switch installed and tested

### Step 2.4: Setup MT3608 Boost Converter

**CRITICAL STEP**: Incorrect voltage will damage components!

**Procedure:**

1. **Inspect MT3608 Module**
   - Locate input pads: IN+, IN-
   - Locate output pads: OUT+, OUT-
   - Locate potentiometer (small blue or white screw)

2. **Wire MT3608 Input**
   - Connect TP4056 OUT+ → Power Switch → MT3608 IN+
   - Connect TP4056 OUT- → MT3608 IN- (GND)
   - Double-check polarity!

3. **Initial Voltage Adjustment**
   ```
   WARNING: Do this BEFORE connecting any components to output!

   1. Insert charged battery (>3.5V recommended)
   2. Turn power switch ON
   3. Set multimeter to DC voltage (20V range)
   4. Touch red probe to MT3608 OUT+
   5. Touch black probe to MT3608 OUT- (GND)
   6. Read voltage on multimeter
   7. Use small screwdriver to adjust potentiometer:
      - Clockwise: Increase voltage
      - Counter-clockwise: Decrease voltage
   8. Adjust until multimeter reads 5.00V ± 0.05V
   9. Wait 30 seconds and re-check (may drift slightly)
   10. Fine-tune to exactly 5.00V
   11. Turn power switch OFF
   ```

4. **Mark the Setting**
   - Use marker or nail polish to mark potentiometer position
   - Take photo for reference
   - Note: Setting may need minor adjustment under load

5. **Prepare Output Wires**
   - Cut multiple red wires for 5V distribution:
     - To ESP32 CYD: ~10cm
     - To LD2450: ~15cm
     - To PAM8403: ~10cm
     - To 5-way switch: ~10cm
   - Cut black wires for GND (same lengths)
   - Tin all wire ends
   - Consider using a distribution terminal block

**Checkpoint**: MT3608 output verified at 5.00V

### Step 2.5: Connect LED Battery Monitor

**Procedure:**

1. **Wire LED Monitor**
   ```
   Battery + ──[Red wire]──→ LED Monitor IN+
   Battery - ──[Black wire]──→ LED Monitor IN-
   ```

   **Important**: Connect BEFORE TP4056, directly to battery terminals

2. **Test LED Monitor**
   - Insert charged battery
   - LEDs should illuminate showing charge level
   - Verify correct number of LEDs for battery state

3. **Mount LED Monitor**
   - Position visible from outside (or with light pipe)
   - Secure with hot glue or double-sided tape

**Checkpoint**: LED battery monitor functioning

---

## Phase 3: Component Wiring

### Step 3.1: Power Distribution

**Procedure:**

1. **Create Common Ground Bus**
   ```
   Options:
   A) Use breadboard or perfboard strip
   B) Twist multiple black wires together with wire nut
   C) Use screw terminal block

   All GND connections must be common:
   - MT3608 OUT-
   - ESP32 CYD GND
   - LD2450 GND
   - PAM8403 GND
   - 5-way switch GND
   - Speaker negative (if needed)
   ```

2. **Distribute 5V Power**
   - Solder red wires to MT3608 OUT+
   - Route to each component:
     - ESP32 CYD 5V pin
     - LD2450 VCC
     - PAM8403 VCC
   - Use heatshrink on all connections

3. **Organize Wiring**
   - Bundle power wires together with zip ties
   - Keep signal wires separate from power
   - Label all connections

**Checkpoint**: Power distribution wiring complete

### Step 3.2: Connect LD2450 Radar

**Procedure:**

1. **Prepare LD2450 Cable**
   ```
   LD2450 has 4-pin JST 1.25mm connector:
   Pin 1: GND (Black)
   Pin 2: TX  (Green) ← LD2450 transmits to ESP32
   Pin 3: RX  (White) ← LD2450 receives from ESP32
   Pin 4: VCC (Red)
   ```

2. **Option A: Use Existing Connector**
   - If LD2450 has pre-attached cable, use it
   - May need JST-to-Dupont adapter

3. **Option B: Direct Wiring**
   - Cut JST connector off (if desired)
   - Strip and tin 4 wires
   - Add heat shrink

4. **Wire to ESP32 CN1**
   ```
   LD2450 Pin → ESP32 CN1
   ──────────────────────
   Pin 1 (GND) → CN1 Pin 5 (GND)
   Pin 2 (TX)  → CN1 Pin 2 (GPIO 22)
   Pin 3 (RX)  → CN1 Pin 4 (GPIO 27)
   Pin 4 (VCC) → 5V rail (NOT 3.3V!)
   ```

5. **Secure Connections**
   - Solder or use female Dupont connectors
   - Add heat shrink to prevent shorts
   - Route cable neatly

**Checkpoint**: LD2450 connected, ready to test

### Step 3.3: Connect Audio System

**Procedure:**

1. **Wire PAM8403 Amplifier**
   ```
   PAM8403 Module → Connection
   ──────────────────────────
   VCC      → 5V rail
   GND      → Common GND
   IN_L     → ESP32 GPIO 25 (DAC1)
   IN_R     → GND (tie to ground for mono)
   ```

2. **Connect Speaker**
   ```
   PAM8403  → 8Ω Speaker
   ─────────────────────
   L+  → Speaker Red (+)
   L-  → Speaker Black (-)
   ```

3. **Secure Speaker**
   - Mount speaker facing forward (sound output)
   - Use hot glue or screws
   - Ensure speaker cone is not obstructed

4. **Volume Control**
   - Adjust PAM8403 onboard potentiometer to mid-level
   - Can fine-tune during testing

**Checkpoint**: Audio system wired and ready

### Step 3.4: Connect 5-Way Navigation Switch

**Procedure:**

1. **Identify Switch Pins**
   ```
   5-Way Breakout Board:
   ┌──────────────────┐
   │ VCC  GND         │ ← Power (3.3V, NOT 5V!)
   │ UP   DOWN        │
   │ LEFT RIGHT       │
   │ CENTER           │
   └──────────────────┘
   ```

2. **Power Connections**
   ```
   IMPORTANT: Use 3.3V, NOT 5V!

   5-Way VCC → ESP32 3.3V pin (NOT 5V rail!)
   5-Way GND → Common GND
   ```

3. **Button Signal Wires**
   ```
   5-Way Pin   → ESP32 GPIO
   ───────────────────────
   UP      → GPIO 4  (freed from RGB LED)
   DOWN    → GPIO 16 (freed from RGB LED)
   LEFT    → GPIO 17 (freed from RGB LED)
   RIGHT   → GPIO 21 (CN1 Pin 3)
   CENTER  → GPIO 2  (separate wire if needed)
   ```

4. **Wire Routing**
   - Use color-coded wires for easy identification
   - Suggested colors:
     - Orange: UP
     - Green: DOWN
     - Blue: LEFT
     - Yellow: RIGHT
     - White: CENTER
   - Add labels with tape/marker
   - Keep wires organized and bundled

5. **Secure Switch**
   - Position for easy thumb access
   - Mount with screws or adhesive
   - Ensure no stress on solder joints

**Checkpoint**: 5-way switch fully wired

---

## Phase 4: Testing & Calibration

### Step 4.1: Visual Inspection

**Before applying power:**

- [ ] All solder joints are clean and shiny (not cold/dull)
- [ ] No solder bridges between adjacent pins
- [ ] Wire polarity correct (red=positive, black=ground)
- [ ] MT3608 output is 5.00V (pre-tested)
- [ ] No loose wires or exposed conductors
- [ ] Battery inserted with correct polarity
- [ ] Power switch in OFF position

### Step 4.2: Continuity Testing

**Use multimeter in continuity/ohms mode:**

1. **Check for Shorts**
   ```
   Test Points           Expected Result
   ─────────────────────────────────
   5V rail to GND       Open circuit (∞Ω)
   3.3V to GND          Open circuit (∞Ω)
   Battery + to -       Open circuit when switch OFF
   ```

2. **Verify Connections**
   ```
   From              To               Expected
   ────────────────────────────────────────
   MT3608 OUT+       ESP32 5V         Continuity
   MT3608 OUT-       ESP32 GND        Continuity
   All GND points    Together         Continuity
   ```

**Checkpoint**: No shorts detected, connections verified

### Step 4.3: Power System Test

**Procedure:**

1. **Initial Power-On**
   - Ensure USB cable is NOT connected to ESP32
   - Turn power switch ON
   - LED battery monitor should light up
   - No smoke, no burning smell

2. **Measure Voltages**
   ```
   Test Point           Expected Value      Action if Wrong
   ──────────────────────────────────────────────────────
   Battery terminals    3.0-4.2V            Charge battery
   MT3608 IN+           3.0-4.2V            Check switch/TP4056
   MT3608 OUT+ (5V)     5.00V ± 0.05V       Adjust potentiometer
   ESP32 5V pin         5.00V               Check connections
   ESP32 3.3V pin       3.30V ± 0.05V       Normal (regulated)
   ```

3. **Load Test**
   - With power ON, display should light up
   - Measure 5V rail again (may drop slightly under load)
   - If voltage drops below 4.8V, MT3608 may be insufficient

**Checkpoint**: All voltages correct under load

### Step 4.4: ESP32 CYD Test

**Procedure:**

1. **Connect USB Cable**
   - Use USB cable for serial communication
   - Do NOT rely on USB for primary power (use battery)

2. **Upload Test Sketch**
   ```cpp
   void setup() {
     Serial.begin(115200);
     pinMode(2, OUTPUT);
   }

   void loop() {
     digitalWrite(2, HIGH);
     delay(500);
     digitalWrite(2, LOW);
     delay(500);
     Serial.println("ESP32 Test OK");
   }
   ```

3. **Verify Operation**
   - Open Arduino IDE Serial Monitor (115200 baud)
   - Should see "ESP32 Test OK" every second
   - Display should be lit
   - Touchscreen should respond (test with touch test sketch)

**Checkpoint**: ESP32 CYD functioning

### Step 4.5: LD2450 Radar Test

**Procedure:**

1. **Upload LD2450 Test Code**
   ```cpp
   #include <HardwareSerial.h>

   HardwareSerial LD2450Serial(1);

   void setup() {
     Serial.begin(115200);
     LD2450Serial.begin(256000, SERIAL_8N1, 22, 27);
     Serial.println("LD2450 Test Starting...");
   }

   void loop() {
     if (LD2450Serial.available()) {
       uint8_t data = LD2450Serial.read();
       Serial.print(data, HEX);
       Serial.print(" ");
     }
   }
   ```

2. **Test Detection**
   - Open Serial Monitor (115200 baud)
   - Should see hex data stream from LD2450
   - Wave hand in front of sensor
   - Data should change when motion detected

3. **Verify Range**
   - Move at various distances (1m, 2m, 4m, 6m)
   - Sensor should detect within rated range

**Checkpoint**: LD2450 detecting motion

### Step 4.6: Audio System Test

**Procedure:**

1. **Upload Tone Generator**
   ```cpp
   #define DAC_PIN 25

   void setup() {
     pinMode(DAC_PIN, OUTPUT);
   }

   void loop() {
     // Generate 1kHz square wave
     for(int i=0; i<100; i++) {
       dacWrite(DAC_PIN, 200);
       delayMicroseconds(500);
       dacWrite(DAC_PIN, 50);
       delayMicroseconds(500);
     }
     delay(500);
   }
   ```

2. **Test Speaker**
   - Should hear beeping tone from speaker
   - Adjust PAM8403 volume potentiometer
   - Verify no distortion at various volumes

3. **Audio Quality Check**
   - Sound should be clear, not fuzzy
   - No buzzing or humming noise
   - If noisy, check ground connections

**Checkpoint**: Audio system working

### Step 4.7: Button Test

**Procedure:**

1. **Upload Button Test Code**
   ```cpp
   void setup() {
     Serial.begin(115200);
     pinMode(4, INPUT_PULLUP);   // UP
     pinMode(16, INPUT_PULLUP);  // DOWN
     pinMode(17, INPUT_PULLUP);  // LEFT
     pinMode(21, INPUT_PULLUP);  // RIGHT
     pinMode(2, INPUT_PULLUP);   // CENTER
   }

   void loop() {
     if(digitalRead(4) == LOW) Serial.println("UP");
     if(digitalRead(16) == LOW) Serial.println("DOWN");
     if(digitalRead(17) == LOW) Serial.println("LEFT");
     if(digitalRead(21) == LOW) Serial.println("RIGHT");
     if(digitalRead(2) == LOW) Serial.println("CENTER");
     delay(100);
   }
   ```

2. **Test All Buttons**
   - Press each direction
   - Verify serial output shows correct button
   - Test CENTER push button
   - Verify no false triggers

**Checkpoint**: All buttons functioning

### Step 4.8: Battery Life Test

**Procedure:**

1. **Full Charge**
   - Charge battery to 4.2V
   - Disconnect charger

2. **Runtime Test**
   - Turn device ON
   - Note start time
   - Let run with typical usage:
     - Display on
     - Radar scanning
     - Occasional audio
   - Monitor voltage periodically

3. **Expected Results**
   ```
   3000mAh battery: 3-4 hours
   3500mAh battery: 4-5 hours

   Battery voltage vs runtime:
   4.2V - 4.0V: ~80-100% capacity
   4.0V - 3.7V: ~50-80% capacity
   3.7V - 3.5V: ~20-50% capacity
   3.5V - 3.3V: ~5-20% capacity
   <3.3V: <5% (recharge soon)
   <3.0V: Protection cutoff
   ```

**Checkpoint**: Battery life meets expectations

---

## Phase 5: Enclosure Assembly

### Step 5.1: Enclosure Planning

**Design Considerations:**

```
Front Panel:
- Display cutout (2.8" rectangular)
- LD2450 sensor opening (small circle)
- Speaker grille/holes
- LED battery monitor window

Top Panel:
- 5-way navigation switch cutout

Side Panel:
- Power switch
- Micro USB charging port (optional panel mount)

Rear Panel:
- Battery access door (optional)
```

### Step 5.2: Component Layout

**Recommended Positioning:**

```
┌─────────────────────────────────┐
│  [Battery Monitor LEDs]         │ ← Top edge
│                                 │
│    ┌──────────────────┐         │
│    │                  │         │ ← Display
│    │   TFT Display    │         │
│    │                  │         │
│    └──────────────────┘         │
│                                 │
│    [5-way Nav Switch]           │ ← Thumb position
│                                 │
│         🔊 Speaker              │ ← Bottom
│    [LD2450]        [○]          │ ← Radar sensor
└─────────────────────────────────┘

Side View:
┌────┐
│ SW │ ← Power switch on side
│    │
│ USB│ ← Optional charging port
└────┘
```

### Step 5.3: 3D Printing (If Applicable)

**If designing custom enclosure:**

1. **Measurements**
   - ESP32 CYD: 62mm x 42mm x 10mm
   - LD2450: 17mm x 21mm x 4mm
   - Speaker: 28-40mm diameter
   - 18650 battery: 18mm diameter x 65mm length

2. **Design Tips**
   - Add screw bosses for M2 and M3 mounting
   - Include standoffs for PCB (3-5mm height)
   - Cable routing channels
   - Ventilation holes (prevent heat buildup)
   - Access for USB programming

3. **Print Settings**
   - Material: PLA or ABS
   - Layer height: 0.2mm
   - Infill: 20-30%
   - Supports: As needed for overhangs

### Step 5.4: Component Mounting

**Procedure:**

1. **Mount ESP32 CYD**
   - Use M2 standoffs (5-8mm height)
   - Secure with M2 screws
   - Ensure display aligns with cutout

2. **Mount LD2450**
   - Position antenna facing forward
   - Keep away from metal (affects radar)
   - Secure with double-sided foam tape or M2 screws
   - Ensure clear line of sight through opening

3. **Mount Speaker**
   - Secure to front panel
   - Grille or mesh cover recommended
   - Seal edges to prevent sound leakage

4. **Mount Power Modules**
   - TP4056: Accessible for USB charging
   - MT3608: Interior position OK
   - Secure with foam tape or hot glue

5. **Mount Battery Holder**
   - Secure firmly (battery is heavy)
   - Consider removable/accessible placement
   - Add strain relief for wires

6. **Mount 5-Way Switch**
   - Panel mount through cutout
   - Secure with included hardware or hot glue
   - Test ergonomics (thumb reach)

7. **Mount LED Battery Monitor**
   - Position visible from outside
   - Use light pipe if needed
   - Secure with glue

**Checkpoint**: All components mounted securely

### Step 5.5: Cable Management

**Procedure:**

1. **Organize Wires**
   - Group by function (power, signal, etc.)
   - Use zip ties or twist ties
   - Avoid sharp bends
   - Keep wires away from moving parts

2. **Strain Relief**
   - Add hot glue blobs at connection points
   - Prevent wire pull from damaging solder joints
   - Especially important for battery and power connections

3. **Secure Loose Components**
   - Hot glue MT3608, TP4056 to base
   - Tack down wire bundles
   - Prevent rattling

**Checkpoint**: Wiring neat and secure

---

## Phase 6: Final Assembly

### Step 6.1: Close Enclosure

**Procedure:**

1. **Final Checks**
   - [ ] All components mounted
   - [ ] No loose wires
   - [ ] Power switch accessible
   - [ ] Charging port accessible (if external)
   - [ ] All screws ready

2. **Close Case**
   - Align top and bottom halves
   - Ensure no wires pinched
   - Insert all screws
   - Tighten evenly (don't overtighten plastic)

3. **Test After Closing**
   - Power ON
   - Verify all functions still work
   - No rattling when shaken gently

**Checkpoint**: Enclosure closed and secure

### Step 6.2: Labeling

**Add labels for:**
- Power switch (ON/OFF)
- Charging port
- LED indicator meanings
- Version/build date

### Step 6.3: Final Testing

**Complete System Test:**

1. **Power Up**
   - Turn device ON with power switch
   - Display should boot immediately
   - Battery LEDs showing charge level

2. **Radar Function**
   - LD2450 detecting targets
   - Display showing radar sweep (once software loaded)
   - Range and position data accurate

3. **Audio Function**
   - Beeps/tones playing correctly
   - Volume appropriate
   - No distortion

4. **Button Function**
   - All 5 directions responsive
   - Menu navigation working
   - No missed presses

5. **Battery Life**
   - Charge fully
   - Run for expected duration
   - Battery monitor accurately reflects level

**Checkpoint**: All systems operational

---

## Troubleshooting

### Power Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| No power at all | Battery dead, switch off | Charge battery, check switch |
| Display dim | Low battery, wrong voltage | Check 5V rail with multimeter |
| Powers on briefly then off | TP4056 protection, low battery | Charge battery fully |
| MT3608 gets hot | Overload, short circuit | Check current draw, look for shorts |

### LD2450 Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| No data from sensor | Wrong wiring, wrong baud | Verify GPIO 22/27, use 256000 baud |
| Random/garbled data | Loose connection, EMI | Check solder joints, add shielding |
| Short detection range | Obstruction, wrong angle | Clear sensor area, adjust mounting |
| False detections | Reflections, interference | Change sensor position, adjust sensitivity |

### Audio Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| No sound | PAM8403 not powered, volume low | Check 5V, increase volume pot |
| Distorted sound | Wrong impedance, too much volume | Use 8Ω speaker, reduce volume |
| Buzzing/humming | Ground loop, poor filtering | Ensure common ground, add capacitor |
| Weak sound | Low volume, speaker mismatch | Increase volume, check speaker specs |

### Button Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| Buttons not responding | Pull-up not configured, bad solder | Check pinMode INPUT_PULLUP, resolder |
| Multiple triggers per press | Bounce, no debounce | Add software debouncing |
| Some buttons work, others don't | Broken trace, cold solder | Check continuity, reflow solder |
| Random triggers | Floating input, EMI | Enable pull-up resistors |

### Display Issues

| Symptom | Possible Cause | Solution |
|---------|---------------|----------|
| Blank screen | No power, wrong voltage | Check 5V supply, backlight |
| White screen | Initialization issue | Check library configuration |
| Touch not working | Touch CS pin conflict | Verify GPIO 33 not used elsewhere |
| Flickering | Low voltage, interference | Check power supply stability |

---

## Maintenance

### Regular Checks

**Monthly:**
- [ ] Clean display with microfiber cloth
- [ ] Check all external connections
- [ ] Verify battery charge cycle
- [ ] Test all buttons

**Every 3 Months:**
- [ ] Inspect solder joints
- [ ] Check for loose screws
- [ ] Clean speaker grille
- [ ] Verify MT3608 voltage still 5.0V

**Annually:**
- [ ] Full disassembly and cleaning
- [ ] Replace thermal paste if components run hot
- [ ] Check battery capacity (may degrade over time)
- [ ] Update firmware

### Battery Care

- **Charge** when below 20% (3.5V)
- **Don't over-discharge** below 3.0V (protection should prevent)
- **Store** at 50-60% charge if not using for months
- **Temperature**: Avoid extreme heat or cold
- **Replace** battery if capacity drops significantly or swells

---

## Congratulations!

Your CYD Handheld Radar motion tracker is complete!

**Next Steps:**
1. Upload final radar software
2. Configure display graphics and radar sweep
3. Fine-tune audio tones and beep patterns
4. Customize menu system with 5-way switch
5. Share your build with the community!

---

**Last Updated**: 2026-01-13
**Revision**: 1.0
