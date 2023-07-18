ESPHome TaiXia Custom Component


## Hardware

### Daikin S21 Port

**NOTE:** The Daikin S21 port provides >5V, so if you intend to power your
board on this pin, be sure to test its output and regulate voltage accordingly.

On my Daikin units, the S21 port has the following pins:

|Pin|Meaning|
|---|-------|
|1|5V|
|2|Tx from aircon - 5V logic|
|3|Rx to aircon - 5V logic but most models accept 3.3V|
|4|12V, or 14V, or some such|
|5|GND|

The S21 plug is JST `EHR-5` and related header `B5B-EH-A(LF)(SN)`, though the
plug pins are at standard pin header widths.

## Installation
Set wifi_ssid and wifi_password in your esphome's secrets.yaml first

1. Place the folder 'taixia' into the custom_components of your esphome configuration folder
2. Create new device with the yaml in this repository


## Configuration Example

```yaml
logger:
  baud_rate: 0  # Disable UART logger if using UART0 (pins 1,3)

external_components:
  - source: github://tsunglung/taixia
    components: [ taixia ]

uart:
  id: uart_taixia
  tx_pin: GPIO0
  rx_pin: GPIO1
  baud_rate: 9600
  debug:
    direction: BOTH

taixia:
#  max_length: 28   # Option to limit the max length of RX buffer

climate:
  - platform: taixia
    name: My Daikin Climate
    supported_modes:
      - COOL
      - HEAT
      - DRY
      - FAN_ONLY
    supported_fan_modes:
      - LOW
      - MEDIUM
      - HIGH
      - AUTO
    supported_presets:
      - ECO
      - BOOST
      - AWAY
      - SLEEP

switch:
  - platform: taixia
    type: airconditioner
    power:
      name: "${upper_devicename} Power Switch"

sensor:
  - platform: taixia
    type: airconditioner
    temperature_indoor:
      name: My Daikin Inside Temperature
    humidity_indoor:
      name: My Daikin Outside Temperature

number:
  - platform: taixia
    type: airconditioner
    vertical_fan_speed:
      name: "${upper_devicename} Vertical Fan Speed"
    horizontal_fan_speed:
      name: "${upper_devicename} Horizontal Fan Speed"

select:
  - platform: taixia
    type: airconditioner
    fuzzy_mode:
      name: "${upper_devicename} Fuzzy Mode"

text_sensor:
  - platform: taixia
    sa_id:
      name: "${upper_devicename} SA ID"
    brand:
      name: "${upper_devicename} SA Brand"
    model:
      name: "${upper_devicename} SA Model"
    version:
      name: "${upper_devicename} SA Version"
    services:
      name: "${upper_devicename} SA Services"

# Optional additional component.


```