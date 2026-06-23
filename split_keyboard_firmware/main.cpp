#include <Arduino.h>
#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
Split Keyboard Firmware Package

File names:
- main.cpp      : Split keyboard firmware for a single Pico per half
- oled_demo.cpp : OLED demo sketch
- platformio.ini : PlatformIO Arduino project file

Pin configuration according to your schematic:
- Row pins: GPIO16, 17, 18, 19, 20
- Col pins: GPIO11, 12, 13, 14, 15
- Rotary encoder pins: GPIO6, GPIO7
- OLED I2C: GPIO26 = SCL, GPIO27 = SDA
- OLED address: 0x3C

*/

#define SIDE_LEFT 1

static constexpr uint8_t ROWS = 5;
static constexpr uint8_t COLS = 5;
static constexpr uint8_t DEBOUNCE_MS = 18;

static constexpr uint8_t ROW_PINS[ROWS] = {16, 17, 18, 19, 20};
static constexpr uint8_t COL_PINS[COLS] = {11, 12, 13, 14, 15};

static constexpr uint8_t ENC_A = 6;
static constexpr uint8_t ENC_B = 7;

static constexpr uint8_t OLED_SDA = 27;
static constexpr uint8_t OLED_SCL = 26;
static constexpr uint8_t OLED_ADDR = 0x3C;

static constexpr int SCREEN_W = 128;
static constexpr int SCREEN_H = 64;

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

struct DebouncedKey {
  bool raw = false;
  bool stable = false;
  uint32_t changed_at = 0;
};

static DebouncedKey keys[ROWS][COLS];
static bool oled_ok = false;
static uint32_t last_oled = 0;
static uint32_t last_scan = 0;

static int32_t encoder_steps = 0;
static int32_t encoder_detents = 0;
static uint8_t enc_prev = 0;

static char last_key_label[16] = "none";
static uint16_t active_keys = 0;
static uint16_t total_presses = 0;

// Common split layout; edit only these arrays to match your preferred keymap.
static const uint8_t keymap_left[ROWS][COLS] = {
  { 'q', 'w', 'e', 'r', 't' },
  { 'a', 's', 'd', 'f', 'g' },
  { 'z', 'x', 'c', 'v', 'b' },
  { KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, ' ', KEY_BACKSPACE },
  { 0, 0, 0, KEY_ESC, KEY_TAB },
};

static const uint8_t keymap_right[ROWS][COLS] = {
  { 'y', 'u', 'i', 'o', 'p' },
  { 'h', 'j', 'k', 'l', ';' },
  { 'n', 'm', ',', '.', '/' },
  { KEY_LEFT_SHIFT, KEY_RETURN, KEY_DELETE, KEY_UP_ARROW, KEY_DOWN_ARROW },
  { 0, 0, 0, KEY_PAGE_UP, KEY_PAGE_DOWN },
};

static inline const uint8_t (*active_keymap())[COLS] {
  return SIDE_LEFT ? keymap_left : keymap_right;
}

static const char* side_name() {
  return SIDE_LEFT ? "LEFT" : "RIGHT";
}

static void setLastKeyLabel(uint8_t code, bool down) {
  if (code == 0) {
    snprintf(last_key_label, sizeof(last_key_label), "unused");
    return;
  }

  if (code >= 32 && code <= 126) {
    snprintf(last_key_label, sizeof(last_key_label), "%c %s", (char)code, down ? "dn" : "up");
    return;
  }

  switch (code) {
    case KEY_RETURN:       snprintf(last_key_label, sizeof(last_key_label), "ENTER %s", down ? "dn" : "up"); break;
    case KEY_ESC:          snprintf(last_key_label, sizeof(last_key_label), "ESC %s", down ? "dn" : "up"); break;
    case KEY_TAB:          snprintf(last_key_label, sizeof(last_key_label), "TAB %s", down ? "dn" : "up"); break;
    case KEY_BACKSPACE:    snprintf(last_key_label, sizeof(last_key_label), "BSPC %s", down ? "dn" : "up"); break;
    case KEY_DELETE:       snprintf(last_key_label, sizeof(last_key_label), "DEL %s", down ? "dn" : "up"); break;
    case KEY_PAGE_UP:      snprintf(last_key_label, sizeof(last_key_label), "PGUP %s", down ? "dn" : "up"); break;
    case KEY_PAGE_DOWN:    snprintf(last_key_label, sizeof(last_key_label), "PGDN %s", down ? "dn" : "up"); break;
    case KEY_LEFT_CTRL:    snprintf(last_key_label, sizeof(last_key_label), "LCTRL %s", down ? "dn" : "up"); break;
    case KEY_LEFT_SHIFT:   snprintf(last_key_label, sizeof(last_key_label), "LSHFT %s", down ? "dn" : "up"); break;
    case KEY_LEFT_ALT:     snprintf(last_key_label, sizeof(last_key_label), "LALT %s", down ? "dn" : "up"); break;
    case KEY_LEFT_GUI:     snprintf(last_key_label, sizeof(last_key_label), "LGUI %s", down ? "dn" : "up"); break;
    case KEY_UP_ARROW:     snprintf(last_key_label, sizeof(last_key_label), "UP %s", down ? "dn" : "up"); break;
    case KEY_DOWN_ARROW:   snprintf(last_key_label, sizeof(last_key_label), "DOWN %s", down ? "dn" : "up"); break;
    case KEY_LEFT_ARROW:   snprintf(last_key_label, sizeof(last_key_label), "LEFT %s", down ? "dn" : "up"); break;
    case KEY_RIGHT_ARROW:  snprintf(last_key_label, sizeof(last_key_label), "RIGHT %s", down ? "dn" : "up"); break;
    default:               snprintf(last_key_label, sizeof(last_key_label), "KC%u %s", code, down ? "dn" : "up"); break;
  }
}

