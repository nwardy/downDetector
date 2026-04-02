# Bill of Materials — Down Detector

ESP32-based website uptime monitor — triggers an ultrasonic mist maker via relay when the site goes down.
Source everything from Amazon.

| Qty | Component | Notes |
|-----|-----------|-------|
| 1 | ESP32 Dev Board | DOIT ESP32 DevKit v1 or any 38-pin ESP32 works |
| 1 | 1-Channel 5V Relay Module | Active-LOW, optocoupler isolated — common on Amazon for ~$5 |
| 1 | Ultrasonic Mist Maker (5V / USB) | The thing that runs when the site is down — plug the USB power into the relay-switched side |
| 1 | 5V Power Supply (phone charger) | Powers the ESP32 + relay; a basic USB wall charger works |
| 1 | Micro USB cable | To flash and power the ESP32 |
| 1 | Small plastic liner | Water container for the mist maker — any small tray or food container |
| — | Jumper wires | M-to-M and M-to-F, whatever you have around |
| — | PLA filament | For printing the enclosure from `hardware/enclosure.scad` |

---

## Wiring

| ESP32 Pin | Goes To |
|-----------|---------|
| GPIO 26 | Relay IN |
| 5V / VIN | Relay VCC |
| GND | Relay GND |
| GPIO 2 | Onboard LED (built in, no wiring needed) |

The relay's NO (normally open) contact goes inline with the mist maker's 5V power line.
When the site is down → relay closes → mist maker turns on.

---

## Heads up

The mist maker needs water in the liner to actually work — don't run it dry for too long.
Keep the ESP32 and relay away from the water/mist.
