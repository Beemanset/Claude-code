/*
 * CYD_Motion_Tracker.ino - Main Arduino Sketch
 * M314-Inspired Handheld Motion Tracker
 *
 * Hardware: ESP32-2432S028 (CYD) + HLK-LD2450 mmWave Radar
 *
 * Features:
 * - Real-time multi-target tracking (up to 3 targets)
 * - Animated radar sweep display (Aliens M314 style)
 * - Proximity audio feedback (beep frequency/rate based on distance)
 * - 5-way navigation switch for menu control
 * - Battery level monitoring
 * - Adjustable settings (volume, brightness, range, sensitivity)
 *
 * Author: CYD Motion Tracker Project
 * Version: 1.0.0
 * License: MIT
 */

#include <TFT_eSPI.h>
#include <Preferences.h>
#include "config.h"

// Note: Separate .h/.cpp files for classes will be included here
// For now, we'll implement simplified versions inline

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

TFT_eSPI tft = TFT_eSPI();
Preferences preferences;

// Hardware Serial for LD2450 (UART1)
HardwareSerial radarSerial(1);

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

SystemState currentState = STATE_BOOT;
Target targets[MAX_TARGETS];
int targetCount = 0;
SystemStatus systemStatus;

// Display animation
float sweepAngle = 0.0;
unsigned long lastSweepUpdate = 0;
unsigned long lastRadarUpdate = 0;
unsigned long lastFrameTime = 0;

// Button states
bool btnUpState = HIGH;
bool btnDownState = HIGH;
bool btnLeftState = HIGH;
bool btnRightState = HIGH;
bool btnCenterState = HIGH;
unsigned long lastButtonPress = 0;

// Settings (loaded from NVRAM)
int volumeLevel = 50;          // 0-100
int brightnessLevel = 80;      // 0-100
int rangeMeters = 6;           // 2-8
int sensitivity = 50;          // 0-100
AudioMode audioMode = AUDIO_BEEP;

// Audio control
unsigned long lastBeepTime = 0;
bool audioEnabled = true;

// Menu state
bool menuVisible = false;
int menuSelection = 0;

// ============================================================================
// SETUP FUNCTION
// ============================================================================

