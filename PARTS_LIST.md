# Complete Parts List - CYD Handheld Radar

## Bill of Materials (BOM)

Total estimated cost: **$58-95 USD** (depending on component quality and sourcing)

---

## Core Electronics

| Part | Specifications | Qty | Unit Cost | Total | Source | Notes |
|------|---------------|-----|-----------|-------|--------|-------|
| **ESP32-2432S028R** | Cheap Yellow Display (CYD)<br>- ESP32-WROOM-32<br>- 2.8" ILI9341 TFT (240x320)<br>- Resistive touchscreen<br>- 4MB Flash | 1 | $12-15 | $12-15 | AliExpress, Amazon | Search "ESP32 CYD" or "ESP32-2432S028R" |
| **HLK-LD2450** | mmWave Radar Sensor<br>- 24GHz FMCW<br>- 6-8m range<br>- 3 target tracking<br>- UART interface | 1 | $15-20 | $15-20 | AliExpress, Amazon | Search "HLK-LD2450" or "LD2450 radar" |
| **18650 Battery** | Lithium-ion cell<br>- 3000-3500mAh<br>- Protected (with PCB)<br>- Brands: Samsung, LG, Panasonic | 1 | $5-8 | $5-8 | Battery stores, Amazon | **MUST be protected cell** |
| **TP4056 Module** | Li-ion Charger<br>- 1A charging current<br>- With protection circuit<br>- Micro USB input | 1 | $0 | $0 | Already owned | User already has this |
| **MT3608 Boost Converter** | DC-DC Step-Up Module<br>- Input: 2-24V<br>- Output: 5-28V adjustable<br>- 2A max current | 1 | $1-2 | $1-2 | AliExpress, Amazon | Set output to 5.0V |

**Subtotal Core Electronics**: $33-45

---

## Audio System

| Part | Specifications | Qty | Unit Cost | Total | Source | Notes |
|------|---------------|-----|-----------|-------|--------|-------|
| **PAM8403 Amplifier** | Audio Amp Module<br>- 2x3W stereo<br>- 5V supply<br>- Potentiometer onboard | 1 | $1-2 | $1-2 | AliExpress, Amazon | Common cheap module |
| **Speaker** | Mini Speaker<br>- 8Ω impedance<br>- 0.5-1W power<br>- 28-40mm diameter | 1 | $2-3 | $2-3 | AliExpress, Amazon, Adafruit | Ensure 8Ω impedance |

**Subtotal Audio**: $3-5

---

## Control & Monitoring

| Part | Specifications | Qty | Unit Cost | Total | Source | Notes |
|------|---------------|-----|-----------|-------|--------|-------|
| **5-Way Nav Switch Breakout** | Navigation Switch<br>- UP/DOWN/LEFT/RIGHT/CENTER<br>- Pull-up resistors included<br>- 5V/3.3V compatible | 1 | $3-6 | $3-6 | Amazon, Adafruit | Adafruit #504 or compatible |
| **LED Battery Monitor** | Battery Level Indicator<br>- 3.7-4.2V range<br>- 3-4 LED display<br>- Standalone board | 1 | $1-2 | $1-2 | AliExpress, Amazon | No GPIO required |
| **Power Switch** | SPST Toggle/Slide Switch<br>- 5A minimum rating<br>- Panel mount | 1 | $1-2 | $1-2 | Amazon, Mouser, Digikey | Main power on/off |

**Subtotal Controls**: $5-10

---

## Wiring & Connectors

| Part | Specifications | Qty | Unit Cost | Total | Notes |
|------|---------------|-----|-----------|-------|-------|
| **Dupont Jumper Wires** | Female-to-female<br>- 20-30 pieces<br>- 20cm length | 1 pack | $2-3 | $2-3 | For prototyping |
| **JST Connectors** | 2.0mm or 2.54mm pitch<br>- 2-pin connectors<br>- With wire pigtails | 3-5 sets | $0.50-1 ea | $2-4 | Battery connections |
| **Pin Headers** | 2.54mm pitch male<br>- Single row<br>- 40-pin strip | 2 strips | $0.50-1 | $1-2 | For CN1 connector |
| **Heat Shrink Tubing** | Assorted sizes<br>- 2mm, 3mm, 5mm<br>- Multiple colors | 1 set | $2-3 | $2-3 | Insulation |
| **Hookup Wire** | 22-24 AWG stranded<br>- Red/black pair<br>- 1-2 meters | 1-2m | $1-2 | $1-2 | Power distribution |

**Subtotal Wiring**: $8-14

---

## Hardware & Enclosure

| Part | Specifications | Qty | Unit Cost | Total | Notes |
|------|---------------|-----|-----------|-------|-------|
| **Enclosure** | Project box or 3D printed<br>- ~120x80x40mm<br>- ABS or PLA | 1 | $5-20 | $5-20 | 3D print or buy |
| **18650 Battery Holder** | Single cell holder<br>- With solder tabs or wires<br>- PCB mount or case mount | 1 | $1-2 | $1-2 | Secure mounting |
| **M2 Screws & Standoffs** | M2 x 5mm, 8mm, 10mm<br>- Nylon or metal | 10-20 | $0.10-0.20 | $2-3 | PCB mounting |
| **M3 Screws & Nuts** | M3 x 8mm, 12mm<br>- For case assembly | 8-12 | $0.10-0.20 | $1-2 | Case assembly |
| **Double-sided Foam Tape** | 3M VHB or similar<br>- 10mm wide<br>- 1m length | 1 roll | $2-3 | $2-3 | Component mounting |

**Subtotal Hardware**: $11-30

---

## Optional Components

| Part | Purpose | Qty | Cost | Priority |
|------|---------|-----|------|----------|
| **Micro USB Panel Mount** | External charging port | 1 | $2-3 | High |
| **10kΩ Potentiometer** | Manual volume control | 1 | $1 | Medium |
| **Tactile Push Button** | Reset/mode button | 1-2 | $0.50-1 | Low |
| **Status LEDs** | Power/charging indicators (3mm) | 2-3 | $0.20 ea | Low |
| **220Ω Resistors** | For status LEDs | 2-3 | $0.10 | Low |
| **MicroSD Card** | Store sound files (8-16GB) | 1 | $5-8 | Low |
| **Fuse Holder + Fuse** | Battery protection (2A fast-blow) | 1 | $1-2 | Medium |
| **Voltage Divider Resistors** | 2x 100kΩ for GPIO 35 battery monitoring | 2 | $0.10 | Low |

**Subtotal Optional**: $10-18

---

## Tools Required (Not Included in Cost)

### Essential Tools

- **Soldering Iron** (30-60W with fine tip)
- **Solder** (60/40 or lead-free)
- **Wire Strippers** (20-30 AWG)
- **Flush Cutters** (for trimming leads)
- **Multimeter** (voltage/continuity testing)
- **Small Phillips Screwdriver** (for M2/M3 screws)

### Recommended Tools

- **Helping Hands** (PCB holder with clips)
- **Desoldering Pump** (for RGB LED removal)
- **Hot Glue Gun** (component securing)
- **Tweezers** (SMD component handling)
- **USB Cable** (Micro USB for ESP32 programming)

---

## Power System Components Summary

| Component | Input Voltage | Output Voltage | Current | Purpose |
|-----------|--------------|----------------|---------|---------|
| 18650 Battery | - | 3.0-4.2V | 3000-3500mAh | Power source |
| TP4056 | 5V USB | 4.2V charging | 1A max | Battery charging |
| MT3608 | 3.0-4.2V | 5.0V | 2A max | Voltage boost |
| ESP32 CYD | 5V | 3.3V internal | 150-250mA | Main controller |
| LD2450 | 5V | - | 100-200mA | Radar sensor |
| PAM8403 | 5V | - | 50-250mA | Audio amplifier |

**Total System Draw**: 310mA typical, 710mA peak

---

## Component Sourcing Guide

### Budget Option (~$58 total)

**Where to buy:**
- AliExpress (electronics, ships from China, 2-4 weeks)
- eBay (mixed sellers, 1-4 weeks)
- Banggood (electronics, ships from China, 2-4 weeks)

**Trade-offs:**
- Longer shipping times
- Generic/unbranded parts
- Less warranty support
- Potentially lower quality

### Quality Option (~$95 total)

