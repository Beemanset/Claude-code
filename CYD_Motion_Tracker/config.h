/*
 * config.h - Hardware Configuration and Constants
 * CYD Handheld Radar Motion Tracker
 *
 * Pin definitions, constants, and system configuration for
 * ESP32-2432S028 (CYD) with LD2450 mmWave radar sensor
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// HARDWARE GPIO PIN DEFINITIONS
// ============================================================================

// LD2450 Radar Sensor (UART)
#define RADAR_RX_PIN        22    // ESP32 RX ← LD2450 TX
#define RADAR_TX_PIN        27    // ESP32 TX → LD2450 RX

// Audio Output (DAC)
#define DAC_AUDIO_PIN       25    // ESP32 DAC1 → PAM8403 amplifier

// 5-Way Navigation Switch
#define BTN_UP              4     // UP button
#define BTN_DOWN            16    // DOWN button
#define BTN_LEFT            17    // LEFT button
#define BTN_RIGHT           21    // RIGHT button
#define BTN_CENTER          2     // CENTER/SELECT button

// Battery Monitoring (Optional)
#define BATTERY_ADC_PIN     35    // Voltage divider input (input-only)

// Display Backlight PWM (for brightness control)
#define BACKLIGHT_PIN       32    // PWM channel for backlight
#define BACKLIGHT_CHANNEL   0     // LEDC channel
#define BACKLIGHT_FREQ      5000  // 5kHz PWM frequency
#define BACKLIGHT_RES       8     // 8-bit resolution (0-255)

// ============================================================================
// RADAR SENSOR CONFIGURATION
// ============================================================================

#define RADAR_BAUD_RATE     256000    // LD2450 UART baud rate
#define MAX_TARGETS         3         // Maximum simultaneous targets
#define MAX_RANGE_MM        8000      // Maximum detection range (8m in mm)
#define MAX_RANGE_METERS    8.0       // Maximum range in meters
#define RADAR_UPDATE_MS     100       // Target update rate (10Hz)

// Target filtering
#define TARGET_TIMEOUT_MS   2000      // Remove stale targets after 2 seconds
#define FILTER_ALPHA        0.25      // Moving average filter coefficient (0-1)

// Detection zones (in mm)
#define ZONE_CRITICAL       1000      // <1m = critical (red)
#define ZONE_WARNING        3000      // 1-3m = warning (yellow)
#define ZONE_NORMAL         8000      // 3-8m = normal (green)

// ============================================================================
// DISPLAY CONFIGURATION
// ============================================================================

// Screen dimensions
#define SCREEN_WIDTH        240
#define SCREEN_HEIGHT       320
#define SCREEN_ROTATION     0         // 0=Portrait, 1=Landscape, etc.

// Radar display area
#define RADAR_CENTER_X      120       // Center X coordinate
#define RADAR_CENTER_Y      180       // Center Y coordinate
#define RADAR_RADIUS        100       // Radar sweep radius (pixels)
#define RADAR_ARC_ANGLE     120       // Sweep arc width (degrees)

// Display zones
#define STATUS_BAR_HEIGHT   30        // Top status bar
#define INFO_BAR_HEIGHT     40        // Bottom info bar
#define RADAR_TOP           STATUS_BAR_HEIGHT
#define RADAR_BOTTOM        (SCREEN_HEIGHT - INFO_BAR_HEIGHT)

// Animation
#define SWEEP_SPEED_DEG     3.0       // Degrees per frame
#define TARGET_FPS          30        // Target frame rate
#define FRAME_TIME_MS       (1000 / TARGET_FPS)

// Sweep trail effect
#define SWEEP_TRAIL_LENGTH  30        // Number of fade steps
#define SWEEP_FADE_STEP     8         // Color decrement per step

// ============================================================================
// COLOR DEFINITIONS (RGB565 format)
// ============================================================================

// Base colors
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_RED           0xF800
#define COLOR_GREEN         0x07E0
#define COLOR_BLUE          0x001F
#define COLOR_YELLOW        0xFFE0
#define COLOR_CYAN          0x07FF
#define COLOR_MAGENTA       0xF81F

// Radar colors (green theme - Aliens style)
#define COLOR_BG            0x0000    // Black background
#define COLOR_GRID          0x0320    // Dark green grid
#define COLOR_SWEEP         0x07E0    // Bright green sweep
#define COLOR_SWEEP_FADE    0x0340    // Faded green trail
#define COLOR_TEXT          0x07E0    // Green text
#define COLOR_TEXT_DIM      0x0320    // Dim green text

// Target colors by distance
#define COLOR_TARGET_CRITICAL   0xF800    // Red (<1m)
#define COLOR_TARGET_WARNING    0xFFE0    // Yellow (1-3m)
#define COLOR_TARGET_NORMAL     0x07E0    // Green (3-8m)

// Menu colors
#define COLOR_MENU_BG       0x1082    // Dark gray
#define COLOR_MENU_TITLE    0x07E0    // Green
#define COLOR_MENU_TEXT     0xFFFF    // White
#define COLOR_MENU_SELECT   0x07E0    // Green highlight
#define COLOR_MENU_BAR      0x0320    // Dark green bar

// Status indicators
#define COLOR_STATUS_OK     0x07E0    // Green
#define COLOR_STATUS_WARN   0xFFE0    // Yellow
#define COLOR_STATUS_ERROR  0xF800    // Red

// ============================================================================
// AUDIO CONFIGURATION
// ============================================================================

// Frequency mapping (Hz)
#define MIN_BEEP_FREQ_HZ    200       // Distant target (low pitch)
#define MAX_BEEP_FREQ_HZ    2000      // Close target (high pitch)

// Beep timing (ms)
#define MIN_BEEP_INTERVAL_MS    100   // Close target (fast beeps)
#define MAX_BEEP_INTERVAL_MS    1000  // Distant target (slow beeps)
#define BEEP_DURATION_MS        50    // Individual beep length

// Volume control
#define DEFAULT_VOLUME      128       // 50% volume (0-255)
#define MIN_VOLUME          0
#define MAX_VOLUME          255

// Audio modes
enum AudioMode {
  AUDIO_OFF = 0,
  AUDIO_BEEP = 1,
  AUDIO_TONE = 2,
  AUDIO_CONTINUOUS = 3
};

// ============================================================================
// MENU SYSTEM CONFIGURATION
// ============================================================================

// Menu items
enum MenuItem {
  MENU_VOLUME = 0,
  MENU_BRIGHTNESS,
  MENU_RANGE,
  MENU_SENSITIVITY,
  MENU_AUDIO_MODE,
  MENU_ABOUT,
  MENU_EXIT,
  MENU_COUNT
};

// Menu layout
#define MENU_TITLE_HEIGHT   30
#define MENU_ITEM_HEIGHT    35
#define MENU_HELP_HEIGHT    20
#define MENU_ITEMS_VISIBLE  6

// ============================================================================
// BUTTON CONFIGURATION
// ============================================================================

#define BUTTON_DEBOUNCE_MS  50        // Debounce delay
#define BUTTON_LONG_PRESS_MS 1000     // Long press threshold
#define BUTTON_REPEAT_MS    200       // Repeat rate when held

// ============================================================================
// BATTERY MONITORING
// ============================================================================

#define BATTERY_SAMPLES     10        // Number of ADC samples to average
#define BATTERY_READ_INTERVAL_MS 5000 // Update every 5 seconds

// Voltage thresholds (for 18650 Li-ion)
#define BATTERY_FULL_V      4.2       // Fully charged
#define BATTERY_NOMINAL_V   3.7       // Nominal voltage
#define BATTERY_LOW_V       3.4       // Low battery warning
#define BATTERY_CRITICAL_V  3.0       // Critical - shutdown imminent

// Voltage divider (2x 100kΩ = 50% division)
#define VOLTAGE_DIVIDER_RATIO 2.0
#define ADC_RESOLUTION      4096      // 12-bit ADC
#define ADC_VREF            3.3       // ESP32 ADC reference voltage

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================

// System states
enum SystemState {
  STATE_BOOT = 0,
  STATE_SCANNING,
  STATE_MENU,
  STATE_SLEEP
};

// Sleep mode settings
#define SLEEP_TIMEOUT_MS    300000    // 5 minutes of inactivity
#define WAKEUP_BUTTON       BTN_CENTER

// Debug settings
#define DEBUG_SERIAL        true      // Enable serial debug output
#define DEBUG_BAUD_RATE     115200    // Serial debug baud rate
#define DEBUG_FPS           false     // Show FPS counter
#define DEBUG_RADAR_DATA    false     // Print raw radar data
#define DEBUG_MEMORY        false     // Print memory usage

// NVRAM keys for settings storage
#define PREF_NAMESPACE      "tracker"
#define PREF_VOLUME         "volume"
#define PREF_BRIGHTNESS     "brightness"
#define PREF_RANGE          "range"
#define PREF_SENSITIVITY    "sensitivity"
#define PREF_AUDIO_MODE     "audioMode"

// Version information
#define FIRMWARE_VERSION    "1.0.0"
#define HARDWARE_VERSION    "ESP32-2432S028"
#define PROJECT_NAME        "CYD Motion Tracker"
#define PROJECT_URL         "github.com/Beemanset/Claude-code"

// ============================================================================
// MACRO HELPERS
// ============================================================================

// Constrain value between min and max
#ifndef constrain
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

// Map value from one range to another
#ifndef map
#define map(x, in_min, in_max, out_min, out_max) \
  ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

// Debug print macros
#if DEBUG_SERIAL
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Target information structure
struct Target {
  float x;              // X coordinate (mm, -4000 to 4000)
  float y;              // Y coordinate (mm, 0 to 6000)
  int16_t speed;        // Speed (cm/s)
  uint16_t resolution;  // Resolution value
  bool valid;           // Is target currently detected
  unsigned long lastSeen; // Timestamp of last detection (ms)

  // Derived values
  float distance;       // Distance from origin (mm)
  float angle;          // Angle from Y-axis (degrees, 0-360)
};

// System status structure
struct SystemStatus {
  bool radarConnected;
  bool audioEnabled;
  int targetCount;
  float batteryVoltage;
  int batteryPercent;
  unsigned long uptime;
  int freeHeap;
};

#endif // CONFIG_H
