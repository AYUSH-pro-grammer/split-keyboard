
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static constexpr uint8_t kSDApin = 27;
static constexpr uint8_t kSCLpin = 26;
static constexpr uint8_t kI2Caddr = 0x3C;

static constexpr int kW = 128;
static constexpr int kH = 64;

Adafruit_SSD1306 scr(kW, kH, &Wire, -1);

void setup() {
  Wire.setSDA(kSDApin);
  Wire.setSCL(kSCLpin);
  Wire.begin();

  scr.begin(SSD1306_SWITCHCAPVCC, kI2Caddr);

  scr.clearDisplay();
  scr.setTextColor(SSD1306_WHITE);
  scr.setTextSize(1);
  scr.setCursor(0, 0);

  scr.println("OLED test");
  scr.println("Split keyboard");
  scr.println("GP26 SCL");
  scr.println("GP27 SDA");
  scr.display();

  delay(1200);
}

void loop() {
  static int wPos = 0;
  static int step = 1;
  static uint32_t tPrev = 0;

  if (millis() - tPrev < 25) return;
  tPrev = millis();

  scr.clearDisplay();
  scr.setTextSize(1);
  scr.setCursor(0, 0);

  scr.println("OLED test");
  scr.println("Connection OK");

  scr.drawRect(wPos, 28, 20, 10, SSD1306_WHITE);
  scr.fillCircle((wPos + 10) % 128, 52, 4, SSD1306_WHITE);

  scr.setCursor(0, 56);
  scr.print("GP26/27");

  scr.display();

  wPos += step;
  if (wPos <= 0 || wPos >= 108) step = -step;
}