void setup() {
  // Initialize Serial for debugging
  Serial.begin(DEBUG_BAUD_RATE);
  delay(100);
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println(PROJECT_NAME);
  Serial.println("Version: " FIRMWARE_VERSION);
  Serial.println("========================================\n");

  // Initialize buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);

  // Initialize DAC for audio
  pinMode(DAC_AUDIO_PIN, OUTPUT);
  dacWrite(DAC_AUDIO_PIN, 0);

  // Initialize display
  Serial.println("Initializing display...");
  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(COLOR_BG);

  // Setup backlight PWM
  ledcSetup(BACKLIGHT_CHANNEL, BACKLIGHT_FREQ, BACKLIGHT_RES);
  ledcAttachPin(BACKLIGHT_PIN, BACKLIGHT_CHANNEL);

  // Show boot screen
  showBootScreen();

  // Load settings from NVRAM
  loadSettings();
  applyBrightness();

  // Initialize LD2450 radar
  Serial.println("Initializing LD2450 radar...");
  radarSerial.begin(RADAR_BAUD_RATE, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  delay(500);

  // Initialize targets
  for (int i = 0; i < MAX_TARGETS; i++) {
    targets[i].valid = false;
    targets[i].lastSeen = 0;
  }

  // Initialize system status
  systemStatus.radarConnected = true;
  systemStatus.audioEnabled = true;
  systemStatus.targetCount = 0;
  systemStatus.batteryVoltage = 3.7;
  systemStatus.batteryPercent = 50;

  // Transition to scanning mode
  currentState = STATE_SCANNING;
  drawRadarBackground();

  Serial.println("Initialization complete!");
  Serial.println("Entering scanning mode...\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  unsigned long now = millis();

  // Update button states
  updateButtons();

  // Handle state-specific logic
  switch (currentState) {
    case STATE_SCANNING:
      handleScanningMode();
      break;

    case STATE_MENU:
      handleMenuMode();
      break;

    case STATE_SLEEP:
      handleSleepMode();
      break;
  }

  // Frame rate limiting
  unsigned long frameTime = millis() - lastFrameTime;
  if (frameTime < FRAME_TIME_MS) {
    delay(FRAME_TIME_MS - frameTime);
  }
  lastFrameTime = millis();
}

// ============================================================================
// STATE HANDLERS
// ============================================================================

void handleScanningMode() {
  unsigned long now = millis();

  // Update radar data
  if (now - lastRadarUpdate >= RADAR_UPDATE_MS) {
    updateRadarData();
    lastRadarUpdate = now;
  }

  // Update sweep animation
  if (now - lastSweepUpdate >= 16) {  // ~60 FPS
    sweepAngle += SWEEP_SPEED_DEG;
    if (sweepAngle >= 360.0) sweepAngle = 0.0;
    lastSweepUpdate = now;
  }

  // Draw radar display
  drawRadarSweep();
  drawTargets();
  drawStatusBar();
  drawInfoBar();

  // Update audio based on closest target
  if (audioEnabled && audioMode != AUDIO_OFF) {
    updateProximityAudio();
  }

  // Check for menu button
  if (digitalRead(BTN_CENTER) == LOW && now - lastButtonPress > 500) {
    currentState = STATE_MENU;
    menuVisible = true;
    menuSelection = 0;
    showMenu();
    lastButtonPress = now;
  }
}

void handleMenuMode() {
  // Menu is already drawn, just handle navigation
  unsigned long now = millis();

  // Debounce delay
  if (now - lastButtonPress < 200) return;

  if (digitalRead(BTN_UP) == LOW) {
    menuSelection--;
    if (menuSelection < 0) menuSelection = MENU_COUNT - 1;
    showMenu();
    lastButtonPress = now;
  }
  else if (digitalRead(BTN_DOWN) == LOW) {
    menuSelection++;
    if (menuSelection >= MENU_COUNT) menuSelection = 0;
    showMenu();
    lastButtonPress = now;
  }
  else if (digitalRead(BTN_LEFT) == LOW) {
    adjustMenuValue(-1);
    showMenu();
    lastButtonPress = now;
  }
  else if (digitalRead(BTN_RIGHT) == LOW) {
    adjustMenuValue(1);
    showMenu();
    lastButtonPress = now;
  }
  else if (digitalRead(BTN_CENTER) == LOW) {
    if (menuSelection == MENU_EXIT) {
      // Exit menu
      currentState = STATE_SCANNING;
      menuVisible = false;
      drawRadarBackground();
    }
    else if (menuSelection == MENU_ABOUT) {
      showAbout();
      delay(3000);
      showMenu();
    }
    lastButtonPress = now;
  }
}

void handleSleepMode() {
  // TODO: Implement deep sleep mode
  // For now, just go back to scanning on button press
  if (digitalRead(BTN_CENTER) == LOW) {
    currentState = STATE_SCANNING;
    drawRadarBackground();
  }
}

// ============================================================================
// RADAR DATA PROCESSING
// ============================================================================

void updateRadarData() {
  // Simple LD2450 parser (29-byte frames)
  static uint8_t buffer[29];
  static int bufferIndex = 0;
  static bool inFrame = false;

  while (radarSerial.available()) {
    uint8_t byte = radarSerial.read();

    // Look for frame header (0xAA 0xFF 0x03 0x00)
    if (!inFrame && byte == 0xAA) {
      buffer[0] = byte;
      bufferIndex = 1;
      inFrame = true;
    }
    else if (inFrame) {
      buffer[bufferIndex++] = byte;

      // Complete frame received
      if (bufferIndex >= 29) {
        // Validate header
        if (buffer[1] == 0xFF && buffer[2] == 0x03 && buffer[3] == 0x00) {
          // Verify checksum (XOR of all bytes except last)
          uint8_t checksum = 0;
          for (int i = 0; i < 28; i++) {
            checksum ^= buffer[i];
          }

          if (checksum == buffer[28]) {
            parseTargets(buffer);
          }
        }
        bufferIndex = 0;
        inFrame = false;
      }
    }
  }

  // Remove stale targets
  unsigned long now = millis();
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].valid && (now - targets[i].lastSeen) > TARGET_TIMEOUT_MS) {
      targets[i].valid = false;
    }
  }

  // Count valid targets
  targetCount = 0;
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].valid) targetCount++;
  }
  systemStatus.targetCount = targetCount;
}

