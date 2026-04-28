#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "taixia_number.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.number";
static const uint8_t RESPONSE_LENGTH = 255;
static const uint8_t CMD_LENGTH = 6;

  static inline uint16_t get_u16(std::vector<uint8_t> &response, int start) {
    return (response[start] << 8) + response[start + 1];
  }

  void TaiXiaNumber::dump_config() {
    LOG_NUMBER(TAG, " TaiXIA Number", this);

    if (!this->parent_->have_sensors())
      this->parent_->send(CMD_LENGTH,
                          0,
                          SA_ID_ALL, // NOTE: should this not be this->parent_->sa_id_ ?
                          SERVICE_ID_READ_STATUS,
                          0xffff);
  }

  void TaiXiaNumber::control(float value) {
    if (this->service_id_ == 0) {
      ESP_LOGE(TAG, "Internal error! Invalid service_id_: 0x%2.2x",
               this->service_id_);
      return;
    }
    if (this->parent_->set_number(this->sa_id_, this->service_id_, value)) {
      this->publish_state(value);
    }
    this->parent_->read_appliance_status_conditional_();
  }

  void TaiXiaNumber::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;

    for (i = 3; i < response[0] - 3; i+=3) {
      // all the logic acts only if 'this' matches the response service_id
      // so just skip entries that do not match to speed things up a little
      if (this->service_id_ != response[i]) {
        continue;
      }

      ESP_LOGV(TAG, "handle_response[%d] {0x%2.2x, 0x%2.2x, 0x%2.2x}",
                    i, response[i+0], response[i+1], response[i+2]);

      this->publish_state(get_u16(response, i + 1));
      break;
    }
  }

}  // namespace taixia
}  // namespace esphome
