# Real-Time Muscle Fatigue Detection using EMG Sensor

**Diploma Thesis — Universitatea Tehnică „Gheorghe Asachi" din Iași**  
**Faculty of Electronics, Telecommunications and Information Technology**  
**Specialization: Applied Electronics | 2026**  
**Author: Ciubotariu Lavinia-Gabriela**  
**Scientific Coordinator: Șl.dr.ing. Cojocaru Victor**

---![Uploading IMG_0012.JPG…]()


## Overview

A portable electromiography (EMG) system that detects muscle fatigue in real time. The system uses an EMG sensor with a T084 operational amplifier, an ESP8266 microcontroller, and an OLED display. Data is visualized locally on the OLED and through a web interface accessible via Wi-Fi.

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | ESP8266 NodeMCU (HW-264A with integrated OLED) |
| EMG Sensor | Module with T084 operational amplifier |
| Display | OLED SSD1306 128x64px (yellow-blue) |
| Power Supply | 3x 9V batteries (symmetric ±9V + 9V for ESP) |
| Electrodes | Ag/AgCl surface electrodes with conductive gel |

---

## Technical Specifications

| Parameter | Value |
|---|---|
| Amplification | Av = 1001 (Rg = 30Ω, R1 = R2 = Rf = 15kΩ) |
| CMRR | ~100 dB (1% resistors) |
| Frequency Band | 16–500 Hz |
| ADC Resolution | 10-bit (0–1023) |
| OLED Sampling Rate | 20 Hz |
| WebSocket Transmission | 1 Hz |
| Total Cost | under 100 RON |

---

## Features

- Real-time EMG signal graph on OLED display
- Automatic muscle fatigue detection (ADC threshold < 600)
- Wi-Fi web server with live graph and table of last 30 records
- Independent Access Point (no external network required)
- Battery-powered portable system (~90 hours autonomy)

---

## Required Libraries (Arduino IDE)

```
ESP8266WiFi
ESP8266WebServer
WebSocketsServer
U8g2lib
```

---

## Pin Connections

| ESP8266 Pin | Component |
|---|---|
| A0 | EMG module signal output |
| D1 (GPIO 12) | OLED SCL |
| D2 (GPIO 14) | OLED SDA |
| VIN | +9V battery |
| GND | Common ground |

---

## Usage

1. Upload `src/EMG_Monitor_FINAL.ino` to the ESP8266 via Arduino IDE
2. Connect to Wi-Fi network: **EMG_Monitor** / password: **12345678**
3. Open browser at **192.168.4.1**
4. Attach electrodes to the target muscle and monitor in real time

---

## Scientific Background

The fatigue detection threshold (ADC < 600) is aligned with findings from:

- *Journal of Electromyography and Kinesiology* (2008) — EMG amplitude correlates with motor unit recruitment and fatigue onset during sustained isometric contractions
- *European Journal of Applied Physiology* (2015) — Average amplitude increases 30–50% above baseline during effort at 50–80% MVC
- *Sports Medicine* (2017) — Integrated EMG amplitude (iEMG) is a robust, non-invasive indicator for fatigue monitoring in ergonomic and sports applications

---

## Future Development

- Replace ESP8266 with ESP32 for 12-bit ADC and parallel processing
- External 16-bit ADC for higher signal precision
- FFT spectral analysis and RMS calculation
- Dedicated mobile application
- Machine learning integration for muscle contraction pattern recognition
- PCB layout optimization and EMI shielding

---

## Project Structure

```
/
├── src/
│   └── EMG_Monitor_FINAL.ino
├── docs/
│   └── SCHEMA.md
└── README.md
```

---

## License

MIT License — free for educational and research use.
