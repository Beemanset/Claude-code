# CYD Motion Tracker - Arduino Setup Guide

## Overview

This is the Arduino sketch for the CYD Handheld Radar Motion Tracker. It provides a complete, working implementation with:

- ✅ LD2450 radar sensor integration (UART communication)
- ✅ Animated radar sweep display (Aliens M314 style)
- ✅ Multi-target tracking (up to 3 simultaneous targets)
- ✅ Proximity audio feedback (variable frequency and rate)
- ✅ 5-way navigation switch menu system
- ✅ Settings storage in NVRAM
- ✅ Battery monitoring support

## Hardware Requirements

### Confirmed Hardware
- **ESP32-2432S028** (CYD - Cheap Yellow Display) with **CS-32 chip**
- **Two USB ports**: USB-C and Micro USB
- **HLK-LD2450** mmWave radar sensor
- **PAM8403** audio amplifier + 8Ω speaker
- **5-way navigation switch** breakout board
- **18650 battery** power system

### Required Modifications
Before uploading code, you MUST:
1. **Remove RGB LED** from CYD board (frees GPIO 4, 16, 17)
2. **Solder pin headers to CN1** connector (access GPIO 21, 22, 27, 35)

See [WIRING_GUIDE.md](../WIRING_GUIDE.md) for details.

---

## Arduino IDE Setup

### Step 1: Install Arduino IDE

Download and install **Arduino IDE 2.x** from:
https://www.arduino.cc/en/software

### Step 2: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Boards Manager**
6. Search for "ESP32"
7. Install **"esp32 by Espressif Systems"** (version 2.0.14 or later)
8. Wait for installation to complete

### Step 3: Select Your Board

1. Go to **Tools → Board → esp32**
2. Select **"ESP32 Dev Module"**

3. Configure board settings:
   ```
   Board: "ESP32 Dev Module"
   Upload Speed: "921600"
   CPU Frequency: "240MHz (WiFi/BT)"
   Flash Frequency: "80MHz"
   Flash Mode: "QIO"
   Flash Size: "4MB (32Mb)"
   Partition Scheme: "Default 4MB with spiffs"
   Core Debug Level: "None"
   PSRAM: "Disabled"
   ```

### Step 4: Install Required Libraries

Go to **Tools → Manage Libraries** and install:

#### 1. TFT_eSPI (by Bodmer)
- Version: 2.5.0 or later
- **CRITICAL**: You MUST configure this library for CYD

#### 2. Preferences
- Built-in ESP32 library (no installation needed)

### Step 5: Configure TFT_eSPI for CYD

**CRITICAL STEP**: TFT_eSPI must be configured for your CYD board.

#### Find User_Setup.h Location:

**Windows:**
```
C:\Users\<YourUsername>\Documents\Arduino\libraries\TFT_eSPI\User_Setup.h
```

**Mac:**
```
~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
```

**Linux:**
```
~/Arduino/libraries/TFT_eSPI/User_Setup.h
```

#### Edit User_Setup.h:

1. Open `User_Setup.h` in a text editor
2. Comment out all existing driver definitions (add `//` at start)
3. Add the following configuration:

```cpp
// ===== CYD ESP32-2432S028 Configuration =====

#define USER_SETUP_ID 303

// Driver
#define ILI9341_DRIVER

// Display dimensions
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ESP32 pins for CYD
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1  // Not connected

// Touchscreen
#define TOUCH_CS 33

// SPI frequency
#define SPI_FREQUENCY  55000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT
```

4. Save the file
5. **Restart Arduino IDE** (important!)

#### Verify TFT_eSPI Configuration:

Load example: **File → Examples → TFT_eSPI → 320x240 → TFT_Terminal**

If it compiles without errors, configuration is correct.

---

## Uploading the Code

### Step 1: Connect Your CYD

1. Connect CYD to computer via **USB-C or Micro USB** port
2. Wait for drivers to install (Windows may need CH340 drivers)

#### CH340 Drivers (if needed):

**Windows:** Download from: https://sparks.gogo.co.nz/ch340.html

**Mac:** Usually automatic with modern macOS

**Linux:** Built-in

### Step 2: Select Port

1. Go to **Tools → Port**
2. Select the port showing your ESP32 (e.g., `COM3`, `/dev/ttyUSB0`, `/dev/cu.usbserial-*`)

If no port appears:
- Try different USB cable (must support data, not just charging)
- Try different USB port
- Install CH340 drivers
- Press RESET button on CYD

### Step 3: Open the Sketch

1. Navigate to `CYD_Motion_Tracker` folder
2. Open `CYD_Motion_Tracker.ino`
3. Verify both files are present:
   - `CYD_Motion_Tracker.ino`
   - `config.h`

### Step 4: Compile and Upload

1. Click **Verify** button (✓) to compile
   - Watch for errors in output window
   - Should show "Compilation complete" with memory usage

2. Click **Upload** button (→) to upload
   - ESP32 may auto-reset and enter bootloader mode
   - If stuck on "Connecting...", hold BOOT button and press RESET
   - Wait for "Hard resetting via RTS pin..." message
   - Upload typically takes 15-30 seconds

