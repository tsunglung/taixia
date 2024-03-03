#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia_sensor.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.sensor";

static inline uint16_t get_u16(std::vector<uint8_t> &response, int start) {
  return (response[start] << 8) + response[start + 1];
}

static inline int16_t get_i16(std::vector<uint8_t> &response, int start) {
  return (int16_t) ((response[start] << 8) + response[start + 1]);
}

static inline void publish_i16(std::vector<uint8_t> &response, int start, sensor::Sensor *sensor) {
  if ((response[start + 1] == 0xFF) && (response[start + 2] == 0xFF)) {
    sensor->publish_state(0.0f);
  } else {
    int16_t new_state = get_i16(response, start + 1);
    if (sensor->get_state() != new_state)
      sensor->publish_state(new_state);
  }
}

static inline void publish_u16(std::vector<uint8_t> &response, int start, sensor::Sensor *sensor) {
  if ((response[start + 1] == 0xFF) && (response[start + 2] == 0xFF)) {
    sensor->publish_state(0.0f);
  } else {
    uint16_t new_state = get_u16(response, start + 1);
    if (sensor->get_state() != new_state)
      sensor->publish_state(new_state);
  }
}

static inline void publish_u16_div_10(std::vector<uint8_t> &response, int start, sensor::Sensor *sensor) {
  if ((response[start + 1] == 0xFF) && (response[start + 2] == 0xFF)) {
    sensor->publish_state(0.0f);
  } else {
    uint16_t new_state = get_u16(response, start + 1) / 10.0f;
    if (sensor->get_state() != new_state)
      sensor->publish_state(new_state);
  }
}

void AirConditionerSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Air Conditioner:");
  if (this->temperature_indoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Temperature Indoor", this->temperature_indoor_sensor_);
  if (this->humidity_indoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Humidity Indoor", this->humidity_indoor_sensor_);
  if (this->temperature_outdoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Temperature Outdoor", this->temperature_outdoor_sensor_);
  if (this->humidity_outdoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Humidity Outdoor", this->humidity_outdoor_sensor_);
  if (this->operating_current_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Current", this->operating_current_sensor_);
  if (this->operating_voltage_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Voltage", this->operating_voltage_sensor_);
  if (this->operating_power_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Power", this->operating_power_sensor_);
  if (this->energy_consumption_sensor_ != nullptr)
    LOG_SENSOR("  ", "Energy Consumption", this->energy_consumption_sensor_);
  if (this->operating_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Hours", this->operating_hours_sensor_);
  if (this->error_code_sensor_ != nullptr)
    LOG_SENSOR("  ", "Error Code", this->error_code_sensor_);
  if (this->filiter_clean_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Filiter Clean Hours", this->filiter_clean_hours_sensor_);
  this->parent_->set_have_sensors(true);
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void AirConditionerSensor::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    if ((response[i + 1] == 0xFF) && (response[i + 2] == 0xFF)) {
      continue;
    }
    switch (response[i]) {
      case SERVICE_ID_CLIMATE_TEMPERATURE_INDOOR:
        if (this->temperature_indoor_sensor_ != nullptr) {
          publish_i16(response, i, this->temperature_indoor_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_HUMIDITY_INDOOR:
        if (this->humidity_indoor_sensor_ != nullptr) {
          publish_u16(response, i, this->humidity_indoor_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_TEMPERATURE_OUTDOOR:
        if (this->temperature_outdoor_sensor_ != nullptr) {
          publish_i16(response, i, this->temperature_outdoor_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_HUMIDITY_OUTDOOR:
        if (this->humidity_outdoor_sensor_ != nullptr) {
          publish_u16(response, i, this->humidity_outdoor_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_CURRENT:
        if (this->operating_current_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->operating_current_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_VOLTAGE:
        if (this->operating_voltage_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_voltage_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_POWER:
        if (this->operating_power_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_power_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_ENERGY:
        if (this->energy_consumption_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->energy_consumption_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_OPERATING_HOURS:
        if (this->operating_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_hours_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_ERROR_CODR:
        if (this->error_code_sensor_ != nullptr) {
          publish_u16(response, i, this->error_code_sensor_);
        }
      break;
      case SERVICE_ID_CLIMATE_FILTER_CLEAN_HOURS:
        if (this->filiter_clean_hours_sensor_ != nullptr) {
          publish_u16(response, i + 1, this->filiter_clean_hours_sensor_);
        }
      break;
    }
  }
}

void AirConditionerSensor::update() {
  this->parent_->send(6, 0, SA_ID_CLIMATE, 0x0, 0xffff);
}

void DehumidifierSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Dehumidifier:");
  if (this->temperature_indoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Temperature Indoor", this->temperature_indoor_sensor_);
  if (this->humidity_indoor_sensor_ != nullptr)
    LOG_SENSOR("  ", "Humidity Indoor", this->humidity_indoor_sensor_);
  if (this->water_full_sensor_ != nullptr)
    LOG_SENSOR("  ", "Water Full", this->water_full_sensor_);
  if (this->filiter_clean_sensor_ != nullptr)
    LOG_SENSOR("  ", "Filiter Clean", this->filiter_clean_sensor_);
  if (this->side_air_vent_sensor_ != nullptr)
    LOG_SENSOR("  ", "Side Air Vent", this->side_air_vent_sensor_);
  if (this->error_code_sensor_ != nullptr)
    LOG_SENSOR("  ", "Error Code", this->error_code_sensor_);
  if (this->operating_current_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Current", this->operating_current_sensor_);
  if (this->operating_voltage_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Voltage", this->operating_voltage_sensor_);
  if (this->operating_power_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating power", this->operating_power_sensor_);
  if (this->energy_consumption_sensor_ != nullptr)
    LOG_SENSOR("  ", "Energy Consumption", this->energy_consumption_sensor_);
  if (this->operating_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Hours", this->operating_hours_sensor_);
  this->parent_->set_have_sensors(true);
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void DehumidifierSensor::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    if ((response[i + 1] == 0xFF) && (response[i + 2] == 0xFF)) {
      continue;
    }
    switch (response[i]) {
      case SERVICE_ID_DEHUMIDTFIER_TEMPERATURE_INDOOR:
        if (this->temperature_indoor_sensor_ != nullptr) {
          publish_i16(response, i, this->temperature_indoor_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_HUMIDITY_INDOOR:
        if (this->humidity_indoor_sensor_ != nullptr) {
          publish_u16(response, i, this->humidity_indoor_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_ERROR_CODR:
        if (this->error_code_sensor_ != nullptr) {
          publish_u16(response, i, this->error_code_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_CURRENT:
        if (this->operating_current_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->operating_current_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_VOLTAGE:
        if (this->operating_voltage_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_voltage_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_POWER:
        if (this->operating_power_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_power_sensor_);
        }
      break;
      case SERVICE_ID_DEHUMIDTFIER_ENERGY:
        if (this->energy_consumption_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->energy_consumption_sensor_);
        }
      break;
    }
  }
}

void DehumidifierSensor::update() {
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void WashingMachineSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Washing Machine:");
  if (this->wash_left_count_sensor_ != nullptr)
    LOG_SENSOR("  ", "Wash Left Count", this->wash_left_count_sensor_);
  if (this->wash_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Wash Left Hours", this->wash_left_hours_sensor_);
  if (this->spin_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Spin Left Hours", this->spin_left_hours_sensor_);
  if (this->soak_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Soak Left Hours", this->soak_left_hours_sensor_);
  if (this->dry_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Dry Left Hours", this->dry_left_hours_sensor_);
  if (this->tank_clean_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Tank Clean Left Hours", this->tank_clean_left_hours_sensor_);
  if (this->dry_clean_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Dry Left Left Hours", this->dry_clean_left_hours_sensor_);
  if (this->total_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Total Left Hours", this->total_left_hours_sensor_);
  if (this->appoint_left_hours_sensor_ != nullptr)
    LOG_SENSOR("  ", "Appoint Left Hours", this->appoint_left_hours_sensor_);
  if (this->operating_current_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Current", this->operating_current_sensor_);
  if (this->operating_voltage_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Voltage", this->operating_voltage_sensor_);
  if (this->operating_power_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating power", this->operating_power_sensor_);
  if (this->energy_consumption_sensor_ != nullptr)
    LOG_SENSOR("  ", "Energy Consumption", this->energy_consumption_sensor_);
  this->parent_->set_have_sensors(true);
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void WashingMachineSensor::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    if ((response[i + 1] == 0xFF) && (response[i + 2] == 0xFF)) {
      continue;
    }
    switch (response[i]) {
      case SERVICE_ID_WASHER_CURRENT_STATUS:
        if (this->wash_left_count_sensor_ != nullptr) {
          publish_u16(response, i, this->wash_current_status_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_WASH_LEFT_COUNT:
        if (this->wash_left_count_sensor_ != nullptr) {
          publish_u16(response, i, this->wash_left_count_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_WASH_LEFT_HOURS:
        if (this->wash_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->wash_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_SPIN_LEFT_HOURS:
        if (this->spin_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->spin_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_SOAK_LEFT_HOURS:
        if (this->soak_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->soak_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_DRY_LEFT_HOURS:
        if (this->dry_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->dry_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_TANK_CLEAN_LEFT_HOURS:
        if (this->wash_left_count_sensor_ != nullptr) {
          publish_u16(response, i, this->wash_left_count_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_DRY_CLEAN_LEFT_HOURS:
        if (this->tank_clean_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->tank_clean_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_TOTAL_LEFT_HOURS:
        if (this->total_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->total_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_APPOINT_LEFT_HOURS:
        if (this->appoint_left_hours_sensor_ != nullptr) {
          publish_u16(response, i, this->appoint_left_hours_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_CURRENT:
        if (this->operating_current_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->operating_current_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_VOLTAGE:
        if (this->operating_voltage_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_voltage_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_POWER:
        if (this->operating_power_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_power_sensor_);
        }
      break;
      case SERVICE_ID_WASHER_ENERGY:
        if (this->energy_consumption_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->energy_consumption_sensor_);
        }
      break;
    }
  }
}

void WashingMachineSensor::update() {
  this->parent_->send(6, 0, SA_ID_CLIMATE, 0x0, 0xffff);
}

void AirPurifierSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "TaiXIA Air Purifier Sensor:");
  if (this->air_quality_sensor_ != nullptr)
    LOG_SENSOR(TAG, "Air Quality", this->air_quality_sensor_);
  if (this->operating_current_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Current", this->operating_current_sensor_);
  if (this->operating_voltage_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Voltage", this->operating_voltage_sensor_);
  if (this->operating_power_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating power", this->operating_power_sensor_);
  if (this->energy_consumption_sensor_ != nullptr)
    LOG_SENSOR("  ", "Energy Consumption", this->energy_consumption_sensor_);
  this->parent_->set_have_sensors(true);
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void AirPurifierSensor::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    if ((response[i + 1] == 0xFF) && (response[i + 2] == 0xFF)) {
      continue;
    }
    switch (response[i]) {
      case SERVICE_ID_PURIFIER_AIR_QUALITY:
        if (this->air_quality_sensor_ != nullptr) {
          publish_i16(response, i, this->air_quality_sensor_);
        }
      break;
      case SERVICE_ID_PURIFIER_CURRENT:
        if (this->operating_current_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->operating_current_sensor_);
        }
      break;
      case SERVICE_ID_PURIFIER_VOLTAGE:
        if (this->operating_voltage_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_voltage_sensor_);
        }
      break;
      case SERVICE_ID_PURIFIER_POWER:
        if (this->operating_power_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_power_sensor_);
        }
      break;
      case SERVICE_ID_PURIFIER_ENERGY:
        if (this->energy_consumption_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->energy_consumption_sensor_);
        }
      break;
    }
  }
}

void AirPurifierSensor::update() {
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void ElectricFanSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "TaiXIA Fan Sensor:");
  if (this->temperature_sensor_ != nullptr)
    LOG_SENSOR(TAG, "Temperature", this->temperature_sensor_);
  if (this->humidity_sensor_ != nullptr)
    LOG_SENSOR(TAG, "Humidity", this->humidity_sensor_);
  if (this->operating_current_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Current", this->operating_current_sensor_);
  if (this->operating_voltage_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating Voltage", this->operating_voltage_sensor_);
  if (this->operating_power_sensor_ != nullptr)
    LOG_SENSOR("  ", "Operating power", this->operating_power_sensor_);
  if (this->energy_consumption_sensor_ != nullptr)
    LOG_SENSOR("  ", "Energy Consumption", this->energy_consumption_sensor_);
  this->parent_->set_have_sensors(true);
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void ElectricFanSensor::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGD(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    if ((response[i + 1] == 0xFF) && (response[i + 2] == 0xFF)) {
      continue;
    }
    switch (response[i]) {
      case SERVICE_ID_FAN_TEMPERATURE:
        if (this->temperature_sensor_ != nullptr) {
          publish_i16(response, i, this->temperature_sensor_);
        }
      break;
      case SERVICE_ID_FAN_HUMIDITY:
        if (this->humidity_sensor_ != nullptr) {
          publish_i16(response, i, this->humidity_sensor_);
        }
      break;
      case SERVICE_ID_FAN_CURRENT:
        if (this->operating_current_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->operating_current_sensor_);
        }
      break;
      case SERVICE_ID_FAN_VOLTAGE:
        if (this->operating_voltage_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_voltage_sensor_);
        }
      break;
      case SERVICE_ID_FAN_POWER:
        if (this->operating_power_sensor_ != nullptr) {
          publish_u16(response, i, this->operating_power_sensor_);
        }
      break;
      case SERVICE_ID_FAN_ENERGY:
        if (this->energy_consumption_sensor_ != nullptr) {
          publish_u16_div_10(response, i, this->energy_consumption_sensor_);
        }
      break;
    }
  }
}

void ElectricFanSensor::update() {
  this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
}

void TaiXiaCustomSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "TaiXIA Custom Sensor:");
  if (this->sa_id_ == 0xffff) {
    ESP_LOGCONFIG(TAG, "  SA ID: ANY");
  } else {
    ESP_LOGCONFIG(TAG, "  SA ID: 0x%04x", this->sa_id_);
  }
  ESP_LOGCONFIG(TAG, "  Sensors:");
  for (TaiXiaCustomSubSensor *sensor : this->sensors_)
    LOG_SENSOR("  ", "-", sensor);
}

void TaiXiaCustomSensor::handle_response(std::vector<uint8_t> &response) {
  for (TaiXiaCustomSubSensor *sensor : this->sensors_)
    sensor->parse_response(response);
}

void TaiXiaCustomSubSensor::parse_response(std::vector<uint8_t> &response) {
  this->publish_state(this->response_parser_(response));
}

}  // namespace taixia
}  // namespace esphome