void parseTargets(uint8_t* data) {
  unsigned long now = millis();

  for (int i = 0; i < MAX_TARGETS; i++) {
    int offset = 4 + (i * 8);  // Each target is 8 bytes

    int16_t x = (int16_t)(data[offset] | (data[offset + 1] << 8));
    int16_t y = (int16_t)(data[offset + 2] | (data[offset + 3] << 8));
    int16_t speed = (int16_t)(data[offset + 4] | (data[offset + 5] << 8));
    uint16_t resolution = (uint16_t)(data[offset + 6] | (data[offset + 7] << 8));

    // Valid target check (resolution > 0)
    if (resolution > 0) {
      // Apply simple low-pass filter if target already existed
      if (targets[i].valid) {
        targets[i].x = FILTER_ALPHA * x + (1.0 - FILTER_ALPHA) * targets[i].x;
        targets[i].y = FILTER_ALPHA * y + (1.0 - FILTER_ALPHA) * targets[i].y;
      } else {
        targets[i].x = x;
        targets[i].y = y;
      }

      targets[i].speed = speed;
      targets[i].resolution = resolution;
      targets[i].valid = true;
      targets[i].lastSeen = now;

      // Calculate derived values
      targets[i].distance = sqrt(targets[i].x * targets[i].x + targets[i].y * targets[i].y);
      targets[i].angle = atan2(targets[i].x, targets[i].y) * 180.0 / PI;
      if (targets[i].angle < 0) targets[i].angle += 360.0;
    }
    else {
      targets[i].valid = false;
    }
  }
}

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

void showBootScreen() {
  tft.fillScreen(COLOR_BG);
  tft.setTextColor(COLOR_TEXT);

  // Project name
  tft.setTextSize(3);
  tft.setCursor(20, 100);
  tft.println("M314");

  tft.setTextSize(2);
  tft.setCursor(15, 135);
  tft.println("MOTION TRACKER");

  // Status
  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT_DIM);
  tft.setCursor(60, 180);
  tft.println("INITIALIZING...");

  // Version
  tft.setCursor(70, 280);
  tft.print("v");
  tft.print(FIRMWARE_VERSION);

  delay(2000);
}

void drawRadarBackground() {
  tft.fillScreen(COLOR_BG);

  // Draw range rings
  for (int r = 20; r <= RADAR_RADIUS; r += 20) {
    tft.drawCircle(RADAR_CENTER_X, RADAR_CENTER_Y, r, COLOR_GRID);
  }

  // Draw crosshairs
  tft.drawLine(RADAR_CENTER_X - RADAR_RADIUS, RADAR_CENTER_Y,
               RADAR_CENTER_X + RADAR_RADIUS, RADAR_CENTER_Y, COLOR_GRID);
  tft.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y - RADAR_RADIUS,
               RADAR_CENTER_X, RADAR_CENTER_Y + RADAR_RADIUS, COLOR_GRID);

  // Draw degree markings
  for (int angle = 0; angle < 360; angle += 30) {
    float rad = angle * PI / 180.0;
    int x1 = RADAR_CENTER_X + (RADAR_RADIUS - 5) * cos(rad);
    int y1 = RADAR_CENTER_Y + (RADAR_RADIUS - 5) * sin(rad);
    int x2 = RADAR_CENTER_X + RADAR_RADIUS * cos(rad);
    int y2 = RADAR_CENTER_Y + RADAR_RADIUS * sin(rad);
    tft.drawLine(x1, y1, x2, y2, COLOR_GRID);
  }
}

void drawRadarSweep() {
  // Draw sweep line with fade trail
  for (int i = 0; i < SWEEP_TRAIL_LENGTH; i++) {
    float angle = sweepAngle - i * 3.0;  // Trail spacing
    if (angle < 0) angle += 360.0;

    // Calculate fade color
    int green = 255 - (i * SWEEP_FADE_STEP);
    if (green < 32) green = 32;
    uint16_t color = tft.color565(0, green, 0);

    // Draw radial line
    float rad = angle * PI / 180.0;
    int x = RADAR_CENTER_X + RADAR_RADIUS * cos(rad);
    int y = RADAR_CENTER_Y + RADAR_RADIUS * sin(rad);

    // Erase old line first (draw in background color)
    float oldAngle = angle - SWEEP_SPEED_DEG;
    float oldRad = oldAngle * PI / 180.0;
    int oldX = RADAR_CENTER_X + RADAR_RADIUS * cos(oldRad);
    int oldY = RADAR_CENTER_Y + RADAR_RADIUS * sin(oldRad);
    tft.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, oldX, oldY, COLOR_BG);

    // Draw new line
    tft.drawLine(RADAR_CENTER_X, RADAR_CENTER_Y, x, y, color);
  }
}

void drawTargets() {
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (!targets[i].valid) continue;

    // Scale and convert coordinates
    float scaledDist = (targets[i].distance / MAX_RANGE_MM) * RADAR_RADIUS;
    float rad = (90 - targets[i].angle) * PI / 180.0;  // Adjust for screen orientation

    int screenX = RADAR_CENTER_X + scaledDist * cos(rad);
    int screenY = RADAR_CENTER_Y - scaledDist * sin(rad);

    // Choose color based on distance
    uint16_t color;
    if (targets[i].distance < ZONE_CRITICAL) {
      color = COLOR_TARGET_CRITICAL;
    } else if (targets[i].distance < ZONE_WARNING) {
      color = COLOR_TARGET_WARNING;
    } else {
      color = COLOR_TARGET_NORMAL;
    }

    // Draw target marker (pulsing effect)
    int pulseSize = 3 + (millis() % 500) / 100;
    tft.fillCircle(screenX, screenY, pulseSize, color);
    tft.drawCircle(screenX, screenY, pulseSize + 2, color);

    // Draw target number
    tft.setTextSize(1);
    tft.setTextColor(color);
    tft.setCursor(screenX + 6, screenY - 3);
    tft.print(i + 1);
  }
}

void drawStatusBar() {
  // Clear status bar area
  tft.fillRect(0, 0, SCREEN_WIDTH, STATUS_BAR_HEIGHT, COLOR_BG);

  // Draw border
  tft.drawLine(0, STATUS_BAR_HEIGHT - 1, SCREEN_WIDTH, STATUS_BAR_HEIGHT - 1, COLOR_GRID);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT);

  // Target count
  tft.setCursor(5, 10);
  tft.print("TGT:");
  tft.print(targetCount);

  // Range
  tft.setCursor(80, 10);
  tft.print("RNG:");
  tft.print(rangeMeters);
  tft.print("m");

  // Battery (placeholder)
  tft.setCursor(160, 10);
  tft.print("BAT:");
  tft.print(systemStatus.batteryPercent);
  tft.print("%");
}