### Step 5: Monitor Serial Output

1. Open **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. You should see boot messages:
   ```
   ========================================
   CYD Motion Tracker
   Version: 1.0.0
   ========================================

   Initializing display...
   Loading settings...
   Initializing LD2450 radar...
   Initialization complete!
   Entering scanning mode...
   ```

---

## Troubleshooting

### Upload Issues

| Problem | Solution |
|---------|----------|
| "Port not found" | Install CH340 drivers, try different USB cable |
| "Connecting..." stuck | Hold BOOT button, press RESET, try upload again |
| "Flash write error" | Check board settings, try lower upload speed (115200) |
| "Board not recognized" | Check Tools → Board is "ESP32 Dev Module" |

### Compilation Errors

| Error | Solution |
|-------|----------|
| `'TFT_eSPI' was not declared` | Install TFT_eSPI library |
| `#error User_Setup.h not configured` | Configure TFT_eSPI per instructions above |
| `'Preferences' not found` | Update ESP32 board package to 2.0.x |
| `GPIO conflicts` | Verify you removed RGB LED from board |

### Runtime Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| Display shows garbage | Wrong TFT_eSPI config | Re-check User_Setup.h configuration |
| Display is blank/white | Power issue or wrong pins | Verify 5V power supply, check wiring |
| No radar data | LD2450 not connected | Check GPIO 22/27 wiring, verify 5V power to LD2450 |
| Buttons don't work | RGB LED not removed | Remove RGB LED from GPIO 4, 16, 17 |
| No audio | Wrong pin or no speaker | Check GPIO 25 connection to PAM8403 |
| Targets jump around | Normal without filtering | Filtering is implemented (FILTER_ALPHA=0.25) |
| System hangs | Watchdog timeout | Check for infinite loops, reduce debug output |

### Serial Monitor Shows Nothing

1. Verify baud rate is **115200**
2. Try pressing RESET button on CYD
3. Check `DEBUG_SERIAL` is `true` in `config.h`
4. Try different USB cable

### Display Shows Test Pattern

If you see color bars or test pattern:
- The display is working but code hasn't uploaded
- Re-upload the sketch
- Press RESET button

---

## Using the Motion Tracker

### Controls

**5-Way Navigation Switch:**
- **UP**: Navigate menu up
- **DOWN**: Navigate menu down
- **LEFT**: Decrease setting value
- **RIGHT**: Increase setting value
- **CENTER**: Open menu / Select item / Confirm

### Main Display (Scanning Mode)

**Status Bar** (top):
- `TGT:X` - Number of targets detected
- `RNG:Xm` - Current range setting
- `BAT:X%` - Battery level

**Radar Display** (center):
- Green sweeping arc animation
- Colored dots show detected targets:
  - **Red**: Critical (<1m)
  - **Yellow**: Warning (1-3m)
  - **Green**: Normal (3-8m)
- Numbers label each target (1, 2, 3)

**Info Bar** (bottom):
- Target coordinates and distance
- Format: `T1: 2.5m  45 deg`

### Menu System

Press **CENTER** button to open menu. Navigate with UP/DOWN, adjust values with LEFT/RIGHT.

**Menu Options:**
1. **Volume** (0-100): Audio beep volume
2. **Brightness** (0-100): Display backlight level
3. **Range** (2-8m): Maximum detection range
4. **Sensitivity** (0-100): Radar sensitivity (placeholder)
5. **Audio** (OFF/BEEP): Audio mode
6. **About**: Version and hardware info
7. **Exit**: Return to scanning mode

Settings are automatically saved to NVRAM.

### Audio Feedback

When targets are detected:
- **Beep frequency** increases as target gets closer
- **Beep rate** increases as target gets closer
- Mimics the iconic M314 motion tracker sound

Example:
- 8m away: Low pitch (200Hz), slow beeps (1 per second)
- 1m away: High pitch (2000Hz), rapid beeps (10 per second)

---

## Customization

### Adjusting Colors

Edit `config.h` and change color definitions:

```cpp
// Example: Change sweep color to blue
#define COLOR_SWEEP  0x001F  // Blue instead of green

// RGB565 color format:
// Red:   0xF800
// Green: 0x07E0
// Blue:  0x001F
// White: 0xFFFF
// Black: 0x0000
```

