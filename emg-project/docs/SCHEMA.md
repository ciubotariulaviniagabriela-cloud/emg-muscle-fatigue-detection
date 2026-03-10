# Schema Electrică

Schema electrică a fost realizată în Fritzing și este prezentată în lucrarea de diplomă (Cap. 2.3).

## Conexiuni principale

| Pin ESP8266 | Conectat la |
|---|---|
| A0 | Ieșire modul EMG |
| VIN | Baterie 9V (+) |
| GND | Masă comună |
| Pin 12 (SCL) | OLED SCL (I2C software) |
| Pin 14 (SDA) | OLED SDA (I2C software) |

## Alimentare simetrică

| Baterie | Rol |
|---|---|
| Baterie 1 + 2 în serie | +9V → Modul EMG (V+) |
| Baterie 3 + 4 în serie | −9V → Modul EMG (V−) |
| Baterie separată | +9V → ESP8266 (VIN) |
| Joncțiunea dintre serii | Masă comună (GND) |

> Adaugă imaginea schemei din Fritzing ca `schema_electrica.png` în acest folder.
