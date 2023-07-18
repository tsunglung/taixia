#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "taixia_number.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.nubmer";

  void TaiXiaNumber::dump_config() {
    LOG_NUMBER(TAG, " TaiXIA Number", this);

    uint8_t buffer[6];
    this->parent_->get_number(this->sa_id_, this->service_id_, buffer);

    if ((buffer[0] >= 0x0) && (buffer[1] == this->sa_id_) && (buffer[2] == this->service_id_))
      this->publish_state((buffer[3] << 8) + buffer[4]);
  }

  void TaiXiaNumber::control(float value) {
    if (this->service_id_ > 0) {
      this->parent_->set_number(this->sa_id_, this->service_id_, value);
    }
  }

}  // namespace taixia
}  // namespace esphome