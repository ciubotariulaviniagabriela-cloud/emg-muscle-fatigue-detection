# 🧠 Detecția Oboselii Musculare în Timp Real cu Senzor EMG

**Lucrare de Diplomă — Universitatea Tehnică „Gheorghe Asachi" din Iași**  
**Facultatea de Electronică, Telecomunicații și Tehnologia Informației**  
**Specializarea: Electronică Aplicată | 2026**  
**Autor: Ciubotariu Lavinia-Gabriela**

---

## 📋 Descriere

Sistem portabil de electromiografie (EMG) care detectează oboseala musculară în timp real, folosind un senzor EMG cu amplificator operațional T084, un microcontroler ESP8266 și un display OLED. Datele sunt vizualizate local pe OLED și printr-o interfață web accesibilă prin Wi-Fi.

---

## ⚙️ Hardware

| Componentă | Detalii |
|---|---|
| Microcontroler | ESP8266 NodeMCU (HW-264A cu OLED integrat) |
| Senzor EMG | Modul cu amplificator operațional T084 |
| Display | OLED SSD1306 128x64px (galben-albastru) |
| Alimentare | 3× baterii 9V (alimentare simetrică ±9V + 9V pentru ESP) |
| Electrozi | Electrozi de suprafață Ag/AgCl cu gel conductor |

---

## 📐 Specificații Tehnice

- **Amplificare:** Av = 1001 (Rg = 30Ω, R1 = R2 = Rf = 15kΩ)
- **CMRR:** ~100 dB (rezistoare 1%)
- **Bandă de frecvență:** 16–500 Hz
- **Rezoluție ADC:** 10 biți (0–1023)
- **Rată eșantionare OLED:** 20 Hz
- **Cost total:** sub 100 lei

---

## 🚀 Funcționalități

- ✅ Grafic EMG în timp real pe display OLED
- ✅ Detectare automată oboseală musculară (prag ADC < 600)
- ✅ Server web Wi-Fi cu grafic live și tabel cu 30 înregistrări
- ✅ Access Point independent (nu necesită rețea externă)
- ✅ Sistem portabil alimentat pe baterii (~90 ore autonomie)

---

## 📦 Biblioteci necesare (Arduino IDE)

```
ESP8266WiFi
ESP8266WebServer
WebSocketsServer
U8g2lib
```

---

## 🔌 Conexiuni

| ESP8266 Pin | Componentă |
|---|---|
| A0 | Ieșire semnal modul EMG |
| D1 (GPIO 12) | SCL OLED |
| D2 (GPIO 14) | SDA OLED |
| VIN | +9V baterie |
| GND | Masă comună |

---

## 📡 Utilizare

1. Încarcă `src/EMG_Monitor_FINAL.ino` pe ESP8266 prin Arduino IDE
2. Conectează-te la Wi-Fi: **EMG_Monitor** / parolă: **12345678**
3. Deschide browserul la **192.168.4.1**
4. Atașează electrozii pe mușchiul dorit și monitorizează în timp real

---

## 🔮 Direcții de Dezvoltare

- Înlocuire ESP8266 cu ESP32 (ADC 12 biți)
- Algoritmi FFT și calcul RMS
- Aplicație mobilă dedicată
- Integrare Machine Learning pentru recunoașterea tiparelor musculare

---

## 📜 Licență

MIT License — liber pentru uz educațional și cercetare.
