#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

#include <vector>

namespace esphome {
namespace taixia {

#define WRITE (0x1 << 7)
#define SA_ID_ALL 0x00
#define SERVICE_ID_REGISTER 0x00
#define SERVICE_ID_READ_DEVICE_TYPE 0x01
#define SERVICE_ID_READ_VERSION 0x02
#define SERVICE_ID_REVERSED 0x03
#define SERVICE_ID_READ_SA_ID 0x04
#define SERVICE_ID_READ_BRAND 0x05
#define SERVICE_ID_READ_MODEL 0x06
#define SERVICE_ID_READ_SERVICES 0x07
#define SERVICE_ID_READ_STATUS 0x08

#define SA_ID_CLIMATE 0x01
#define SERVICE_ID_CLIMATE_STATUS 0x00
#define SERVICE_ID_CLIMATE_MODE 0x01
#define SERVICE_ID_CLIMATE_FAN_SPEED 0x02
#define SERVICE_ID_CLIMATE_TARGET_TEMPERATURE 0x03
#define SERVICE_ID_CLIMATE_TEMPERATURE_INDOOR 0x04
#define SERVICE_ID_CLIMATE_SLEEP 0x05
#define SERVICE_ID_CLIMATE_SLEEP_TIMER 0x06
#define SERVICE_ID_CLIMATE_FUZZY_MODE 0x07
#define SERVICE_ID_CLIMATE_AIR_PURIFIER 0x08
#define SERVICE_ID_CLIMATE_ON_TIME 0x09
#define SERVICE_ID_CLIMATE_OFF_TIME 0x0A
#define SERVICE_ID_CLIMATE_ON_TIMER 0x0B
#define SERVICE_ID_CLIMATE_OFF_TIMER 0x0C
#define SERVICE_ID_CLIMATE_SYSTEM_TIME 0x0D
#define SERVICE_ID_CLIMATE_SWING_VERTICAL 0x0E
#define SERVICE_ID_CLIMATE_SWING_VERTICAL_LEVEL 0x0F
#define SERVICE_ID_CLIMATE_SWING_HORIZONTAL 0x10
#define SERVICE_ID_CLIMATE_SWING_HORIZONTAL_LEVEL 0x11
#define SERVICE_ID_CLIMATE_FILITER_NOTIFY 0x12
#define SERVICE_ID_CLIMATE_HUMIDITY 0x13
#define SERVICE_ID_CLIMATE_HUMIDITY_INDOOR 0x14
#define SERVICE_ID_CLIMATE_CHECK 0x15
#define SERVICE_ID_CLIMATE_AIR_QUALITY_DETECT 0x16
#define SERVICE_ID_CLIMATE_MILDEW_PROOF 0x17
#define SERVICE_ID_CLIMATE_SELF_CLEANING 0x18
#define SERVICE_ID_CLIMATE_ACTIVITY 0x19
#define SERVICE_ID_CLIMATE_BOOST 0x1A
#define SERVICE_ID_CLIMATE_ECO 0x1B
#define SERVICE_ID_CLIMATE_COMFORT 0x1C
#define SERVICE_ID_CLIMATE_FEATURE_PROHIBIT 0x1D
#define SERVICE_ID_CLIMATE_BEEPER 0x1E
#define SERVICE_ID_CLIMATE_DISPLAY_MODE 0x1F
#define SERVICE_ID_CLIMATE_MOISTURIZE 0x20
#define SERVICE_ID_CLIMATE_TEMPERATURE_OUTDOOR 0x21
#define SERVICE_ID_CLIMATE_HUMIDITY_OUTDOOR 0xFF
#define SERVICE_ID_CLIMATE_OPERATING_CURRENT 0x24
#define SERVICE_ID_CLIMATE_OPERATING_VOLTAGE 0x25
#define SERVICE_ID_CLIMATE_FACTOR 0x26
#define SERVICE_ID_CLIMATE_OPERATING_WATT 0x27
#define SERVICE_ID_CLIMATE_ENERGY_CONSUMPTION 0x28
#define SERVICE_ID_CLIMATE_ERROR_CODE 0x29
#define SERVICE_ID_CLIMATE_OPERATING_HOURS 0x2F
#define SERVICE_ID_CLIMATE_FILTER_CLEAN_HOURS 0x30
#define SERVICE_ID_CLIMATE_SWITCH_39 0x39
#define SERVICE_ID_CLIMATE_3A 0x3A
#define SERVICE_ID_CLIMATE_3B 0x3B

#define SA_ID_WASHER 0x03
#define SERVICE_ID_WASHER_STATUS 0x00
#define SERVICE_ID_WASHER_MODE 0x01
#define SERVICE_ID_WASHER_WASH_PROGRAM 0x02
#define SERVICE_ID_WASHER_CURRENT_STATUS 0x03
#define SERVICE_ID_WASHER_WASH_LEFT_COUNT 0x06
#define SERVICE_ID_WASHER_WASH_LEFT_HOURS 0x08
#define SERVICE_ID_WASHER_SPIN_LEFT_HOURS 0x0A
#define SERVICE_ID_WASHER_SOAK_LEFT_HOURS 0x0C
#define SERVICE_ID_WASHER_DRY_LEFT_HOURS 0x0E
#define SERVICE_ID_WASHER_TANK_CLEAN_LEFT_HOURS 0x10
#define SERVICE_ID_WASHER_DRY_CLEAN_LEFT_HOURS 0x12
#define SERVICE_ID_WASHER_TOTAL_LEFT_HOURS 0x13
#define SERVICE_ID_WASHER_APPOINT_LEFT_HOURS 0x15
#define SERVICE_ID_WASHER_OTHER_FUNCTION 0x16
#define SERVICE_ID_WASHER_WASH_MODE 0x17
#define SERVICE_ID_WASHER_WARM_WATER_PROGRAM 0x18
#define SERVICE_ID_WASHER_OPERATING_CURRENT 0x1A
#define SERVICE_ID_WASHER_OPERATING_VOLTAGE 0x1B
#define SERVICE_ID_WASHER_FACTOR 0x1C
#define SERVICE_ID_WASHER_OPERATING_WATT 0x1D
#define SERVICE_ID_WASHER_ENERGY_CONSUMPTION 0x1E

#define SA_ID_DEHUMIDIFIER 0x04
#define SERVICE_ID_DEHUMIDTFIER_STATUS 0x00
#define SERVICE_ID_DEHUMIDTFIER_MODE 0x01
#define SERVICE_ID_DEHUMIDTFIER_OPERATING_TIME 0x02
#define SERVICE_ID_DEHUMIDTFIER_RELATIVE_HUMIDITY 0x03
#define SERVICE_ID_DEHUMIDTFIER_DEHUMIDIFY_LEVEL 0x04
#define SERVICE_ID_DEHUMIDTFIER_DRY_LEVEL 0x05
#define SERVICE_ID_DEHUMIDTFIER_TEMPERATURE_INDOOR 0x06
#define SERVICE_ID_DEHUMIDTFIER_HUMIDITY_INDOOR 0x07
#define SERVICE_ID_DEHUMIDTFIER_AIR_FLOW_AUTO 0x08
#define SERVICE_ID_DEHUMIDTFIER_FAN_ANGLE 0x09
#define SERVICE_ID_DEHUMIDTFIER_WATER_TANK_FULL 0x0A
#define SERVICE_ID_DEHUMIDTFIER_FILTER_NOTIFY 0x0B
#define SERVICE_ID_DEHUMIDTFIER_LIGHT 0x0C
#define SERVICE_ID_DEHUMIDTFIER_AIR_PURFIFIER 0x0D
#define SERVICE_ID_DEHUMIDTFIER_FAN_LEVEL 0x0E
#define SERVICE_ID_DEHUMIDTFIER_SIDE_AIR_VENT 0x0F
#define SERVICE_ID_DEHUMIDTFIER_SOUND_MODE 0x10
#define SERVICE_ID_DEHUMIDTFIER_DEFROST 0x11
#define SERVICE_ID_DEHUMIDTFIER_ERROR_CODE 0x12
#define SERVICE_ID_DEHUMIDTFIER_MILDEW_PROOF 0x13
#define SERVICE_ID_DEHUMIDTFIER_HIGH_HUMIDITY_NOTIFY 0x14
#define SERVICE_ID_DEHUMIDTFIER_HIGH_HUMIDITY_LEVEL 0x15
#define SERVICE_ID_DEHUMIDTFIER_LOCK 0x16
#define SERVICE_ID_DEHUMIDTFIER_FUNCTIONS_SETTING 0x17
#define SERVICE_ID_DEHUMIDTFIER_BEEPER 0x18
#define SERVICE_ID_DEHUMIDTFIER_OPERATING_CURRENT 0x19
#define SERVICE_ID_DEHUMIDTFIER_OPERATING_VOLTAGE 0x1A
#define SERVICE_ID_DEHUMIDTFIER_FACTOR 0x1B
#define SERVICE_ID_DEHUMIDTFIER_OPERATING_WATT 0x1C
#define SERVICE_ID_DEHUMIDTFIER_ENERGY_CONSUMPTION 0x1D
#define SERVICE_ID_DEHUMIDTFIER_PM_2_5 0x25
#define SERVICE_ID_DEHUMIDTFIER_LIGHT_LEVEL 0x27
#define SERVICE_ID_DEHUMIDTFIER_ODOURS 0x28
#define SERVICE_ID_DEHUMIDTFIER_PM_2_5_DETECT 0x29
#define SERVICE_ID_DEHUMIDTFIER_FILTER_RESET 0x52

#define SA_ID_AIR_PURIFIER 0x08
#define SERVICE_ID_PURIFIER_STATUS 0x00
#define SERVICE_ID_PURIFIER_MODE 0x01
#define SERVICE_ID_PURIFIER_AIR_QUALITY 0x04
#define SERVICE_ID_PURIFIER_OPERATING_CURRENT 0x0A
#define SERVICE_ID_PURIFIER_OPERATING_VOLTAGE 0x0B
#define SERVICE_ID_PURIFIER_FACTOR 0x0C
#define SERVICE_ID_PURIFIER_OPERATING_WATT 0x0D
#define SERVICE_ID_PURIFIER_ENERGY_CONSUMPTION 0x0E

#define SA_ID_ERV 0x0E
#define SERVICE_ID_ERV_STATUS 0x00
#define SERVICE_ID_ERV_MODE 0x01
#define SERVICE_ID_ERV_FAN_SPEED 0x02
#define SERVICE_ID_ERV_TARGET_TEMPERATURE 0x03
#define SERVICE_ID_ERV_TEMPERATURE_INDOOR 0x04
#define SERVICE_ID_ERV_TEMPERATURE_OUTDOOR 0x05
#define SERVICE_ID_ERV_TIMER_ON 0x06
#define SERVICE_ID_ERV_CHECK_CODE 0x07
#define SERVICE_ID_ERV_MODEL_TYPE 0x08
#define SERVICE_ID_ERV_ERROR_CODE 0x09
#define SERVICE_ID_ERV_ENERGY 0x0E
#define SERVICE_ID_ERV_RESET_FILTER_NOTIFY 0x14
#define SERVICE_ID_ERV_VENTILATE_MODE 0x15
#define SERVICE_ID_ERV_PRE_HEAT_COOL 0x16

#define SA_ID_FAN 0x0F
#define SERVICE_ID_FAN_STATUS 0x00
#define SERVICE_ID_FAN_MODE 0x01
#define SERVICE_ID_FAN_SPEED 0x02
#define SERVICE_ID_FAN_TEMPERATURE 0x03
#define SERVICE_ID_FAN_OSCILLATE 0x05
#define SERVICE_ID_FAN_ION 0x06
#define SERVICE_ID_FAN_LIGHT 0x07
#define SERVICE_ID_FAN_HUMIDITY 0x08
#define SERVICE_ID_FAN_OPERATING_CURRENT 0x0E
#define SERVICE_ID_FAN_OPERATING_VOLTAGE 0x0F
#define SERVICE_ID_FAN_FACTOR 0x10
#define SERVICE_ID_FAN_OPERATING_WATT 0x11
#define SERVICE_ID_FAN_ENERGY_CONSUMPTION 0x12

#define TAIXIA_VALUED_ENTITY_(type, name, value_type) \
 protected: \
  value_type value_##name##_; \
  TAIXIA_ENTITY_(type, name)

#define TAIXIA_ENTITY_(type, name) \
 protected: \
  type *name##_{}; /* NOLINT */ \
\
 public: \
  void set_##name(type *name) { /* NOLINT */ \
    this->name##_ = name; \
  }

#define TAIXIA_BINARY_SENSOR(name) TAIXIA_ENTITY_(binary_sensor::BinarySensor, name)
#define TAIXIA_BUTTON(name) TAIXIA_ENTITY_(button::Button, name)
#define TAIXIA_NUMBER(name) TAIXIA_ENTITY_(number::Number, name)
#define TAIXIA_SWITCH(name) TAIXIA_ENTITY_(switch_::Switch, name)
#define TAIXIA_TEXT_SENSOR(name) TAIXIA_ENTITY_(text_sensor::TextSensor, name)

using response_parser_t = std::function<float(std::vector<uint8_t> &)>;

class TaiXia;

class TaiXiaListener {
 public:
  void set_sa_id(uint16_t sa_id) { this->sa_id_ = sa_id; }

