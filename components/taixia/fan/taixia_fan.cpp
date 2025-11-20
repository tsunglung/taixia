#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia_fan.h"

#include <vector>

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.fan";

  static inline uint16_t get_u16(std::vector<uint8_t> &response, int start) {
    return (response[start] << 8) + response[start + 1];
  }

  static inline std::string get_preset_mode_(uint16_t mode) {
    switch (mode){
      case 0:
        return "auto";
      break;
      case 1:
        return "normal";
      break;
      case 2:
        return "away";
      break;
      case 3:
        return "boost";
      break;
      case 4:
        return "baby";
      break;
      case 5:
        return "sleep";  // anti-mildew, anti-mite
      break;
      case 6:
        return "fan";
      break;
      case 7:
        return "comfort";
      break;
      case 8:
        return "home";
      break;
      case 9:
        return "eco";
      break;
    }
    return "unknown";
  }

  static inline uint16_t get_preset_mode_value(std::string mode) {
      if (!mode.compare("auto"))
        return 0;
      if (!mode.compare("normal"))
        return 1;
      if (!mode.compare("away"))
        return 2;
      if (!mode.compare("boost"))
        return 3;
      if (!mode.compare("baby"))
        return 4;
      if (!mode.compare("sleep"))  // anti-mildew, anti-mite
        return 5;
      if (!mode.compare("fan"))
        return 6;
      if (!mode.compare("comfort"))
        return 7;
      if (!mode.compare("home"))
        return 8;
      if (!mode.compare("eco"))
        return 9;
      ESP_LOGE(TAG, "unknow mode %s", mode.c_str());
      return 0;
  }

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

    uint8_t command[6] = {0x06, this->sa_id_, 0x00, 0xFF, 0xFF, 0x00};
    uint8_t buffer[6];

    if (this->sa_id_ == SA_ID_FAN) {
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
          this->speed = buffer[4];
    } else if (this->sa_id_ == SA_ID_DEHUMIDIFIER) {
        command[2] = SERVICE_ID_DEHUMIDTFIER_STATUS;
        command[5] = this->parent_->checksum(command, 5);
        this->parent_->send_cmd(command, buffer, 6);
        if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
          ESP_LOGV(TAG, "Invaild value");
        else
          this->state = buffer[4];

        command[2] = SERVICE_ID_DEHUMIDTFIER_FAN_LEVEL;
        command[5] = this->parent_->checksum(command, 5);
        this->parent_->send_cmd(command, buffer, 6);
        if ((buffer[3] == 0xFF) && (buffer[4] == 0xFF))
          ESP_LOGV(TAG, "Invaild value");
        else
          this->speed = buffer[4];
    }

    this->publish_state();
  }

  void TaiXiaFan::update() {
    if (!this->parent_->have_sensors())
      this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
  }

  void TaiXiaFan::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

    if (response[1] == 0x00 && response[2] == SERVICE_ID_READ_STATUS) {
        for (i = 3; i < response[0] - 3; i+=3) {
            if (this->sa_id_ == SA_ID_FAN) {
                switch (response[i]) {
                  case SERVICE_ID_FAN_STATUS:
                    this->state = get_u16(response, i + 1);
                    this->parent_->power_switch(this->state);
                  break;
                  case SERVICE_ID_FAN_SPEED:
                    this->speed = get_u16(response, i + 1);
                  break;
                  case SERVICE_ID_FAN_OSCILLATE:
                    this->oscillating = get_u16(response, i + 1);
                  break;
                }
            } else if (this->sa_id_ == SA_ID_DEHUMIDIFIER) {
                switch (response[i]) {
                  case SERVICE_ID_DEHUMIDTFIER_STATUS:
                    this->state = get_u16(response, i + 1);
                    this->parent_->power_switch(this->state);
                  break;
                  case SERVICE_ID_DEHUMIDTFIER_FAN_LEVEL:
                    this->speed = get_u16(response, i + 1);
                  break;
                  case SERVICE_ID_DEHUMIDTFIER_MODE:
                    this->set_preset_mode_(get_preset_mode_(get_u16(response, i + 1)));
                  break;
                }
            }
        }
        this->publish_state();
    }
  }

  fan::FanTraits TaiXiaFan::get_traits() {
    fan::FanTraits traits = fan::FanTraits(this->oscillation_.has_value(), this->speed_.has_value(), false,
                          this->speed_count_);
    traits.set_supported_preset_modes(this->preset_modes_);
    return traits;
  }

  void TaiXiaFan::control(const fan::FanCall &call) {
    bool set_speed = false;
    uint8_t command[6] = {0x06, this->sa_id_, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    uint8_t status = 0;
    uint8_t speed = 0;
    uint8_t oscillate = 0;
    uint8_t preset_mode = 0;

    if (this->sa_id_ == SA_ID_DEHUMIDIFIER) {
        status = SERVICE_ID_DEHUMIDTFIER_STATUS;
        speed = SERVICE_ID_DEHUMIDTFIER_FAN_LEVEL;
        oscillate = 0xff;
        preset_mode = SERVICE_ID_DEHUMIDTFIER_MODE;
    } else {
        status = SERVICE_ID_FAN_STATUS;
        speed = SERVICE_ID_FAN_SPEED;
        oscillate = SERVICE_ID_FAN_OSCILLATE;
        preset_mode = SERVICE_ID_FAN_MODE;
    }

    if (call.get_speed().has_value()) {
        this->speed = *call.get_speed();
        if (this->speed == 0) {
            command[2] = WRITE | status;
            command[4] = 0x0;
            command[5] = this->parent_->checksum(command, 5);
            this->parent_->send_cmd(command, buffer, 6);
        } else {
            // this->state = *call.get_state();
            if (this->state == 0) {
              command[2] = WRITE | status;
              command[4] = 0x1;
              command[5] = this->parent_->checksum(command, 5);
              this->parent_->send_cmd(command, buffer, 6);
            }
            command[2] = WRITE | speed;
            command[4] = this->speed;
            command[5] = this->parent_->checksum(command, 5);
            this->parent_->send_cmd(command, buffer, 6);
        }
        set_speed = true;
    }
    ESP_LOGE(TAG, "set speed %d", set_speed);
    if (call.get_state().has_value()) {
        this->state = *call.get_state();
        if (!set_speed) {
            if (this->state == 0) {
              command[2] = WRITE | status;
              command[5] = this->parent_->checksum(command, 5);
              this->parent_->send_cmd(command, buffer, 6);
            } else {
              command[2] = WRITE | status;
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
            command[2] = WRITE | oscillate;
            command[4] = 0x1;
            command[5] = this->parent_->checksum(command, 5);
            this->parent_->send_cmd(command, buffer, 6);
        } else {
            command[2] = WRITE | oscillate;
            command[4] = 0x0;
            command[5] = this->parent_->checksum(command, 5);
            this->parent_->send_cmd(command, buffer, 6);
        }
    }
    if (call.get_preset_mode() != nullptr) {
      uint8_t mode = get_preset_mode_value(call.get_preset_mode());

      command[2] = WRITE | preset_mode;
      command[4] = mode;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
    }

    this->publish_state();
    this->parent_->send(6, 0, 0, SERVICE_ID_READ_STATUS, 0xffff);
  }


}  // namespace taixia
}  // namespace esphome