static void pressKey(uint8_t code) {
  if (code == 0) return;
  Keyboard.press(code);
  total_presses++;
  setLastKeyLabel(code, true);
}

static void releaseKey(uint8_t code) {
  if (code == 0) return;
  Keyboard.release(code);
  setLastKeyLabel(code, false);
}

static void handleKeyChange(uint8_t row, uint8_t col, bool pressed) {
  const uint8_t code = active_keymap()[row][col];
  if (pressed) pressKey(code);
  else releaseKey(code);
}

static uint16_t countActiveKeys() {
  uint16_t count = 0;
  for (uint8_t r = 0; r < ROWS; ++r) {
    for (uint8_t c = 0; c < COLS; ++c) {
      if (keys[r][c].stable) ++count;
    }
  }
  return count;
}

static void initMatrix() {
  for (uint8_t r = 0; r < ROWS; ++r) {
    pinMode(ROW_PINS[r], OUTPUT);
    digitalWrite(ROW_PINS[r], HIGH);
  }
  for (uint8_t c = 0; c < COLS; ++c) {
    pinMode(COL_PINS[c], INPUT_PULLUP);
  }
}

static void initEncoder() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  enc_prev = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
}

static void initOLED() {
  Wire.setSDA(OLED_SDA);
  Wire.setSCL(OLED_SCL);
  Wire.begin();

  oled_ok = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  if (!oled_ok) return;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Split keyboard");
  display.setCursor(0, 12);
  display.print(side_name());
  display.display();
}

static void drawOLED() {
  if (!oled_ok) return;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Split keyboard");
  display.setCursor(0, 10);
  display.print("Side: ");
  display.print(side_name());

  display.setCursor(0, 20);
  display.print("Active: ");
  display.print(active_keys);

  display.setCursor(0, 30);
  display.print("Enc: ");
  display.print(encoder_detents);

  display.setCursor(0, 40);
  display.print("Keys: ");
  display.print(total_presses);

  display.setCursor(0, 52);
  display.print(last_key_label);

  // tiny animation
  uint8_t x = (millis() / 18) % 104;
  display.drawRect(x, 56, 24, 6, SSD1306_WHITE);
  display.fillRect(x + 2, 58, 10, 2, SSD1306_WHITE);

  display.display();
}

static void scanEncoder() {
  // Simple quadrature decode with detent accumulation.
  static const int8_t lut[16] = {
    0, -1,  1,  0,
    1,  0,  0, -1,
   -1,  0,  0,  1,
    0,  1, -1,  0
  };

  const uint8_t curr = (digitalRead(ENC_A) << 1) | digitalRead(ENC_B);
  const uint8_t idx = (enc_prev << 2) | curr;
  const int8_t delta = lut[idx];
  enc_prev = curr;

  if (delta == 0) return;

  encoder_steps += delta;

  // four transitions per detent on most EC11-style encoders
  if (encoder_steps >= 4) {
    encoder_steps = 0;
    encoder_detents++;
    Keyboard.write(KEY_PAGE_UP);
    snprintf(last_key_label, sizeof(last_key_label), "ENC cw");
  } else if (encoder_steps <= -4) {
    encoder_steps = 0;
    encoder_detents--;
    Keyboard.write(KEY_PAGE_DOWN);
    snprintf(last_key_label, sizeof(last_key_label), "ENC ccw");
  }
}

static void scanMatrix() {
  const uint32_t now = millis();

  for (uint8_t r = 0; r < ROWS; ++r) {
    digitalWrite(ROW_PINS[r], LOW);
    delayMicroseconds(5);

    for (uint8_t c = 0; c < COLS; ++c) {
      const bool raw_pressed = (digitalRead(COL_PINS[c]) == LOW);

      DebouncedKey &k = keys[r][c];
      if (raw_pressed != k.raw) {
        k.raw = raw_pressed;
        k.changed_at = now;
      }

      if ((now - k.changed_at) >= DEBOUNCE_MS && k.stable != k.raw) {
        k.stable = k.raw;
        handleKeyChange(r, c, k.stable);
      }
    }

    digitalWrite(ROW_PINS[r], HIGH);
  }

  active_keys = countActiveKeys();
}

void setup() {
  delay(300);
  Keyboard.begin();

  initMatrix();
  initEncoder();
  initOLED();

  last_scan = millis();
}

void loop() {
  scanMatrix();
  scanEncoder();

  if (millis() - last_oled >= 80) {
    last_oled = millis();
    drawOLED();
  }

  // If you want to reduce CPU use a little:
  delay(1);
}
