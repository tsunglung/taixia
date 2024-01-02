#pragma once

#include "esphome/components/sensor/sensor.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

class AirConditionerSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void set_temperature_indoor_sensor(sensor::Sensor *sensor) { this->temperature_indoor_sensor_ = sensor; }
  void set_humidity_indoor_sensor(sensor::Sensor *sensor) { this->humidity_indoor_sensor_ = sensor; }
  void set_temperature_outdoor_sensor(sensor::Sensor *sensor) { this->temperature_outdoor_sensor_ = sensor; }
  void set_humidity_outdoor_sensor(sensor::Sensor *sensor) { this->humidity_outdoor_sensor_ = sensor; }
  void set_operating_current_sensor(sensor::Sensor *sensor) { this->operating_current_sensor_ = sensor; }
  void set_operating_voltage_sensor(sensor::Sensor *sensor) { this->operating_voltage_sensor_ = sensor; }
  void set_operating_power_sensor(sensor::Sensor *sensor) { this->operating_power_sensor_ = sensor; }
  void set_energy_consumption_sensor(sensor::Sensor *sensor) { this->energy_consumption_sensor_ = sensor; }
  void set_operating_hours_sensor(sensor::Sensor *sensor) { this->operating_hours_sensor_ = sensor; }
  void set_error_code_sensor(sensor::Sensor *sensor) { this->error_code_sensor_ = sensor; }
  void set_filiter_clean_sensor(sensor::Sensor *sensor) { this->filiter_clean_hours_sensor_ = sensor; }

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;
  sensor::Sensor *temperature_indoor_sensor_{nullptr};
  sensor::Sensor *humidity_indoor_sensor_{nullptr};
  sensor::Sensor *temperature_outdoor_sensor_{nullptr};
  sensor::Sensor *humidity_outdoor_sensor_{nullptr};
  sensor::Sensor *operating_current_sensor_{nullptr};
  sensor::Sensor *operating_voltage_sensor_{nullptr};
  sensor::Sensor *operating_power_sensor_{nullptr};
  sensor::Sensor *energy_consumption_sensor_{nullptr};
  sensor::Sensor *operating_hours_sensor_{nullptr};
  sensor::Sensor *error_code_sensor_{nullptr};
  sensor::Sensor *filiter_clean_hours_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

class DehumidifierSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void set_temperature_indoor_sensor(sensor::Sensor *sensor) { this->temperature_indoor_sensor_ = sensor; }
  void set_humidity_indoor_sensor(sensor::Sensor *sensor) { this->humidity_indoor_sensor_ = sensor; }
  void set_water_full_sensor(sensor::Sensor *sensor) { this->water_full_sensor_ = sensor; }
  void set_filiter_clean_sensor(sensor::Sensor *sensor) { this->filiter_clean_sensor_ = sensor; }
  void set_side_air_vent_sensor(sensor::Sensor *sensor) { this->side_air_vent_sensor_ = sensor; }
  void set_defrost_sensor(sensor::Sensor *sensor) { this->defrost_sensor_ = sensor; }
  void set_operating_current_sensor(sensor::Sensor *sensor) { this->operating_current_sensor_ = sensor; }
  void set_operating_voltage_sensor(sensor::Sensor *sensor) { this->operating_voltage_sensor_ = sensor; }
  void set_operating_power_sensor(sensor::Sensor *sensor) { this->operating_power_sensor_ = sensor; }
  void set_energy_consumption_sensor(sensor::Sensor *sensor) { this->energy_consumption_sensor_ = sensor; }
  void set_operating_hours_sensor(sensor::Sensor *sensor) { this->operating_hours_sensor_ = sensor; }


  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;
  sensor::Sensor *temperature_indoor_sensor_{nullptr};
  sensor::Sensor *humidity_indoor_sensor_{nullptr};
  sensor::Sensor *water_full_sensor_{nullptr};
  sensor::Sensor *filiter_clean_sensor_{nullptr};
  sensor::Sensor *side_air_vent_sensor_{nullptr};
  sensor::Sensor *defrost_sensor_{nullptr};
  sensor::Sensor *operating_current_sensor_{nullptr};
  sensor::Sensor *operating_voltage_sensor_{nullptr};
  sensor::Sensor *operating_power_sensor_{nullptr};
  sensor::Sensor *energy_consumption_sensor_{nullptr};
  sensor::Sensor *operating_hours_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

class WashingMachineSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void set_wash_current_status_sensor(sensor::Sensor *sensor) { this->wash_current_status_sensor_ = sensor; }
  void set_wash_left_count_sensor(sensor::Sensor *sensor) { this->wash_left_count_sensor_ = sensor; }
  void set_wash_left_hours_sensor(sensor::Sensor *sensor) { this->wash_left_hours_sensor_ = sensor; }
  void set_spin_left_hours_sensor(sensor::Sensor *sensor) { this->spin_left_hours_sensor_ = sensor; }
  void set_soak_left_hours_sensor(sensor::Sensor *sensor) { this->soak_left_hours_sensor_ = sensor; }
  void set_dry_left_hours_sensor(sensor::Sensor *sensor) { this->dry_left_hours_sensor_ = sensor; }
  void set_tank_clean_left_hours_sensor(sensor::Sensor *sensor) { this->tank_clean_left_hours_sensor_ = sensor; }
  void set_dry_clean_left_hours_sensor(sensor::Sensor *sensor) { this->dry_clean_left_hours_sensor_ = sensor; }
  void set_total_left_hours_sensor(sensor::Sensor *sensor) { this->total_left_hours_sensor_ = sensor; }
  void set_appoint_left_hours_sensor(sensor::Sensor *sensor) { this->appoint_left_hours_sensor_ = sensor; }
  void set_operating_current_sensor(sensor::Sensor *sensor) { this->operating_current_sensor_ = sensor; }
  void set_operating_voltage_sensor(sensor::Sensor *sensor) { this->operating_voltage_sensor_ = sensor; }
  void set_operating_power_sensor(sensor::Sensor *sensor) { this->operating_power_sensor_ = sensor; }
  void set_energy_consumption_sensor(sensor::Sensor *sensor) { this->energy_consumption_sensor_ = sensor; }

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;
  sensor::Sensor *wash_current_status_sensor_{nullptr};
  sensor::Sensor *wash_left_count_sensor_{nullptr};
  sensor::Sensor *wash_left_hours_sensor_{nullptr};
  sensor::Sensor *spin_left_hours_sensor_{nullptr};
  sensor::Sensor *soak_left_hours_sensor_{nullptr};
  sensor::Sensor *dry_left_hours_sensor_{nullptr};
  sensor::Sensor *tank_clean_left_hours_sensor_{nullptr};
  sensor::Sensor *dry_clean_left_hours_sensor_{nullptr};
  sensor::Sensor *total_left_hours_sensor_{nullptr};
  sensor::Sensor *appoint_left_hours_sensor_{nullptr};
  sensor::Sensor *operating_current_sensor_{nullptr};
  sensor::Sensor *operating_voltage_sensor_{nullptr};
  sensor::Sensor *operating_power_sensor_{nullptr};
  sensor::Sensor *energy_consumption_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

class AirPurifierSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void set_air_quality_sensor(sensor::Sensor *sensor) { this->air_quality_sensor_ = sensor; }
  void set_operating_current_sensor(sensor::Sensor *sensor) { this->operating_current_sensor_ = sensor; }
  void set_operating_voltage_sensor(sensor::Sensor *sensor) { this->operating_voltage_sensor_ = sensor; }
  void set_operating_power_sensor(sensor::Sensor *sensor) { this->operating_power_sensor_ = sensor; }
  void set_energy_consumption_sensor(sensor::Sensor *sensor) { this->energy_consumption_sensor_ = sensor; }

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;

  sensor::Sensor *air_quality_sensor_{nullptr};
  sensor::Sensor *operating_current_sensor_{nullptr};
  sensor::Sensor *operating_voltage_sensor_{nullptr};
  sensor::Sensor *operating_power_sensor_{nullptr};
  sensor::Sensor *energy_consumption_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

class ElectricFanSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void update() override;
  void set_temperature_sensor(sensor::Sensor *sensor) { this->temperature_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { this->humidity_sensor_ = sensor; }
  void set_operating_current_sensor(sensor::Sensor *sensor) { this->operating_current_sensor_ = sensor; }
  void set_operating_voltage_sensor(sensor::Sensor *sensor) { this->operating_voltage_sensor_ = sensor; }
  void set_operating_power_sensor(sensor::Sensor *sensor) { this->operating_power_sensor_ = sensor; }
  void set_energy_consumption_sensor(sensor::Sensor *sensor) { this->energy_consumption_sensor_ = sensor; }

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *operating_current_sensor_{nullptr};
  sensor::Sensor *operating_voltage_sensor_{nullptr};
  sensor::Sensor *operating_power_sensor_{nullptr};
  sensor::Sensor *energy_consumption_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

class TaiXiaCustomSubSensor;

class TaiXiaCustomSensor : public TaiXiaListener, public PollingComponent {
 public:
  void dump_config() override;
  void set_sensors(std::vector<TaiXiaCustomSubSensor *> sensors) { this->sensors_ = std::move(sensors); };

 protected:
  std::vector<TaiXiaCustomSubSensor *> sensors_;
  void handle_response(std::vector<uint8_t> &response) override;
};

class TaiXiaCustomSubSensor : public sensor::Sensor, public Component {
 public:
  void set_response_parser(response_parser_t parser) { this->response_parser_ = std::move(parser); };
  void parse_response(std::vector<uint8_t> &response);

 protected:
  response_parser_t response_parser_;
};

}  // namespace taixia
}  // namespace esphome