**Where to buy:**
- Amazon (fast shipping, 1-2 days)
- Adafruit (quality components, USA)
- SparkFun (quality components, USA)
- Mouser/Digikey (components, USA/global)

**Advantages:**
- Faster shipping
- Name-brand components
- Better documentation
- Warranty/support
- Higher reliability

---

## Recommended Brands

### Batteries
- **Best**: Samsung 35E, LG HG2, Panasonic NCR18650B
- **Good**: Protected generic cells from reputable sellers
- **Avoid**: Ultra-cheap "9900mAh" fake capacity cells

### Power Modules
- **TP4056**: Generic Chinese modules are fine (all similar)
- **MT3608**: Generic modules work well, Adafruit PowerBoost better
- **Alternative**: All-in-one UPS modules (easier but more expensive)

### ESP32 CYD
- **Sunton**: Original manufacturer (best quality)
- **Generic**: Most clones are identical, check reviews

### LD2450
- **Hi-Link**: Original manufacturer (HLK-LD2450)
- **Avoid**: Fake/counterfeit radar modules

---

## Shopping List Checklist

**Before ordering, verify:**
- [ ] ESP32 model is ESP32-2432S028**R** (R version, not C)
- [ ] LD2450 (not LD2410 or RD-03D)
- [ ] 18650 battery is **protected** (has built-in PCB)
- [ ] MT3608 has adjustable output (potentiometer)
- [ ] 5-way switch has pull-up resistors (breakout board version)
- [ ] Speaker is 8Ω impedance (not 4Ω or 16Ω)
- [ ] Power switch rated for at least 2A

---

## Alternative Components

### If LD2450 Unavailable:
- **RD-03D** ($15-20): Similar specs, 8m range, requires angle-to-XY conversion in code
- **LD2410** ($10-15): No multi-target, no positioning, **NOT RECOMMENDED**

### If PAM8403 Unavailable:
- **PAM8406**: 5W stereo amplifier
- **MAX98357A**: I2S digital amplifier (requires different wiring)

### If 5-Way Switch Unavailable:
- **Individual tactile buttons** (5x): Cheaper but requires more space
- **Analog joystick module**: Provides more control but uses ADC pins

### If MT3608 Unavailable:
- **SX1308**: Similar boost converter
- **XL6009**: Higher power boost converter
- **PowerBoost 1000C** (Adafruit): More expensive but better quality

---

## Inventory Tracking

Use this checklist when gathering parts:

```
Core Electronics:
[ ] ESP32-2432S028R (CYD)
[ ] HLK-LD2450 radar sensor
[ ] 18650 battery (protected)
[✓] TP4056 charging module (already owned)
[ ] MT3608 boost converter

Audio:
[ ] PAM8403 amplifier module
[ ] 8Ω speaker (28-40mm)

Controls:
[ ] 5-way navigation switch breakout
[ ] LED battery monitor board
[ ] SPST power switch

Wiring:
[ ] Dupont jumper wires (20-30x)
[ ] JST connectors (3-5 sets)
[ ] Pin headers (2x 40-pin strips)
[ ] Heat shrink tubing (assorted)
[ ] Hookup wire (22-24 AWG, 1-2m)

Hardware:
[ ] Enclosure/case (~120x80x40mm)
[ ] 18650 battery holder
[ ] M2 screws & standoffs (10-20x)
[ ] M3 screws & nuts (8-12x)
[ ] Double-sided foam tape

Optional:
[ ] Micro USB panel mount extension
[ ] 10kΩ potentiometer
[ ] Tactile push buttons (1-2x)
[ ] Status LEDs (2-3x)
[ ] 2A fuse + holder
```

---

## Notes

1. **Battery Safety**: Always use protected 18650 cells. Never use damaged or unknown batteries.

2. **Component Compatibility**: Double-check voltage requirements before connecting power.

3. **Static Protection**: LD2450 and ESP32 are ESD-sensitive. Use anti-static precautions.

4. **Wire Gauge**: 22-24 AWG suitable for all connections. Don't use wire thinner than 26 AWG for power.

5. **Substitutions**: Most components can be substituted with similar specs. Check datasheets.

---

**Last Updated**: 2026-01-13
