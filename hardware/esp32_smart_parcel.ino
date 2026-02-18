/*
  Smart Parcel Tampering Detection System
  Author: Rohith Inturi

  Description:
  ESP32-based real-time parcel monitoring system using:
  - LDR sensor (ambient light detection)
  - Vibration sensor (physical disturbance detection)
  - RTC (DS3231) for timestamp logging
  - OLED display for real-time status visualization

  Logs tamper events and displays system status.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

// ====== OLED Setup ======
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ====== RTC Setup ======
RTC_DS3231 rtc;
bool rtcFound = false;

// ====== Sensor Pins ======
#define LDR_PIN 34
#define VIB_PIN 27

// ====== Thresholds ======
#define LDR_THRESHOLD 4000

// ====== Tamper Log ======
#define MAX_LOGS 5
String tamperLogs[MAX_LOGS];
int logIndex = 0;

// ====== Tamper State Tracking ======
bool lastTamperState = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Serial.println("Starting Smart Parcel System...");

  pinMode(VIB_PIN, INPUT);

  // OLED Initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Parcel Init...");
  display.display();
  delay(1500);

  // RTC Initialization
  if (!rtc.begin()) {
    rtcFound = false;
    Serial.println("RTC not detected, using ESP32 millis()");
  } else {
    rtcFound = true;
    if (rtc.lostPower()) {
      Serial.println("RTC lost power, setting compile time...");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
}

void loop() {

  // ====== Sensor Readings ======
  int ldrValue = analogRead(LDR_PIN);
  int vibState = digitalRead(VIB_PIN);

  // ====== Time Handling ======
  String timeStr;

  if (rtcFound) {
    DateTime now = rtc.now();
    char buf[20];
    sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    timeStr = String(buf);
  } else {
    unsigned long sec = millis() / 1000;
    unsigned long hh = (sec / 3600) % 24;
    unsigned long mm = (sec / 60) % 60;
    unsigned long ss = sec % 60;
    char buf[20];
    sprintf(buf, "%02lu:%02lu:%02lu", hh, mm, ss);
    timeStr = String(buf);
  }

  // ====== Tamper Detection Logic ======
  bool tampered = !(ldrValue >= LDR_THRESHOLD && vibState == 0);
  String tamperStatus = tampered ? "Tampered!" : "Safe";

  // Log only when state changes from Safe → Tampered
  if (tampered && !lastTamperState && rtcFound) {
    tamperLogs[logIndex] = timeStr;
    logIndex = (logIndex + 1) % MAX_LOGS;
  }

  lastTamperState = tampered;

  // ====== Serial Monitor Output ======
  Serial.print("Time: "); Serial.print(timeStr);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | VIB: "); Serial.print(vibState);
  Serial.print(" | Status: "); Serial.println(tamperStatus);

  // ====== OLED Display ======
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("Smart Parcel Status");

  display.setCursor(0, 12);
  display.print("Time: ");
  display.println(timeStr);

  display.setCursor(0, 24);
  display.print("LDR: ");
  display.println(ldrValue);

  display.setCursor(0, 36);
  display.print("VIB: ");
  display.println(vibState);

  display.setCursor(0, 48);
  display.print("Status: ");
  display.println(tamperStatus);

  display.display();

  delay(1000);
}
