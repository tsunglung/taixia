#include "esphome/core/log.h"
#include "taixia_binary_sensor.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.binary_sensor";

  uint32_t lastPeriodicMillis = millis();

  void TaiXiaBinarySensor::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA Binary Sensor:");
    ESP_LOGCONFIG(TAG, "  Power Status");
  }

  void TaiXiaBinarySensor::update() {

    /*
      Reduce data update rate to prevent uart bus busy
    */
    uint32_t currentMillis = millis();
    if (currentMillis - lastPeriodicMillis < 6000)
      return;
    lastPeriodicMillis = currentMillis;

    this->parent_->send(6, 0, SA_ID_ALL, SERVICE_ID_READ_STATUS, 0xFFFF);
  }

  void TaiXiaBinarySensor::setup() { this->publish_initial_state(false); }

  void TaiXiaBinarySensor::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    ESP_LOGV(TAG, "handle_response");

    if ((response[1] == SA_ID_ALL) && (response[2] == SERVICE_ID_READ_STATUS)) {
      for (i = 3; i < response[0] - 3; i+=3) {
        switch (response[i]) {
          case 0x00:
            if (this->power_binary_sensor_ != nullptr)
              this->power_binary_sensor_->publish_state(response[i + 1] << 8 | response[i + 2]);
          break;
        }
      }
    }
  }

}  // namespace taixia
}  // namespace esphome