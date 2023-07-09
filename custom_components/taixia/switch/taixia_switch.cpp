#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "taixia_switch.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.switch";

  void TaiXiaSwitch::dump_config() {
    LOG_SWITCH("", "TaiXIA Switch", this);
    uint8_t command[6] = {0x06, this->sa_id_, 0x00, 0xFF, 0xFF, 0x00};
    uint8_t buffer[6];

    command[2] = 0x00;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
    if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
      ESP_LOGV(TAG, "Invaild value");
    else
      this->state = buffer[4];
  }

  void TaiXiaSwitch::write_state(bool state) {
    if (this->service_id_ >= 0) {
      this->parent_->switch_command(this->sa_id_, this->service_id_, state);
      this->publish_state(state);
    }
    this->parent_->send(6, 0, SA_ID_ALL, SERVICE_ID_READ_STATUS, 0xFFFF);
  }

}  // namespace taixia
}  // namespace esphome