  void on_response(uint16_t sa_id, std::vector<uint8_t> &response);

 protected:
  uint16_t sa_id_{0};

  virtual void handle_response(std::vector<uint8_t> &response) = 0;
};

class TaiXia : public uart::UARTDevice, public Component {
 public:
  void dump_config() override;
  void loop() override;
  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_sa_id(uint8_t id) { this->sa_id_ = id; }
  void set_version(float version) { this->version_ = version; }
  void set_max_length(uint16_t len) { this->max_length_ = len; }
  void set_response_time(uint16_t time) { this->response_time_ = time; }
  void register_listener(TaiXiaListener *listener) { this->listeners_.push_back(listener); }

  uint8_t checksum(const uint8_t *data, uint8_t len);
  void readline(bool handle_response);
  bool send(uint8_t packet_length, uint8_t date_type, uint8_t sa_id, uint8_t service_id, uint16_t data);
  bool send_cmd(const uint8_t *command, uint8_t *response, uint8_t len) {
    if (this->version_ < 3.0)
      return write_command_(command, response, len, len, 60000);
    else
      return write_command_(command, response, len, len);
  }
  void switch_command(uint8_t sa_id, uint8_t service_id, bool onoff);
  void set_number(uint8_t sa_id, uint8_t service_id, float value);
  void get_number(uint8_t sa_id, uint8_t service_id, uint8_t *buffer);
  void button_command(uint8_t sa_id, uint8_t service_id);
  bool have_sensors() { return this->have_sensors_; }
  void set_have_sensors(bool have_sensors) { this->have_sensors_ = have_sensors; }
  float get_version() { return this->version_; }
  bool read_sa_status();

