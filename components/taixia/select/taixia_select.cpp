#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia_select.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.select";

  static inline uint16_t get_u16(std::vector<uint8_t> &response, int start) {
    return (response[start] << 8) + response[start + 1];
  }

  static inline size_t get_mapping_idx(std::vector<uint8_t> &response, int start, std::vector<uint8_t> mappings) {
    if ((response[start + 1] == 0xFF) && (response[start + 2] == 0xFF)) {
      return -1;
    } else {
      uint8_t enum_value = get_u16(response, start + 1);

      auto it = std::find(mappings.cbegin(), mappings.cend(), enum_value);
      if (it == mappings.end()) {
        ESP_LOGW(TAG, "Invalid value %u", enum_value);
        return -1;
      }

      return std::distance(mappings.cbegin(), it);
    }

  }

  void AirConditionerSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "Air Conditioner:");
    if (this->fuzzy_mode_select_ != nullptr)
      LOG_SELECT("  ", "Fuzzy Mode", this->fuzzy_mode_select_);
    if (this->display_mode_select_ != nullptr)
      LOG_SELECT("  ", "Display Mode", this->display_mode_select_);
    if (this->motion_detect_select_ != nullptr)
      LOG_SELECT("  ", "Motion Detect", this->motion_detect_select_);
    if (this->swing_horizontal_level_select_ != nullptr)
      LOG_SELECT("  ", "Swing horizontal level", this->swing_horizontal_level_select_);
  }

  void AirConditionerSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 9; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_CLIMATE_FUZZY_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_CLIMATE_DISPLAY_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_CLIMATE_ACTIVITY:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_CLIMATE_SWING_HORIZONTAL_LEVEL:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }

      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void AirConditionerSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_CLIMATE, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

  void WashingMachineSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "Washing Machine:");
    if (this->wash_program_select_ != nullptr)
      LOG_SELECT("  ", "Wash Program", this->wash_program_select_);
    if (this->wash_other_function_select_ != nullptr)
      LOG_SELECT("  ", "Wash Other Function", this->wash_other_function_select_);
    if (this->wash_mode_select_ != nullptr)
      LOG_SELECT("  ", "Wash Mode", this->wash_mode_select_);
    if (this->warm_water_program_select_ != nullptr)
      LOG_SELECT("  ", "Warm Water Program", this->warm_water_program_select_);
  }

  void WashingMachineSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 9; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_WASHER_WASH_PROGRAM:
          mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_WASHER_OTHER_FUNCTION:
          mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_WASHER_WASH_MODE:
          mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_WASHER_WARM_WATER_PROGRAM:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }
      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void WashingMachineSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_WASHER, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

  void DehumidifierSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "Dehumidifier:");
    if (this->operating_program_select_ != nullptr)
      LOG_SELECT("  ", "Operating Program", this->operating_program_select_);
    if (this->air_purfifier_select_ != nullptr)
      LOG_SELECT("  ", "Air Purfifier", this->air_purfifier_select_);
  }

  void DehumidifierSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 6; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_DEHUMIDTFIER_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_DEHUMIDTFIER_AIR_PURFIFIER:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }
      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void DehumidifierSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_DEHUMIDIFIER, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

  void AirPurifierSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA Air Purifier Select:");
    if (this->operating_program_select_ != nullptr)
      LOG_SELECT("  ", "   Operating Program", this->operating_program_select_);
  }

  void AirPurifierSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 6; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_PURIFIER_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }
      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void AirPurifierSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_AIR_PURIFIER, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

  void ErvSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA Erv Select:");
    if (this->ventilate_mode_select_ != nullptr)
      LOG_SELECT("  ", "   Ventilate Mode", this->ventilate_mode_select_);
    if (this->pre_heat_cool_select_ != nullptr)
      LOG_SELECT("  ", "   Ventilate Mode", this->pre_heat_cool_select_);
  }

  void ErvSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 6; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_ERV_VENTILATE_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
        case SERVICE_ID_ERV_PRE_HEAT_COOL:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }
      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void ErvSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_FAN, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

  void ElectricFanSelect::dump_config() {
    ESP_LOGCONFIG(TAG, "TaiXIA Fan Select:");
    if (this->operating_program_select_ != nullptr)
      LOG_SELECT("  ", "   Operating Program", this->operating_program_select_);
  }

  void ElectricFanSelect::handle_response(std::vector<uint8_t> &response) {
    uint8_t i;
    size_t mapping_idx = -1;

    ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
        response[0], response[1], response[2], response[3], \
        response[4], response[5], response[6], response[7], response[8]);

    for (i = 6; i < response[0] - 3; i+=3) {
      switch (response[i]) {
        case SERVICE_ID_FAN_MODE:
            mapping_idx = get_mapping_idx(response, i, this->mappings_);
        break;
      }
      if ((mapping_idx != -1) && (this->service_id_ == response[i])) {
        auto value = this->at(mapping_idx);
        this->publish_state(value.value());
        return;
      }
    }
  }

  void ElectricFanSelect::control(const std::string &value) {
    uint8_t command[6] = {0x06, SA_ID_FAN, 0x00, 0x00, 0x00, 0x00};
    uint8_t buffer[6];
    auto idx = this->index_of(value);

    if (idx.has_value()) {
      uint8_t mapping = this->mappings_.at(idx.value());
      ESP_LOGV(TAG, "Setting value to %u:%s", mapping, value.c_str());
      command[2] = WRITE | this->service_id_;
      command[4] = mapping;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      return;
    }

    ESP_LOGW(TAG, "Invalid value %s", value.c_str());
  }

}  // namespace taixia
}  // namespace esphome
