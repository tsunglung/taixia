#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../taixia.h"

namespace esphome {
namespace taixia {

class TaiXiaTextSensor : public Component {
 public:
  float get_setup_priority() const override;

  void dump_config() override;
  void setup() override;

  void set_taixia_parent(TaiXia *parent) { this->parent_ = parent; }

 protected:
  TaiXia *parent_;
};

}  // namespace taixia
}  // namespace esphome