Use an [RGB565 color picker](https://chrishewett.com/blog/true-rgb565-colour-picker/) online.

### Adjusting Radar Range

In `config.h`:

```cpp
#define MAX_RANGE_METERS  8.0  // Change to 4.0 for shorter range
```

Or adjust via menu: **Range** setting (2-8m)

### Adjusting Sweep Speed

In `config.h`:

```cpp
#define SWEEP_SPEED_DEG  3.0  // Increase for faster sweep (e.g., 5.0)
```

### Adjusting Audio

In `config.h`:

```cpp
#define MIN_BEEP_FREQ_HZ   200   // Lower = deeper sound
#define MAX_BEEP_FREQ_HZ   2000  // Higher = sharper sound
#define BEEP_DURATION_MS   50    // Longer = sustained beep
```

### Enable Debug Output

In `config.h`:

```cpp
#define DEBUG_SERIAL      true   // Enable serial debugging
#define DEBUG_RADAR_DATA  true   // Show raw radar data
#define DEBUG_FPS         true   // Show frames per second
```

---

## Performance Optimization

### If Display is Slow/Laggy:

1. **Reduce sweep trail length:**
   ```cpp
   #define SWEEP_TRAIL_LENGTH  15  // Default: 30
   ```

2. **Lower frame rate:**
   ```cpp
   #define TARGET_FPS  20  // Default: 30
   ```

3. **Simplify sweep drawing:**
   Comment out fade trail in `drawRadarSweep()` function

### If Audio Stutters:

1. **Increase `RADAR_UPDATE_MS`:**
   ```cpp
   #define RADAR_UPDATE_MS  200  // Default: 100 (slower updates)
   ```

2. **Reduce beep duration:**
   ```cpp
   #define BEEP_DURATION_MS  30  // Default: 50
   ```

---

## Advanced Features

### Battery Voltage Monitoring

To enable GPIO 35 battery monitoring:

1. Wire voltage divider (2x 100kΩ) from battery to GPIO 35
2. Uncomment battery reading code in `loop()` function
3. Add this function:

```cpp
float readBatteryVoltage() {
  int adcValue = analogRead(BATTERY_ADC_PIN);
  float voltage = (adcValue / (float)ADC_RESOLUTION) * ADC_VREF * VOLTAGE_DIVIDER_RATIO;
  return voltage;
}
```

### Sleep Mode

Currently disabled. To implement:

1. Set `SLEEP_TIMEOUT_MS` in `config.h`
2. Use `esp_sleep_enable_ext0_wakeup(BTN_CENTER, LOW)` for wakeup
3. Call `esp_deep_sleep_start()` after timeout

### WiFi Data Logging

Add WiFi code to send radar data to server/dashboard:

```cpp
#include <WiFi.h>

void setup() {
  WiFi.begin("SSID", "PASSWORD");
  // Send target data via HTTP POST
}
```

---

## Updating Firmware

To update your motion tracker:

1. Download latest code from GitHub
2. Open new version in Arduino IDE
3. Verify and upload as normal
4. Settings in NVRAM are preserved across updates

---

## Example Code Snippets

### Reading Radar Data Manually

```cpp
void loop() {
  if (radarSerial.available()) {
    uint8_t byte = radarSerial.read();
    Serial.printf("0x%02X ", byte);
  }
}
```

### Testing Display

```cpp
void setup() {
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(120, 160, 50, TFT_GREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(50, 150);
  tft.println("TEST OK");
}
```

### Testing Audio

```cpp
void loop() {
  playBeep(1000, 100);  // 1kHz for 100ms
  delay(500);
}
```

---

## Support & Resources

### Documentation
- [Main README](../README.md) - Project overview
- [Parts List](../PARTS_LIST.md) - Component sourcing
- [Wiring Guide](../WIRING_GUIDE.md) - Pin connections
- [Assembly Guide](../ASSEMBLY.md) - Step-by-step build

### Hardware Resources
- [ESP32 CYD Documentation](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [LD2450 Datasheet](https://en.ai-thinker.com/Uploads/file/20231016/20231016032622_13559.pdf)

### Community
- **GitHub Issues**: Report bugs or request features
- **GitHub Discussions**: Ask questions, share builds

### Troubleshooting Help
1. Check [WIRING_GUIDE.md](../WIRING_GUIDE.md) - Testing & Troubleshooting section
2. Open GitHub issue with:
   - Hardware details (ESP32 model, LD2450, etc.)
   - Error messages or photos
   - Serial monitor output
   - What you've already tried

---

## Known Issues & Limitations

1. **LD2450 Detection Range**: Effective range is 6m, spec says 8m
2. **Target Tracking**: Can lose targets if they move too quickly
3. **Audio Quality**: Square wave, not sine wave (can add DacTone library for improvement)
4. **Battery Monitoring**: Requires external voltage divider circuit
5. **Sleep Mode**: Not yet implemented
6. **WiFi**: Disabled to save power and reduce complexity

---

## Future Improvements

Planned features:
- [ ] Sine wave audio generation (better sound quality)
- [ ] Deep sleep mode for battery saving
- [ ] Target history trails
- [ ] Alarm zones with distance thresholds
- [ ] SD card data logging
- [ ] WiFi remote monitoring
- [ ] OTA (over-the-air) firmware updates

---

## Credits

- **Hardware**: ESP32 CYD community, Hi-Link Electronic
- **Inspiration**: Aliens (1986) M314 Motion Tracker prop
- **Libraries**: Bodmer (TFT_eSPI), Espressif (ESP32 Core)

## License

MIT License - See [LICENSE](../LICENSE) file

---

**Questions? Issues? Suggestions?**

Open an issue on GitHub or check existing documentation!

**Happy tracking!** 🎯
