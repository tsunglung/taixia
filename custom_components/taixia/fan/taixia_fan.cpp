#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia_fan.h"

#include <vector>

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.fan";

  void TaiXiaFan::setup() {
    auto restore = this->restore_state_();
    if (restore.has_value()) {
      restore->apply(*this);
      //this->write_state_();
    }
  }

  void TaiXiaFan::dump_config() {
    LOG_FAN(TAG, " TaiXIA Fan", this);
    if (this->speed_.has_value()) {
      ESP_LOGCONFIG(TAG, "  Speed %u", *this->speed_);
    }
    if (this->mode_.has_value()) {
      ESP_LOGCONFIG(TAG, "  Mode %u", *this->mode_);
    }
    if (this->oscillation_.has_value()) {
      ESP_LOGCONFIG(TAG, "  Oscillation: %u", *this->oscillation_);
    }
    ESP_LOGCONFIG(TAG, "  Speed list: %u", this->speed_count_);

    uint8_t command[6] = {0x06, SA_ID_FAN, 0x00, 0xFF, 0xFF, 0x00};
    uint8_t buffer[6];

    command[2] = SERVICE_ID_FAN_STATUS;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
    if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
      ESP_LOGV(TAG, "Invaild value");
    else
      this->state = buffer[4];

    command[2] = SERVICE_ID_FAN_OSCILLATE;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
    if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
      ESP_LOGV(TAG, "Invaild value");
    else
      this->oscillation_ = buffer[4];

    command[2] = SERVICE_ID_FAN_SPEED;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
    if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
      ESP_LOGV(TAG, "Invaild value");
    else
      this->speed_ = buffer[4];

    this->publish_state();
  }

  void TaiXiaFan::handle_response(std::vector<uint8_t> &response) {
    if (response[1] == 0x00 && response[2] == SERVICE_ID_READ_STATUS) {
        this->state = response[5];
        // this->speed = response[9];
        // this->oscillating = response[10];
        this->publish_state();
    }
  }

  fan::FanTraits TaiXiaFan::get_traits() {
    return fan::FanTraits(this->oscillation_.has_value(), this->speed_.has_value(), false,
                          this->speed_count_);
  }

  void TaiXiaFan::control(const fan::FanCall &call) {
    LOG_FAN(TAG, "TaiXia Fan", this);
    bool set_speed = false;
    uint8_t command[6] = {0x06, SA_ID_FAN, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];

    if (call.get_speed().has_value()) {
      this->speed = *call.get_speed();

      if (this->speed == 0) {
        command[2] = WRITE | SERVICE_ID_FAN_STATUS;
        command[4] = 0x0;
        command[5] = this->parent_->checksum(command, 5);
        this->parent_->send_cmd(command, buffer, 6);
      } else {
        // this->state = *call.get_state();
        if (this->state == 0) {
          command[2] = WRITE | SERVICE_ID_FAN_STATUS;
          command[4] = 0x1;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
        }
        command[2] = WRITE | SERVICE_ID_FAN_SPEED;
        command[4] = this->speed;
        command[5] = this->parent_->checksum(command, 5);
        this->parent_->send_cmd(command, buffer, 6);
      }
      set_speed = true;
    }
    if (call.get_state().has_value()) {
      this->state = *call.get_state();
      if (!set_speed) {
        if (this->state == 0) {
          command[2] = WRITE | SERVICE_ID_FAN_STATUS;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
        } else {
          command[2] = WRITE | SERVICE_ID_FAN_STATUS;
          command[4] = 0x1;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
        }
        this->parent_->power_switch(state);
      }
    }
    if (call.get_oscillating().has_value()) {
      this->oscillating = *call.get_oscillating();
      if (this->oscillating == 1) {
          command[2] = WRITE | SERVICE_ID_FAN_OSCILLATE;
          command[4] = 0x1;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
      } else {
          command[2] = WRITE | SERVICE_ID_FAN_OSCILLATE;
          command[4] = 0x0;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
      }
    }

    this->publish_state();
  }


}  // namespace taixia
}  // namespace esphome
