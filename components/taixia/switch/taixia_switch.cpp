#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "taixia_switch.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.switch";

  void TaiXiaSwitch::dump_config() {
    LOG_SWITCH("", "TaiXIA Switch", this);
    if (!this->parent_->have_sensors())
      this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
  }

  void TaiXiaSwitch::write_state(bool state) {
    if (this->service_id_ >= 0) {
      bool org_state = state;
      if (((this->sa_id_ == SA_ID_CLIMATE) && (this->service_id_ == SERVICE_ID_CLIMATE_BEEPER)) || 
          ((this->sa_id_ == SA_ID_DEHUMIDIFIER) && (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER)))
        state = !state;
      this->parent_->switch_command(this->sa_id_, this->service_id_, state);
      this->publish_state(org_state);
    }
    this->parent_->send(6, 0, SA_ID_ALL, SERVICE_ID_READ_STATUS, 0xFFFF);
  }

  void TaiXiaSwitch::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 3; i < response[0] - 3; i+=3) {
      if (this->service_id_ == response[i])
        this->state = bool(response[i + 2]);
      if (((this->sa_id_ == SA_ID_CLIMATE) && (this->service_id_ == SERVICE_ID_CLIMATE_BEEPER)) || 
          ((this->sa_id_ == SA_ID_DEHUMIDIFIER) && (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER)))
          this->state = !this->state;
        goto done;
    }
done:
    this->publish_state(this->state);
  }

}  // namespace taixia
}  // namespace esphome