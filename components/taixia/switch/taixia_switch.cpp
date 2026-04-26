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
          ((this->sa_id_ == SA_ID_DEHUMIDIFIER) && (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER))) {
        state = !state;
      }
      if (this->parent_->switch_command(this->sa_id_, this->service_id_, state)) {
        this->publish_state(org_state);
      }

      ESP_LOGV(TAG, "Control is %s", (this->parent_->get_optimistic() ? "optimistic" : "pessimistic"));
      if (!this->parent_->get_optimistic()) {
        if (this->parent_->get_version() < 3.0) {
          this->parent_->read_sa_status();
        } else {
          this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
        }
      }
    }
  }

  void TaiXiaSwitch::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    bool new_state = this->state;

    for (i = 3; i < response[0] - 3; i+=3) {
      // all the logic acts only if 'this' matches the response service_id
      // so just skip entries that do not match to speed things up a little
      if (this->service_id_ != response[i]) {
        continue;
      }

      ESP_LOGV(TAG, "handle_response[%d] {0x%2.2x, 0x%2.2x, 0x%2.2x}",
                    i, response[i+0], response[i+1], response[i+2]);

      new_state = bool(response[i + 2]);

      switch (this->sa_id_) {
        case SA_ID_CLIMATE:
          if (this->service_id_ == SERVICE_ID_CLIMATE_BEEPER) {
            new_state = !new_state;
          }
          break;
        case SA_ID_DEHUMIDIFIER:
          if (this->service_id_ == SERVICE_ID_DEHUMIDTFIER_BEEPER) {
            new_state = !new_state;
          }
          break;
      }
      break;
    }
publish:
    if (this->state != new_state) {
      this->publish_state(new_state);
    }
  }

}  // namespace taixia
}  // namespace esphome