  void power_switch(bool state) { this->power_switch_->publish_state(state); }

  // TaiXIA
  TAIXIA_BINARY_SENSOR(power_binary_sensor)
  TAIXIA_BUTTON(get_info_button)
  TAIXIA_TEXT_SENSOR(sa_id_textsensor)
  TAIXIA_TEXT_SENSOR(brand_textsensor)
  TAIXIA_TEXT_SENSOR(model_textsensor)
  TAIXIA_TEXT_SENSOR(version_textsensor)
  TAIXIA_TEXT_SENSOR(services_textsensor)
  TAIXIA_SWITCH(power_switch)

  // Climate 0x01
  TAIXIA_SWITCH(sleepy_switch)
  TAIXIA_SWITCH(air_purifier_switch)
  TAIXIA_SWITCH(swing_vertical_switch)
  TAIXIA_SWITCH(swing_horizontal_switch)
  TAIXIA_SWITCH(filter_notify_switch)
  TAIXIA_SWITCH(air_detect_switch)
  TAIXIA_SWITCH(mildew_proof_switch)
  TAIXIA_SWITCH(super_mode_switch)
  TAIXIA_SWITCH(power_saving_switch)
  TAIXIA_SWITCH(beeper_switch)
  TAIXIA_SWITCH(moisturize_switch)
  TAIXIA_SWITCH(frost_wash_switch)
  TAIXIA_SWITCH(self_cleaning_switch)
  TAIXIA_NUMBER(swing_vertical_level_number)
  TAIXIA_NUMBER(swing_horizontal_level_number)
  TAIXIA_NUMBER(sleep_timer_number)

