#include "esphome/core/log.h"
#include "taixia_text_sensor.h"

namespace esphome {
namespace taixia {

static const char *TAG = "taixia.text_sensor";

  void TaiXiaTextSensor::setup() {

  }

  float TaiXiaTextSensor::get_setup_priority() const { return setup_priority::DATA; }
  void TaiXiaTextSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA Text Sensor:");
  }

}  // namespace taixia
}  // namespace esphome
