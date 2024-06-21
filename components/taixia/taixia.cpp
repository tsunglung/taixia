#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia";
static const uint8_t RESPONSE_LENGTH = 255;

  void TaiXia::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA:");
    ESP_LOGCONFIG(TAG, "    SA_ID: %x", this->sa_id_);
    check_uart_settings(9600);
  }

  // ---=== Calc XOR ===---
  uint8_t TaiXia::checksum(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0x0;
    while (len--) {
      crc ^= *data++;
    }
    return crc;
  }

  bool TaiXia::write_command_(const uint8_t *command, uint8_t *response, uint8_t len, uint8_t rlen, uint32_t timeout) {
    ESP_LOGV(TAG, "command %x %x %x %x %x %x, wait %d us", command[0], command[1], command[2], command[3], command[4], command[5], this->response_time_);

    if (response == nullptr)
      return true;

    this->flush();

    this->write_array(command, len);

    if (this->response_time_ != 0) {
      delayMicroseconds(this->response_time_);
      while (!available() && timeout > 0) {
        timeout--;
        if (timeout == 0) {
            ESP_LOGE(TAG, "command timeout!");
            return false;
        }
      }
    }
    this->flush();

    // wait for data
    bool ret = this->read_array(response, rlen);
    this->flush();
    uint8_t crc = this->checksum(response, rlen - 1);
    if (crc != response[rlen - 1])
        return false;
    return ret;
  }

  bool TaiXia::write_command_(const uint8_t *command, uint8_t *response, uint8_t len, uint8_t rlen) {
    return this->write_command_(command, response, len, rlen, 6000);
  }

  void TaiXia::get_info_() {
    uint8_t i;
    this->buffer_.clear();

    this->send(6, 0, 0x00, SERVICE_ID_READ_VERSION, 0xFFFF);
    this->readline(false);

    if ((this->buffer_[0] >= 0x0) && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_READ_VERSION)) {
      if (this->version_textsensor_ != nullptr) {
        std::string version;
        version = format_hex_pretty(this->buffer_[3]) + "." + format_hex_pretty(this->buffer_[4]);
        this->version_textsensor_->publish_state(version);
      }
    }
    this->buffer_.clear();

    this->send(6, 0, 0x00, SERVICE_ID_READ_SA_ID, 0xFFFF);
    this->readline(false);

    if ((this->buffer_[0] >= 0x0) && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_READ_SA_ID)) {
      if (this->sa_id_textsensor_ != nullptr) {
        std::string sa_id;
        sa_id = format_hex_pretty(this->buffer_[3]) + format_hex_pretty(this->buffer_[4]);
        this->sa_id_textsensor_->publish_state(sa_id);
      }
      // if not preset sa_id
      if (this->sa_id_ == 0)
        this->sa_id_ = this->buffer_[3] << 8 | this->buffer_[4];
    }
    this->buffer_.clear();

    this->send(6, 0, 0x00, SERVICE_ID_READ_BRAND, 0xFFFF);
    this->readline(false);

    if ((this->buffer_[0] >= 0x0) && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_READ_BRAND)) {
      std::string brand;
      for (i = 3; i < this->buffer_[0]; i++) {
        if (this->buffer_[i] != 0x0) {
          brand = brand + str_sprintf("%c", this->buffer_[i]);
        } else {
          break;
        }
      }
      if (this->brand_textsensor_ != nullptr) {
        this->brand_textsensor_->publish_state(brand);
      }
    }
    this->buffer_.clear();

    this->send(6, 0, 0x00, SERVICE_ID_READ_MODEL, 0xFFFF);
    this->readline(false);

    if ((this->buffer_[0] >= 0x0) && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_READ_MODEL)) {
      std::string model;
      for (i = 3; i < this->buffer_[0]; i++) {
        if (this->buffer_[i] != 0x0) {
          model = model + str_sprintf("%c", this->buffer_[i]);
        } else {
          break;
        }
      }
      if (this->model_textsensor_ != nullptr) {
        this->model_textsensor_->publish_state(model);
      }
    }
    this->buffer_.clear();

    this->send(6, 0, 0x00, SERVICE_ID_READ_SERVICES, 0xFFFF);
    this->readline(false);

    // compatible with Panasonic which do not include service id of read services in response
    if ((this->buffer_[0] >= 0x0)
//        && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_READ_SERVICES)
      ) {
      std::string services;
      uint8_t start = 1;
      if (this->buffer_[2] == SERVICE_ID_READ_SERVICES)
        start = 3;

      for (i = start; i < this->buffer_[0]; i++) {
        services = services + format_hex_pretty(this->buffer_[i]) + " ";
      }
      if (this->services_textsensor_ != nullptr) {
        this->services_textsensor_->publish_state(services);
      }
    }
    this->buffer_.clear();
  }

  void TaiXia::setup() {
    uint8_t i, j, k;

    if (this->version_ < 3.0)
      return;

    this->send(6, 0, 0x00, SERVICE_ID_REGISTER, 0xFFFF);
    this->readline(false);

//    uint8_t crc = this->checksum(this->buffer_, this->buffer_[0] - 1);
    if ((this->buffer_[0] >= 0x0) && (this->buffer_[1] == 0x0) && (this->buffer_[2] == SERVICE_ID_REGISTER)) {

      std::string brand;
      for (i = 8; i < this->buffer_[0]; i++) {
        if (this->buffer_[i] != 0x0) {
          brand = brand + str_sprintf("%c", this->buffer_[i]);
        } else {
          break;
        }
      }

      std::string model;
      for (j = i + 1; j < this->buffer_[0]; j++) {
        if (this->buffer_[j] != 0x0) {
          model = model + str_sprintf("%c", this->buffer_[j]);
        } else {
          break;
        }
      }

      std::string services;
      for (k = j + 1; k < this->buffer_[0]; k++) {
        services = services + format_hex_pretty(this->buffer_[k]) + " ";
      }

      if (this->sa_id_textsensor_ != nullptr) {
        std::string sa_id;
        sa_id = format_hex_pretty(this->buffer_[6]) + format_hex_pretty(this->buffer_[7]);
        this->sa_id_textsensor_->publish_state(sa_id);
      }
      if (this->brand_textsensor_ != nullptr) {
        this->brand_textsensor_->publish_state(brand);
      }
      if (this->model_textsensor_ != nullptr) {
        this->model_textsensor_->publish_state(model);
      }
      if (this->services_textsensor_ != nullptr) {
        this->services_textsensor_->publish_state(services);
      }
      if (this->version_textsensor_ != nullptr) {
        std::string version;
        version = format_hex_pretty(this->buffer_[3]) + "." + format_hex_pretty(this->buffer_[4]);
        this->version_textsensor_->publish_state(version);
      }

      // if not preset sa_id
      if (this->sa_id_ == 0)
        this->sa_id_ = this->buffer_[6] << 8 | this->buffer_[7];
      } else {
        this->get_info_();
      }
  }

  void TaiXia::switch_command(uint8_t sa_id, uint8_t service_id, bool onoff) {
    ESP_LOGV(TAG, "switch command: %d %d %d %x", this->sa_id_, sa_id, service_id, onoff);
    if (this->sa_id_ != sa_id)
      return;

    uint8_t response[RESPONSE_LENGTH];
    uint8_t cmd[6] = {0x06, sa_id, (uint8_t)(WRITE | service_id), 0x00, 0x00, 0x00};
    if (onoff)
      cmd[4] = 0x01;
    else
      cmd[4] = 0x00;
    cmd[5] = this->checksum(cmd, 5);

    this->write_command_(cmd, response, 6, 6);
  }

  void TaiXia::set_number(uint8_t sa_id, uint8_t service_id, float value) {
    ESP_LOGV(TAG, "set number: %d %d %f", sa_id, service_id, value);
    if (this->sa_id_ != sa_id)
      return;

    uint8_t response[RESPONSE_LENGTH];
    uint8_t cmd[6] = {0x06, sa_id, (uint8_t)(WRITE | service_id), 0x00, 0x00, 0x00};

    cmd[3] = (int(value) & 0xFF00) >> 8;
    cmd[4] = int(value) & 0xFF;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6);
  }

  void TaiXia::get_number(uint8_t sa_id, uint8_t service_id, uint8_t *response) {
    ESP_LOGV(TAG, "get number: %x %x %x", this->sa_id_ , sa_id, service_id);
    if (this->sa_id_ != sa_id)
      return;

    uint8_t cmd[6] = {0x06, sa_id, service_id, 0xFF, 0xFF, 0x00};
    cmd[5] = this->checksum(cmd, 5);

    uint8_t ret = this->write_command_(cmd, response, 6, 6);
  }

  bool TaiXia::read_climate_status_() {
    uint8_t response[6];
    uint8_t buffer[RESPONSE_LENGTH];
    uint8_t cmd[6] = {0x06, 0x00, 0x00, 0xFF, 0xFF, 0x00};
    uint8_t ret = 0;
    uint8_t i = 3;
    uint32_t timeout = 60000;

    memset(response, 0x00, 6);
    memset(buffer, 0x00, RESPONSE_LENGTH);

    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_STATUS;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_MODE;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_FAN_SPEED;
    cmd[5] = this->checksum(cmd, 5);

    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_TARGET_TEMPERATURE;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_TEMPERATURE_INDOOR;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_OFF_TIME;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_ON_TIMER;
    cmd[5] = this->checksum(cmd, 5);

    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    timeout = 60000;
    cmd[1] = this->sa_id_;
    cmd[2] = SERVICE_ID_CLIMATE_SWING_HORIZONTAL;
    cmd[5] = this->checksum(cmd, 5);

    this->write_command_(cmd, response, 6, 6, timeout);
    buffer[i++] = response[2];
    buffer[i++] = response[3];
    buffer[i++] = response[4];

    buffer[i] = this->checksum(cmd, i);

    this->buffer_.clear();
    buffer[0] = i;
    for (uint8_t j = 0; j < i; j++) {
      this->buffer_.push_back(buffer[j]);
    }

    ESP_LOGV(TAG, "cmd buffer: %x %x %x %x %x %x", this->buffer_[0], this->buffer_[1], this->buffer_[2], this->buffer_[3], this->buffer_[4], this->buffer_[5]);
    for (auto &listener : this->listeners_)
      listener->on_response(this->sa_id_, this->buffer_);

    this->buffer_.clear();

    return true;
  }

  bool TaiXia::read_sa_status() {
    if (this->sa_id_ == 1)
      return this->read_climate_status_();
    return false;
  }

  void TaiXia::button_command(uint8_t sa_id, uint8_t service_id) {
    if (service_id == 0x00)
      this->get_info_();
  }

  void TaiXia::readline(bool handle_response) {
      uint8_t c, len;
      uint8_t response[255];
      read_byte(&c);

      this->len_ = c;
      this->buffer_.push_back(c);

      len = this->len_ - 1;
      if ((this->max_length_ > 6) && (len >= this->max_length_))
          len = this->max_length_;

      // wait for data
      read_array(response, len);

      for (uint8_t i = 0; i < len; i++) {
        this->buffer_.push_back(response[i]);
      }

      if (handle_response) {
        auto crc = this->checksum(response, len - 1) ^ (len + 1);
        if (crc == response[len - 1]) {
          for (auto &listener : this->listeners_)
            listener->on_response(this->sa_id_, this->buffer_);
        }
        this->buffer_.clear();
      }
  }

  void TaiXia::loop() {
    if (!available())
      return;

    while (available()) {
      readline(true);
    }
  }

  bool TaiXia::send(uint8_t packet_length, uint8_t data_type, uint8_t sa_id, uint8_t service_id, uint16_t data) {
    uint8_t frame[255];

    if (this->protocol_ != data_type)
      data_type = this->protocol_;

    if (data_type == 1) {
      frame[0] = packet_length;
      frame[1] = sa_id;
      frame[2] = service_id;
      // ....
    } else {
      frame[0] = packet_length;
      frame[1] = sa_id;
      frame[2] = service_id;
      frame[3] = data >> 8;
      frame[4] = data >> 0;
      auto crc = this->checksum(frame, 5);
      frame[5] =  crc;
    }

    this->write_array(frame, packet_length);
    this->flush();
    return true;
  }

  void TaiXiaListener::on_response(uint16_t sa_id, std::vector<uint8_t> &response) {
//    if ((this->sa_id_ != 0xffff) && (this->sa_id_ != sa_id))
//      return;
    this->handle_response(response);
  }

}  // namespace taixia
}  // namespace esphome
