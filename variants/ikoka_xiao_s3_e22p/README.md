# Ikoka XIAO ESP32-S3 + E22P-915M30S

This variant targets the minimized solar repeater PCB using the XIAO ESP32-S3,
Ebyte E22P-915M30S, and INA3221.

## Build

```sh
pio run -e Ikoka_Xiao_S3_E22P_repeater
```

## Radio and sensor wiring

| Function | XIAO signal / GPIO | E22P pin |
| --- | --- | --- |
| DIO1 | D1 / GPIO2 | 13 |
| Reset | D2 / GPIO3 | 15 |
| Busy | D3 / GPIO4 | 14 |
| NSS | D4 / GPIO5 | 19 |
| RF EN | D5 / GPIO6 | 6 |
| SDA | D6 / GPIO43 | INA3221 SDA |
| SCL | D7 / GPIO44 | INA3221 SCL |
| SCK | D8 / GPIO7 | 18 |
| MISO | D9 / GPIO8 | 16 |
| MOSI | D10 / GPIO9 | 17 |
| DIO2 / T/R CTRL | E22P internal connection | 8 to 7 |

For the E22P-915M30S, pin 6 `EN` is held high for both TX and RX. DIO2 drives
the module's T/R CTRL input (the PCB ties E22P pins 7 and 8 together), so the
SX1262 DIO2 RF-switch function is enabled and the ordinary SX1262 RXEN pin
control is disabled for this variant. TX power is capped at the project's
19 dBm setting for the 1 W module.

The INA3221 is configured at I2C address `0x42` with 0.1 Ω shunts.