void drawInfoBar() {
  // Clear info bar area
  int startY = SCREEN_HEIGHT - INFO_BAR_HEIGHT;
  tft.fillRect(0, startY, SCREEN_WIDTH, INFO_BAR_HEIGHT, COLOR_BG);

  // Draw border
  tft.drawLine(0, startY, SCREEN_WIDTH, startY, COLOR_GRID);

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT_DIM);

  // Display target info
  if (targetCount > 0) {
    for (int i = 0; i < MAX_TARGETS && i < 3; i++) {
      if (targets[i].valid) {
        int y = startY + 5 + (i * 10);
        tft.setCursor(5, y);
        tft.printf("T%d: %.1fm  %d deg", i + 1, targets[i].distance / 1000.0, (int)targets[i].angle);
      }
    }
  } else {
    tft.setCursor(60, startY + 15);
    tft.print("NO TARGETS");
  }
}

// ============================================================================
// MENU FUNCTIONS
// ============================================================================

void showMenu() {
  tft.fillScreen(COLOR_MENU_BG);

  // Title bar
  tft.fillRect(0, 0, SCREEN_WIDTH, MENU_TITLE_HEIGHT, COLOR_MENU_TITLE);
  tft.setTextColor(COLOR_BG);
  tft.setTextSize(2);
  tft.setCursor(50, 8);
  tft.print("SETTINGS");

  // Menu items
  const char* labels[] = {"Volume", "Brightness", "Range", "Sensitivity", "Audio", "About", "Exit"};

  for (int i = 0; i < MENU_COUNT; i++) {
    int y = MENU_TITLE_HEIGHT + 5 + (i * MENU_ITEM_HEIGHT);

    // Highlight selected
    if (i == menuSelection) {
      tft.fillRect(0, y, SCREEN_WIDTH, MENU_ITEM_HEIGHT - 2, COLOR_MENU_SELECT);
      tft.setTextColor(COLOR_BG);
    } else {
      tft.setTextColor(COLOR_MENU_TEXT);
    }

    tft.setTextSize(2);
    tft.setCursor(10, y + 8);
    tft.print(labels[i]);

    // Draw value
    tft.setTextSize(1);
    tft.setCursor(170, y + 12);
    switch (i) {
      case MENU_VOLUME:
        tft.print(volumeLevel);
        break;
      case MENU_BRIGHTNESS:
        tft.print(brightnessLevel);
        break;
      case MENU_RANGE:
        tft.print(rangeMeters);
        tft.print("m");
        break;
      case MENU_SENSITIVITY:
        tft.print(sensitivity);
        break;
      case MENU_AUDIO_MODE:
        tft.print(audioMode == AUDIO_OFF ? "OFF" : "BEEP");
        break;
    }
  }

  // Help text
  tft.setTextColor(COLOR_TEXT_DIM);
  tft.setTextSize(1);
  tft.setCursor(30, SCREEN_HEIGHT - 15);
  tft.print("UP/DN NAV  L/R ADJ");
}

void adjustMenuValue(int direction) {
  switch (menuSelection) {
    case MENU_VOLUME:
      volumeLevel = constrain(volumeLevel + direction * 10, 0, 100);
      break;
    case MENU_BRIGHTNESS:
      brightnessLevel = constrain(brightnessLevel + direction * 10, 0, 100);
      applyBrightness();
      break;
    case MENU_RANGE:
      rangeMeters = constrain(rangeMeters + direction, 2, 8);
      break;
    case MENU_SENSITIVITY:
      sensitivity = constrain(sensitivity + direction * 10, 0, 100);
      break;
    case MENU_AUDIO_MODE:
      audioMode = (audioMode == AUDIO_OFF) ? AUDIO_BEEP : AUDIO_OFF;
      break;
  }
  saveSettings();
}

