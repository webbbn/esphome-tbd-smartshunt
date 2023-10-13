
# esphome-tbd-smartshunt
ESPHome component that monitors a TBD Smartshunt via UART.

The TBD Smartshunt outputs a 38 byte message at around 1Hz, which at least contains the voltage and current readings. There appears to be a third value that might be the temperature reading, but that has not been verified. There is also a constant header that might be some sort of device ID, and what might be a checksum at the end. Each line is preceeded by a ':', and each line is a 37 byte hex string encoded as ASCII.

## Schematics

```
# UART-TTL jack (JST-PH 2.0mm pinch)
┌────────────────┐
│                │
│ O   O   O   O  │
│2.8V TX     GND │
└─── ─────── ────┘
```

## Installation

An example YAML file is provide for an ESP32. The provided configuration includes additional virtual sensor that track features such as total energy in, total energy out, state of charge, etc. These are stored in persistant storage, so should survive restart, and should be usable for tracking battery state.
Example:
