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

  bool TaiXia::write_command_(const uint8_t *command, uint8_t *response, uint8_t len, uint8_t rlen) {
    ESP_LOGV(TAG, "command %x %x %x %x %x %x", command[0], command[1], command[2], command[3], command[4], command[5]);
    this->write_array(command, len);

    if (response == nullptr)
      return true;

    // wait for data
    bool ret = this->read_array(response, rlen);
    this->flush();
    uint8_t crc = this->checksum(response, rlen - 1);
    if (crc != response[rlen - 1])
        return false;
    return ret;
  }

  void TaiXia::setup() {
    uint8_t i, j, k;

    this->send(6, 0, 0x00, SERVICE_ID_REGISTER, 0xFFFF);
    this->readline();
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
  //      sa_id = str_sprintf("%x%x", response[6], response[7]);
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
      if (this->sa_id_ == 0xffff)
        this->sa_id_ = this->buffer_[6] << 8 | this->buffer_[7];
    }
  }

  void TaiXia::switch_command(uint8_t sa_id, uint8_t service_id, bool onoff) {
    ESP_LOGV(TAG, "got command: %d %d %d %x", this->sa_id_, sa_id, service_id, onoff);
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
    ESP_LOGV(TAG, "got command: %d %d %f", sa_id, service_id, value);
    if (this->sa_id_ != sa_id)
      return;

    uint8_t response[RESPONSE_LENGTH];
    uint8_t cmd[6] = {0x06, sa_id, (uint8_t)(WRITE | service_id), 0x00, 0x00, 0x00};

    cmd[3] = int(value) << 8 & 0xFF00;
    cmd[4] = int(value) & 0xFF;
    cmd[5] = this->checksum(cmd, 5);
    this->write_command_(cmd, response, 6, 6);
  }

  void TaiXia::get_number(uint8_t sa_id, uint8_t service_id, uint8_t *response) {
    ESP_LOGV(TAG, "got command: %x %x %x", this->sa_id_ , sa_id, service_id);
    if (this->sa_id_ != sa_id)
      return;

    uint8_t cmd[6] = {0x06, sa_id, service_id, 0xFF, 0xFF, 0x00};
    cmd[5] = this->checksum(cmd, 5);

    this->write_command_(cmd, response, 6, 6);
  }

  void TaiXia::readline(void) {
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

      for (auto &listener : this->listeners_)
        listener->on_response(this->sa_id_, this->buffer_);
      this->state_ = 0;
      this->buffer_.clear();
  }

  void TaiXia::loop() {
    if (!available())
      return;

    while (available()) {
      readline();
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
