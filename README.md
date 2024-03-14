ESPHome TaiXia Custom Component


## Special Thank
  ### Thanks for Andrew Wang provide the related hardware and spec.
  ### Thanks for Seven Hong provide Air Conditioner for test.
  ### Thanks for Mr. Wang and Mr Huang donate $230 and $200.

## Hardware

### UART Port

Hitachi JST PAP-04-V
|Pin|Meaning|
|---|-------|
|1|5V|
|2|GND|
|3|Rx to aircon or Fan - 5V logic|
|4|Tx from aircon or Fan- 5V logic|

Panasonic 
|Pin|Meaning|
|---|-------|
|1||
|2||
|3||
|4||


## Installation
Set wifi_ssid and wifi_password in your esphome's secrets.yaml first

1. Place the folder 'taixia' into the components of your esphome configuration folder
2. Create new device with the yaml in this repository
3. Or you can check the example "climate-taiseia.yaml" or "fan-taiseia.yaml"


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
The climate example exported to Home Assistant
<img src="https://github.com/tsunglung/taixia/raw/master/pictures/climate.png">
