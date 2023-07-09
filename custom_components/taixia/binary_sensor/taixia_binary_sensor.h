#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

class TaiXiaBinarySensor : public binary_sensor::BinarySensor, public TaiXiaListener, public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;
  void set_power_sensor(binary_sensor::BinarySensor *binary_sensor) { power_binary_sensor_ = binary_sensor; }
  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;

  binary_sensor::BinarySensor *power_binary_sensor_{nullptr};

  void handle_response(std::vector<uint8_t> &response) override;
};

}  // namespace taixia
}  // namespace esphome