void showAbout() {
  tft.fillScreen(COLOR_BG);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(2);
  tft.setCursor(70, 60);
  tft.print("ABOUT");

  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT_DIM);
  tft.setCursor(20, 100);
  tft.print(PROJECT_NAME);
  tft.setCursor(20, 120);
  tft.print("Version: ");
  tft.print(FIRMWARE_VERSION);
  tft.setCursor(20, 140);
  tft.print("Hardware: ");
  tft.print(HARDWARE_VERSION);
  tft.setCursor(20, 160);
  tft.print("Sensor: HLK-LD2450");
  tft.setCursor(20, 200);
  tft.print(PROJECT_URL);
}

// ============================================================================
// AUDIO FUNCTIONS
// ============================================================================

void updateProximityAudio() {
  if (targetCount == 0) {
    // No targets, stop audio
    return;
  }

  // Find closest target
  float minDist = 999999.0;
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].valid && targets[i].distance < minDist) {
      minDist = targets[i].distance;
    }
  }

  // Map distance to frequency and beep rate
  float distMeters = minDist / 1000.0;
  int frequency = map(distMeters * 100, 0, MAX_RANGE_METERS * 100,
                     MAX_BEEP_FREQ_HZ, MIN_BEEP_FREQ_HZ);
  int interval = map(distMeters * 100, 0, MAX_RANGE_METERS * 100,
                    MIN_BEEP_INTERVAL_MS, MAX_BEEP_INTERVAL_MS);

  unsigned long now = millis();
  if (now - lastBeepTime >= interval) {
    playBeep(frequency, BEEP_DURATION_MS);
    lastBeepTime = now;
  }
}

void playBeep(int frequency, int duration) {
  if (volumeLevel == 0) return;

  int halfPeriodUs = (1000000 / frequency) / 2;
  int volume = map(volumeLevel, 0, 100, 0, 255);
  unsigned long startTime = millis();

  while (millis() - startTime < duration) {
    dacWrite(DAC_AUDIO_PIN, volume);
    delayMicroseconds(halfPeriodUs);
    dacWrite(DAC_AUDIO_PIN, 0);
    delayMicroseconds(halfPeriodUs);
  }

  dacWrite(DAC_AUDIO_PIN, 0);
}

// ============================================================================
// SETTINGS FUNCTIONS
// ============================================================================

void loadSettings() {
  preferences.begin(PREF_NAMESPACE, true);  // Read-only
  volumeLevel = preferences.getInt(PREF_VOLUME, 50);
  brightnessLevel = preferences.getInt(PREF_BRIGHTNESS, 80);
  rangeMeters = preferences.getInt(PREF_RANGE, 6);
  sensitivity = preferences.getInt(PREF_SENSITIVITY, 50);
  audioMode = (AudioMode)preferences.getInt(PREF_AUDIO_MODE, AUDIO_BEEP);
  preferences.end();

  Serial.println("Settings loaded from NVRAM");
}

void saveSettings() {
  preferences.begin(PREF_NAMESPACE, false);  // Read-write
  preferences.putInt(PREF_VOLUME, volumeLevel);
  preferences.putInt(PREF_BRIGHTNESS, brightnessLevel);
  preferences.putInt(PREF_RANGE, rangeMeters);
  preferences.putInt(PREF_SENSITIVITY, sensitivity);
  preferences.putInt(PREF_AUDIO_MODE, (int)audioMode);
  preferences.end();

  Serial.println("Settings saved to NVRAM");
}

void applyBrightness() {
  int pwmValue = map(brightnessLevel, 0, 100, 0, 255);
  ledcWrite(BACKLIGHT_CHANNEL, pwmValue);
}

// ============================================================================
// BUTTON FUNCTIONS
// ============================================================================

void updateButtons() {
  // Simple button reading (debouncing handled by delays in handlers)
  btnUpState = digitalRead(BTN_UP);
  btnDownState = digitalRead(BTN_DOWN);
  btnLeftState = digitalRead(BTN_LEFT);
  btnRightState = digitalRead(BTN_RIGHT);
  btnCenterState = digitalRead(BTN_CENTER);
}
