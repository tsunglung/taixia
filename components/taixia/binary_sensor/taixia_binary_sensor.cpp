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

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 3; i < response[0] - 3; i+=3) {
      if (this->service_id_ == 0x00) {
        this->state = bool(response[4]);
        goto done;
      }
      if (this->sa_id_ == SA_ID_DEHUMIDIFIER) {
        switch (response[i]) {
          case SERVICE_ID_DEHUMIDTFIER_WATER_TANK_FULL:
            if (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_WATER_TANK_FULL) {
              this->state = bool(response[i + 2]);
              goto done;
            }
          break;
          case SERVICE_ID_DEHUMIDTFIER_AIR_PURFIFIER:
            if (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_AIR_PURFIFIER) {
              this->state = bool(response[i + 2]);
              goto done;
            }
          break;
          case SERVICE_ID_DEHUMIDTFIER_SIDE_AIR_VENT:
            if (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_SIDE_AIR_VENT) {
              this->state = bool(response[i + 2]);
              goto done;
            }
          break;
          case SERVICE_ID_DEHUMIDTFIER_DEFROST:
            if (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_DEFROST) {
              this->state = bool(response[i + 2]);
              goto done;
            }
          break;
        }
      }
    }
done:
    this->publish_state(this->state);
  }

}  // namespace taixia
}  // namespace esphome