#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

class TaiXiaFan : public TaiXiaListener, public Component, public fan::Fan {
 public:
  TaiXiaFan(TaiXia *parent, int speed_count) : parent_(parent), speed_count_(speed_count) {}
  void setup() override;
  void dump_config() override;
  void set_speed(uint8_t speed) { this->speed_ = speed; }
  void set_speeds(uint8_t speeds) { this->speed_count_ = speeds; }
  void set_mode(uint8_t mode) { this->mode_ = mode; }
  void set_oscillation(uint8_t oscillation) { this->oscillation_ = oscillation; }
  void set_speed_count(uint8_t speed_count) { this->speed_count_ = speed_count; }
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

  void handle_response(std::vector<uint8_t> &response) override;
};

}  // namespace taixia
}  // namespace esphome
