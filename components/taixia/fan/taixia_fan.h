#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

class TaiXiaFan : public TaiXiaListener, public PollingComponent, public fan::Fan {
 public:
  TaiXiaFan(TaiXia *parent, int speed_count) : parent_(parent), speed_count_(speed_count) {}
  void setup() override;
  void update() override;
  void dump_config() override;
  void set_speed(uint8_t speed) { this->speed_ = speed; }
  void set_speeds(uint8_t speeds) { this->speed_count_ = speeds; }
  void set_mode(uint8_t mode) { this->mode_ = mode; }
  void set_oscillation(uint8_t oscillation) { this->oscillation_ = oscillation; }
  void set_speed_count(uint8_t speed_count) { this->speed_count_ = speed_count; }
  void set_sa_id(uint8_t sa_id) { this->sa_id_ = sa_id; };
  void set_have_sensors(bool have_sensors) { this->have_sensors_ = have_sensors; };
  void set_preset_modes(std::initializer_list<const char *> presets) { preset_modes_ = presets; }
  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();

  TaiXia *parent_;
  optional<uint8_t> speed_{};
  optional<uint8_t> mode_{};
  optional<uint8_t> oscillation_{};
  int speed_count_{};
  uint8_t sa_id_;
  bool have_sensors_{false};
  std::vector<const char *> preset_modes_{};

  void handle_response(std::vector<uint8_t> &response) override;
};

}  // namespace taixia
}  // namespace esphome
