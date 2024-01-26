#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/components/climate/climate.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

enum TaiXiaPreset : uint8_t {
  CLIMATE_PRESET_AIR_DETECT = climate::CLIMATE_PRESET_ACTIVITY + 1,
  CLIMATE_PRESET_ANTI_MILDEW = climate::CLIMATE_PRESET_ACTIVITY + 2,
  CLIMATE_PRESET_SELF_CLEAN = climate::CLIMATE_PRESET_ACTIVITY + 3,
  CLIMATE_PRESET_BODY_MOTION = climate::CLIMATE_PRESET_ACTIVITY + 4,
};

class TaiXiaClimate : public climate::Climate, public TaiXiaListener, public PollingComponent {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  climate::ClimateTraits traits() override;

  void set_max_temperature(float temp) { this->max_temp_ = temp; }
  void set_min_temperature(float temp) { this->min_temp_ = temp; }
  void set_temperature_step(float step) { this->temp_step_ = step; }
  void set_supported_modes(const std::set<esphome::climate::ClimateMode> &modes);
  void set_supported_fan_modes(const std::set<esphome::climate::ClimateFanMode> &modes);
  void set_supported_swing_modes(const std::set<esphome::climate::ClimateSwingMode> &modes);
  void set_supported_preset_modes(const std::set<esphome::climate::ClimatePreset> &modes);
  void set_supported_humidity(bool feature) { this->supported_humidity_ = feature; }

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  void control(const climate::ClimateCall &call) override;

  TaiXia *parent_;
  bool supports_cool_{true};
  bool supports_heat_{true};
  bool supports_dry_{false};
  bool supports_fan_only_{false};
  bool supported_humidity_{false};
  uint8_t preset_modes_;
  uint16_t fan_modes_;
  uint8_t swing_modes_;
  float max_temp_{19.0};
  float min_temp_{43.0};
  float temp_step_{1.0};
  esphome::climate::ClimateTraits traits_;

  void reset_state_();
  bool update_status_();

  void handle_response(std::vector<uint8_t> &response) override;
};

}  // namespace taixia
}  // namespace esphome
