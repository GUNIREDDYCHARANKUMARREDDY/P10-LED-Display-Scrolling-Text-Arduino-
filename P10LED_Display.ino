#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_Black_16_ISO_8859_1.h"   // BIG FONT

#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// ------------------ PWM Brightness ------------------
#define OE_PIN 9
volatile uint8_t brightness = 155;   // 0–255 (LOW = bright, HIGH = dim)

// ------------------ Interrupt Scan ------------------
void ScanDMD() {

  dmd.scanDisplayBySPI();

  // OE is ACTIVE LOW → invert PWM
  analogWrite(OE_PIN, 155 - brightness);
}

// ------------------ Setup ------------------
void setup(void) {

  pinMode(OE_PIN, OUTPUT);
  analogWrite(OE_PIN, 255 - brightness);

  // Refresh timing (adjust for brightness + flicker)
  Timer1.initialize(800);   // try 600–1000
  Timer1.attachInterrupt(ScanDMD);

  dmd.clearScreen(true);
  Serial.begin(115200);
}

// ------------------ Loop ------------------
void loop(void) {

  dmd.selectFont(Arial_Black_16_ISO_8859_1);  // BIG FONT
  dmd.clearScreen(true);

  char Text[] = "WELCOME TO EROTECH SOLUTIONS";

  dmd.drawMarquee(Text, strlen(Text), (32 * DISPLAYS_ACROSS) - 1, 0);

  long timer = millis();
  boolean ret = false;
  int interval = 100;

  while (!ret) {
    if (millis() - timer > interval) {

      ret = dmd.stepMarquee(-1, 0);

      // Optional bold effect (remove if overlapping)
      dmd.stepMarquee(0, 0);

      timer = millis();
    }
  }

  delay(500);
}