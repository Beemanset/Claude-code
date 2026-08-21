/*
 * GPIO_Pin_Test.ino - Hardware Validation Test
 *
 * Use this sketch to verify all GPIO connections are correct
 * before uploading the main motion tracker code.
 *
 * Tests:
 * - Display initialization
 * - Button inputs (5-way switch)
 * - Audio output (DAC on GPIO 26)
 * - Serial communication for radar
 *
 * Upload this first to confirm your wiring!
 */

#include <TFT_eSPI.h>

// Pin definitions (must match config.h)
#define RADAR_RX_PIN    22
#define RADAR_TX_PIN    27
#define DAC_AUDIO_PIN   26    // Corrected pin
#define BTN_UP          4
#define BTN_DOWN        16
#define BTN_LEFT        17
#define BTN_RIGHT       35    // Needs external 10kΩ pull-up!
#define BTN_CENTER      5     // Or GPIO 0 if using SD card
#define BACKLIGHT_PIN   21

TFT_eSPI tft = TFT_eSPI();
HardwareSerial radarSerial(1);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\n=================================");
  Serial.println("CYD Motion Tracker - GPIO Test");
  Serial.println("=================================\n");

  // Initialize buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT);        // External pull-up required!
  pinMode(BTN_CENTER, INPUT_PULLUP);

  Serial.println("✓ Buttons initialized");

  // Initialize DAC
  pinMode(DAC_AUDIO_PIN, OUTPUT);
  dacWrite(DAC_AUDIO_PIN, 0);
  Serial.println("✓ Audio DAC initialized (GPIO 26)");

  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  Serial.println("✓ Display initialized");

  // Setup backlight
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BACKLIGHT_PIN, 0);
  ledcWrite(0, 200);  // 80% brightness
  Serial.println("✓ Backlight configured");

  // Initialize radar serial
  radarSerial.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN);
  Serial.println("✓ Radar UART initialized (256000 baud)");

  // Display test pattern
  drawTestScreen();

  Serial.println("\n=================================");
  Serial.println("Hardware Test Ready!");
  Serial.println("=================================\n");
  Serial.println("Commands:");
  Serial.println("  b - Test buttons");
  Serial.println("  a - Test audio");
  Serial.println("  d - Test display");
  Serial.println("  r - Test radar serial");
  Serial.println("  s - Run all tests\n");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch(cmd) {
      case 'b':
        testButtons();
        break;
      case 'a':
        testAudio();
        break;
      case 'd':
        testDisplay();
        break;
      case 'r':
        testRadar();
        break;
      case 's':
        runAllTests();
        break;
    }
  }

  // Continuous button monitoring
  checkButtons();
  delay(10);
}

void drawTestScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.println("GPIO TEST");

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 100);
  tft.println("Display: OK");
  tft.setCursor(10, 120);
  tft.println("Send 's' for tests");

  // Draw test pattern
  tft.drawCircle(120, 200, 50, TFT_GREEN);
  tft.drawLine(70, 200, 170, 200, TFT_GREEN);
  tft.drawLine(120, 150, 120, 250, TFT_GREEN);
}

void testButtons() {
  Serial.println("\n--- Button Test ---");
  Serial.println("Press each button (10 seconds):");
  Serial.println("UP, DOWN, LEFT, RIGHT, CENTER");

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.println("BUTTON TEST");
  tft.setTextSize(1);
  tft.setCursor(20, 140);
  tft.println("Press each button...");

  unsigned long start = millis();
  bool tested[5] = {false, false, false, false, false};

  while (millis() - start < 10000) {
    if (digitalRead(BTN_UP) == LOW && !tested[0]) {
      Serial.println("✓ UP button works");
      tft.setCursor(20, 160);
      tft.println("UP: OK");
      tested[0] = true;
      delay(200);
    }
    if (digitalRead(BTN_DOWN) == LOW && !tested[1]) {
      Serial.println("✓ DOWN button works");
      tft.setCursor(20, 180);
      tft.println("DOWN: OK");
      tested[1] = true;
      delay(200);
    }
    if (digitalRead(BTN_LEFT) == LOW && !tested[2]) {
      Serial.println("✓ LEFT button works");
      tft.setCursor(20, 200);
      tft.println("LEFT: OK");
      tested[2] = true;
      delay(200);
    }
    if (digitalRead(BTN_RIGHT) == LOW && !tested[3]) {
      Serial.println("✓ RIGHT button works (GPIO 35)");
      tft.setCursor(20, 220);
      tft.println("RIGHT: OK");
      tested[3] = true;
      delay(200);
    }
    if (digitalRead(BTN_CENTER) == LOW && !tested[4]) {
      Serial.println("✓ CENTER button works");
      tft.setCursor(20, 240);
      tft.println("CENTER: OK");
      tested[4] = true;
      delay(200);
    }
  }

  // Check if all tested
  bool allOk = true;
  for (int i = 0; i < 5; i++) {
    if (!tested[i]) {
      allOk = false;
      Serial.print("✗ Button ");
      Serial.print(i);
      Serial.println(" not tested!");
    }
  }

  if (allOk) {
    Serial.println("\n✓ All buttons working!");
  } else {
    Serial.println("\n✗ Some buttons not tested - check wiring");
  }

  delay(2000);
  drawTestScreen();
}

