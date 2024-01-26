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

    command[2] = this->service_id_;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);

    if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
      ESP_LOGV(TAG, "Invaild value");
    else {
      this->state = bool(buffer[4]);
      if (((this->sa_id_ == SA_ID_CLIMATE) && (this->service_id_ == SERVICE_ID_CLIMATE_BEEPER)) || 
          ((this->sa_id_ == SA_ID_DEHUMIDTFIER) && (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER)))
          this->state = !this->state;
    }
  }

  void TaiXiaSwitch::write_state(bool state) {
    if (this->service_id_ >= 0) {
      bool org_state = state;
      if (((this->sa_id_ == SA_ID_CLIMATE) && (this->service_id_ == SERVICE_ID_CLIMATE_BEEPER)) || 
          ((this->sa_id_ == SA_ID_DEHUMIDTFIER) && (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER)))
        state = !state;
      this->parent_->switch_command(this->sa_id_, this->service_id_, state);
      this->publish_state(org_state);
    }
    this->parent_->send(6, 0, SA_ID_ALL, SERVICE_ID_READ_STATUS, 0xFFFF);
  }

}  // namespace taixia
}  // namespace esphome