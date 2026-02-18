# smart-parcel-monitoring
Smart parcel tampering detection system with real-time sensor monitoring, anomaly detection, and web dashboard visualization.

![Block Diagram](block-diagram.png)

## How It Works

1. LDR monitors ambient light conditions inside the parcel.
2. Vibration sensor detects physical disturbance.
3. ESP32 processes sensor inputs using threshold-based logic.
4. If tampering is detected, event is logged with timestamp.
5. OLED displays real-time parcel status.


## Features

- Real-time sensor data monitoring
- Tamper detection using LDR and vibration sensor
- Timestamp logging using DS3231 RTC
- Circular buffer log storage
- OLED-based real-time status display

## Tech Stack

- ESP32
- Embedded C
- DS3231 RTC
- LDR Sensor
- Vibration Sensor
- OLED Display (SSD1306)
## System Architecture