void testAudio() {
  Serial.println("\n--- Audio Test ---");
  Serial.println("Playing frequency sweep (GPIO 26)...");

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.println("AUDIO TEST");

  // Test frequencies
  int frequencies[] = {200, 500, 1000, 1500, 2000};

  for (int i = 0; i < 5; i++) {
    int freq = frequencies[i];
    Serial.print("Playing ");
    Serial.print(freq);
    Serial.println(" Hz...");

    tft.setTextSize(3);
    tft.fillRect(0, 150, 240, 40, TFT_BLACK);
    tft.setCursor(60, 150);
    tft.print(freq);
    tft.println(" Hz");

    playTone(freq, 300);
    delay(200);
  }

  Serial.println("✓ Audio test complete");
  Serial.println("Did you hear 5 beeps? (200Hz-2000Hz)");

  delay(2000);
  drawTestScreen();
}

void testDisplay() {
  Serial.println("\n--- Display Test ---");

  // Color test
  Serial.println("Testing colors...");
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_WHITE);
  delay(500);

  // Pattern test
  Serial.println("Testing patterns...");
  tft.fillScreen(TFT_BLACK);
  for (int i = 0; i < 240; i += 20) {
    tft.drawLine(i, 0, i, 320, TFT_GREEN);
  }
  delay(1000);

  for (int i = 0; i < 320; i += 20) {
    tft.drawLine(0, i, 240, i, TFT_GREEN);
  }
  delay(1000);

  // Circle test
  tft.fillScreen(TFT_BLACK);
  for (int r = 10; r < 100; r += 10) {
    tft.drawCircle(120, 160, r, TFT_GREEN);
  }
  delay(1000);

  Serial.println("✓ Display test complete");

  drawTestScreen();
}

void testRadar() {
  Serial.println("\n--- Radar Serial Test ---");
  Serial.println("Listening for LD2450 data (10 seconds)...");
  Serial.println("Wave hand in front of sensor");

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_MAGENTA);
  tft.setTextSize(2);
  tft.setCursor(20, 100);
  tft.println("RADAR TEST");
  tft.setTextSize(1);
  tft.setCursor(20, 140);
  tft.println("Listening for data...");

  unsigned long start = millis();
  int bytesReceived = 0;
  bool dataDetected = false;

  while (millis() - start < 10000) {
    if (radarSerial.available()) {
      uint8_t byte = radarSerial.read();
      bytesReceived++;
      dataDetected = true;

      if (bytesReceived <= 50) {  // Print first 50 bytes
        Serial.print("0x");
        if (byte < 16) Serial.print("0");
        Serial.print(byte, HEX);
        Serial.print(" ");
        if (bytesReceived % 16 == 0) Serial.println();
      }
    }
  }

  Serial.println();
  Serial.print("Bytes received: ");
  Serial.println(bytesReceived);

  tft.setCursor(20, 160);
  if (dataDetected) {
    Serial.println("✓ Radar data detected!");
    tft.setTextColor(TFT_GREEN);
    tft.print("Data: ");
    tft.print(bytesReceived);
    tft.println(" bytes");
  } else {
    Serial.println("✗ No radar data received");
    Serial.println("Check:");
    Serial.println("  - LD2450 power (5V)");
    Serial.println("  - TX/RX wiring (GPIO 22/27)");
    Serial.println("  - Baud rate (256000)");
    tft.setTextColor(TFT_RED);
    tft.println("NO DATA!");
    tft.setCursor(20, 180);
    tft.setTextColor(TFT_YELLOW);
    tft.println("Check wiring");
  }

  delay(2000);
  drawTestScreen();
}

void runAllTests() {
  Serial.println("\n=================================");
  Serial.println("Running All Tests");
  Serial.println("=================================\n");

  testDisplay();
  delay(1000);
  testAudio();
  delay(1000);
  testButtons();
  delay(1000);
  testRadar();

  Serial.println("\n=================================");
  Serial.println("All Tests Complete!");
  Serial.println("=================================\n");
}

void checkButtons() {
  // Continuous monitoring for quick feedback
  static bool lastStates[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
  bool currentStates[5] = {
    digitalRead(BTN_UP),
    digitalRead(BTN_DOWN),
    digitalRead(BTN_LEFT),
    digitalRead(BTN_RIGHT),
    digitalRead(BTN_CENTER)
  };

  const char* names[] = {"UP", "DOWN", "LEFT", "RIGHT", "CENTER"};

  for (int i = 0; i < 5; i++) {
    if (currentStates[i] == LOW && lastStates[i] == HIGH) {
      Serial.print("Button: ");
      Serial.println(names[i]);
    }
    lastStates[i] = currentStates[i];
  }
}

void playTone(int frequency, int duration) {
  int halfPeriod = (1000000 / frequency) / 2;
  unsigned long start = millis();

  while (millis() - start < duration) {
    dacWrite(DAC_AUDIO_PIN, 200);  // ~80% volume
    delayMicroseconds(halfPeriod);
    dacWrite(DAC_AUDIO_PIN, 0);
    delayMicroseconds(halfPeriod);
  }

  dacWrite(DAC_AUDIO_PIN, 0);
}
