#include "esphome/core/log.h"
#include "taixia_binary_sensor.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.binary_sensor";

  void TaiXiaBinarySensor::dump_config() {
    LOG_BINARY_SENSOR("", "TaiXIA Binary Sensor", this);
    if (!this->parent_->have_sensors())
      this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
  }

  void TaiXiaBinarySensor::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    bool new_state = this->state;

    if (this->service_id_ == 0x00) {
      ESP_LOGV(TAG, "handle_response[%d] {0x%2.2x, 0x%2.2x, 0x%2.2x}",
                    i, response[i+0], response[i+1], response[i+2]);
      new_state = bool(response[4]);
      goto publish;
    }

    for (i = 3; i < response[0] - 3; i+=3) {
      // all the logic acts only if 'this' matches the response service_id
      // so just skip entries that do not match to speed things up a little
      if (this->service_id_ != response[i]) {
        continue;
      }

      ESP_LOGV(TAG, "handle_response[%d] {0x%2.2x, 0x%2.2x, 0x%2.2x}",
                    i, response[i+0], response[i+1], response[i+2]);

      // NOTE: this can be optimized even more
      switch(this->sa_id_) {
        case SA_ID_CLIMATE:
          switch (response[i]) {
            case SERVICE_ID_CLIMATE_FILTER_NOTIFY:
              new_state = bool(response[i + 2]);
              break;
          }
          break;
        case SA_ID_DEHUMIDIFIER:
          switch (response[i]) {
            case SERVICE_ID_DEHUMIDTFIER_WATER_TANK_FULL:
            case SERVICE_ID_DEHUMIDTFIER_AIR_PURFIFIER:
            case SERVICE_ID_DEHUMIDTFIER_SIDE_AIR_VENT:
            case SERVICE_ID_DEHUMIDTFIER_DEFROST:
            case SERVICE_ID_DEHUMIDTFIER_FILTER_RESET:
              new_state = bool(response[i + 2]);
              break;
          }
          break;
        case SA_ID_ERV:
          switch (response[i]) {
            case SERVICE_ID_ERV_RESET_FILTER_NOTIFY:
            case SERVICE_ID_ERV_FRONT_FILTER_NOTIFY:
            case SERVICE_ID_ERV_PM25_FILTER_NOTIFY:
              new_state = bool(response[i + 2]);
              break;
          }
          break;
      }
      break;
    }
publish:
    if (this->state != new_state) {
      this->publish_state(this->state);
    }
  }

}  // namespace taixia
}  // namespace esphome