  // Dehumiditier 0x04
  TAIXIA_BINARY_SENSOR(water_tank_full_binary_sensor)
  TAIXIA_BINARY_SENSOR(filter_notify_binary_sensor)
  TAIXIA_BINARY_SENSOR(side_air_flow_binary_sensor)
  TAIXIA_BINARY_SENSOR(defrost_binary_sensor)
  TAIXIA_BINARY_SENSOR(odours_binary_sensor)
  TAIXIA_SWITCH(air_flow_auto_switch)
  TAIXIA_SWITCH(pm25_detect_switch)
  TAIXIA_SWITCH(filter_reset_switch)
  TAIXIA_NUMBER(operating_time_number)
  TAIXIA_NUMBER(relative_humidity_number)
  TAIXIA_NUMBER(dehumidifier_level_number)
  TAIXIA_NUMBER(dry_level_number)
  TAIXIA_NUMBER(fan_angle_number)
  TAIXIA_NUMBER(air_purifier_number)
  TAIXIA_NUMBER(fan_level_number)
  TAIXIA_NUMBER(sound_mode_number)
  TAIXIA_NUMBER(high_humidity_level_number)
  TAIXIA_NUMBER(light_level_number)
/*
  TAIXIA_SWITCH(filter_notify_switch)
  TAIXIA_SWITCH(light_switch)
  TAIXIA_SWITCH(beeper_switch)
  TAIXIA_SWITCH(mildew_proof_switch)
*/
  TAIXIA_SWITCH(humidity_notify_switch)
  TAIXIA_SWITCH(lock_switch)
  TAIXIA_SWITCH(saa_notify_switch)
  TAIXIA_SWITCH(pm25_switch)

  // Fan 0x0F
  TAIXIA_SWITCH(ions_switch)
  TAIXIA_SWITCH(light_switch)
  TAIXIA_NUMBER(off_timer_number)
  TAIXIA_NUMBER(on_timer_number)

 protected:
  void readline_(int readch, char *buffer, int len);
  void get_info_(void);
  bool read_climate_status_(void);

  std::vector<uint8_t> buffer_;
  uint8_t protocol_;
  uint8_t sa_id_;
  float version_{4.0};
  uint8_t len_;
  uint16_t max_length_{0};
  uint32_t response_time_{1};
  bool have_sensors_{false};

  std::vector<TaiXiaListener *> listeners_{};

  bool write_command_(const uint8_t *command, uint8_t *response, uint8_t len, uint8_t tlen, uint32_t timeout);
  bool write_command_(const uint8_t *command, uint8_t *response, uint8_t len, uint8_t tlen);
};

}  // namespace taixia
}  // namespace esphome
