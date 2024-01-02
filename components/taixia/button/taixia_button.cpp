#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "taixia_button.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.button";

  void TaiXiaButton::dump_config() {
    LOG_BUTTON("", "TaiXIA Button", this);

  }

  void TaiXiaButton::press_action() {
    if (this->service_id_ >= 0) {
      this->parent_->button_command(this->sa_id_, this->service_id_);
    }
  }

}  // namespace taixia
}  // namespace esphome