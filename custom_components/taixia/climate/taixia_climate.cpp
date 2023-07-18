#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "taixia_climate.h"

namespace esphome {
namespace taixia {

static const char *const TAG = "taixia.climate";

using namespace esphome::climate;

void TaiXiaClimate::dump_config() {
  LOG_CLIMATE("", "TaiXIA Climate", this);
  auto traits = this->get_traits();

  ESP_LOGCONFIG(TAG, "  Supported modes:");
  for (auto mode : traits.get_supported_modes()) {
    ESP_LOGCONFIG(TAG, "   - %s", LOG_STR_ARG(climate_mode_to_string(mode)));
  }

  ESP_LOGCONFIG(TAG, "  Supported fan modes:");
  for (const auto &mode : traits.get_supported_fan_modes()) {
    ESP_LOGCONFIG(TAG, "   - %s", LOG_STR_ARG(climate_fan_mode_to_string(mode)));
  }
  for (const auto &mode : traits.get_supported_custom_fan_modes()) {
    ESP_LOGCONFIG(TAG, "   - %s (c)", mode.c_str());
  }

  ESP_LOGCONFIG(TAG, "  Supported presets:");
  for (auto preset : traits.get_supported_presets()) {
    ESP_LOGCONFIG(TAG, "   - %s", LOG_STR_ARG(climate_preset_to_string(preset)));
  }
  for (const auto &preset : traits.get_supported_custom_presets()) {
    ESP_LOGCONFIG(TAG, "   - %s (c)", preset.c_str());
  }
}

void TaiXiaClimate::setup() {
  // restore set points
  auto restore = this->restore_state_();
  if (restore.has_value()) {
    ESP_LOGI(TAG, "Restored previous saved state.");
    restore->apply(this);
  } else {
    // Initial status is unknown until we connect
    this->reset_state_();
  }
  ESP_LOGD(TAG, "climate setup");
}

/** Resets states to defaults. */
void TaiXiaClimate::reset_state_() {
//  this->mode = CLIMATE_MODE_OFF;
//  this->action = CLIMATE_ACTION_IDLE;
  this->target_temperature = NAN;
  this->current_temperature = NAN;
  this->preset.reset();
  this->custom_preset.reset();
  this->publish_state();
}

void TaiXiaClimate::loop() {}

void TaiXiaClimate::control(const climate::ClimateCall &call) {
  //ESP_LOGD(TAG, "Received TaiXiaClimate::control");
  LOG_CLIMATE("", "TaiXia Climate conrol", this);
  uint8_t command[6] = {0x06, SA_ID_FAN, 0x00, 0x00, 0x00, 0x00};
  uint8_t buffer[6];

  if (call.get_mode().has_value()) {
      this->mode = *call.get_mode();
      if (this->mode == CLIMATE_MODE_OFF) {
          command[2] = WRITE | SERVICE_ID_CLIMATE_STATUS;
          command[4] = 0x0;
          this->parent_->power_switch(false);
      } else {
          uint8_t mode;
          command[2] = WRITE | SERVICE_ID_CLIMATE_STATUS;
          command[4] = 0x1;
          command[5] = this->parent_->checksum(command, 5);
          this->parent_->send_cmd(command, buffer, 6);
          switch (this->mode) {
            case CLIMATE_MODE_COOL:
              mode = 0;
              break;
            case CLIMATE_MODE_HEAT:
              mode = 4;
              break;
            case CLIMATE_MODE_DRY:
              mode = 1;
              break;
            case CLIMATE_MODE_FAN_ONLY:
              mode = 2;
              break;
//            case CLIMATE_MODE_AUTO:
            default:
              mode = 3;
              break;
          }
          this->parent_->power_switch(true);
          command[2] = WRITE | SERVICE_ID_CLIMATE_MODE;
          command[4] = mode;
      }
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
  }

  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
    command[2] = WRITE | SERVICE_ID_CLIMATE_TARGET_TEMPERATURE;
    command[4] = this->target_temperature;
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
  }

  if (call.get_preset().has_value()) {
    this->preset = *call.get_preset();
    command[2] = 0;
    switch (this->preset.value()) {
      case climate::CLIMATE_PRESET_HOME:
        command[2] = WRITE | SERVICE_ID_CLIMATE_AIR_PURIFIER;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_AWAY:
        command[2] = WRITE | SERVICE_ID_CLIMATE_DEFROST;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_BOOST:
        command[2] = WRITE | SERVICE_ID_CLIMATE_BOOST;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_COMFORT:
        command[2] = WRITE | SERVICE_ID_CLIMATE_COMFORT;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_ECO:
        command[2] = WRITE | SERVICE_ID_CLIMATE_ECO;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_SLEEP:
        command[2] = WRITE | SERVICE_ID_CLIMATE_SLEEP;
        command[4] = 0x01;
      break;
      case climate::CLIMATE_PRESET_ACTIVITY:
        command[2] = WRITE | SERVICE_ID_CLIMATE_ACTIVITY;
        command[4] = 0x01;
      break;
    }
    if (command[2] != 0) {
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
    }
  }

  if (call.get_fan_mode().has_value()) {
    this->fan_mode = *call.get_fan_mode();
    command[2] = WRITE | SERVICE_ID_CLIMATE_FAN_SPEED;

    switch (this->fan_mode.value()) {
      case climate::CLIMATE_FAN_OFF:
        command[2] = WRITE | SERVICE_ID_CLIMATE_STATUS;
        command[4] = 0;
      break;
      case climate::CLIMATE_FAN_ON:
        command[2] = WRITE | SERVICE_ID_CLIMATE_STATUS;
        command[4] = 1;
      break;
      case climate::CLIMATE_FAN_AUTO:
        command[4] = 0;
      break;
      case climate::CLIMATE_FAN_LOW:
        command[4] = 3;
      break;
      case climate::CLIMATE_FAN_MEDIUM:
        command[4] = 6;
      break;
      case climate::CLIMATE_FAN_HIGH:
        command[4] = 15;
      break;
      case climate::CLIMATE_FAN_MIDDLE:
        command[4] = 12;
      break;
      case climate::CLIMATE_FAN_FOCUS:
        command[4] = 9;
      break;
      case climate::CLIMATE_FAN_DIFFUSE:
        command[4] = 10;
      break;
      case climate::CLIMATE_FAN_QUIET:
        command[4] = 1;
      break;
    }
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
  }

  if (call.get_swing_mode().has_value()) {
    this->swing_mode = *call.get_swing_mode();
    if (this->swing_mode == CLIMATE_SWING_VERTICAL) {
      command[2] = WRITE | SERVICE_ID_CLIMATE_SWING_VERTICAL;
      command[4] = 0x1;
    } else if (this->swing_mode == CLIMATE_SWING_HORIZONTAL) {
      command[2] = WRITE | SERVICE_ID_CLIMATE_SWING_HORIZONTAL;
      command[4] = 0x1;
    } else if (this->swing_mode == CLIMATE_SWING_BOTH) {
      command[2] = WRITE | SERVICE_ID_CLIMATE_SWING_VERTICAL;
      command[4] = 0x1;
      command[5] = this->parent_->checksum(command, 5);
      this->parent_->send_cmd(command, buffer, 6);
      command[2] = WRITE | SERVICE_ID_CLIMATE_SWING_HORIZONTAL;
      command[4] = 0x1;
    }
    command[5] = this->parent_->checksum(command, 5);
    this->parent_->send_cmd(command, buffer, 6);
  }
}

bool TaiXiaClimate::update_status_() {
  return false;
}

void TaiXiaClimate::update() {
  ESP_LOGD(TAG, "[%s] update()", this->get_name().c_str());
  // TODO: if the hub component is already polling, do we also need to include polling?
  //  We're already going to get on_status() at the hub's polling interval.
  auto result = this->update_status_();
  ESP_LOGD(TAG, "[%s] update_status result=%s", this->get_name().c_str(), result ? "true" : "false");
}

void TaiXiaClimate::set_supported_preset_modes(const std::set<climate::ClimatePreset> &modes) {
  this->traits_.set_supported_presets(modes);
}

void TaiXiaClimate::set_supported_swing_modes(const std::set<climate::ClimateSwingMode> &modes) {
  this->traits_.set_supported_swing_modes(modes);
  this->traits_.add_supported_swing_mode(climate::CLIMATE_SWING_OFF);       // Always available
}

void TaiXiaClimate::set_supported_fan_modes(const std::set<climate::ClimateFanMode> &modes) {
  this->traits_.set_supported_fan_modes(modes);
  this->traits_.add_supported_fan_mode(climate::CLIMATE_FAN_ON);   // Always available
  this->traits_.add_supported_fan_mode(climate::CLIMATE_FAN_OFF);  // Always available
}

void TaiXiaClimate::set_supported_modes(const std::set<climate::ClimateMode> &modes) {
  this->traits_.set_supported_modes(modes);
  this->traits_.add_supported_mode(climate::CLIMATE_MODE_OFF);   // Always available
  this->traits_.add_supported_mode(climate::CLIMATE_MODE_AUTO);  // Always available
}

climate::ClimateTraits TaiXiaClimate::traits() {
//    auto traits = climate::ClimateTraits();

//    traits.set_supports_action(true);

    this->traits_.set_supports_current_temperature(true);
    if (this->supports_cool_)
      this->traits_.add_supported_mode(CLIMATE_MODE_COOL);
    if (this->supports_heat_)
      this->traits_.add_supported_mode(CLIMATE_MODE_HEAT);
    if (this->supports_dry_)
      this->traits_.add_supported_mode(CLIMATE_MODE_DRY);
    if (this->supports_cool_ && this->supports_heat_)
      this->traits_.add_supported_mode(CLIMATE_MODE_HEAT_COOL);
    if (this->supports_fan_only_)
      this->traits_.add_supported_mode(CLIMATE_MODE_FAN_ONLY);

    if (this->fan_modes_ & 1 << CLIMATE_FAN_LOW)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_LOW);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_MEDIUM)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_MEDIUM);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_HIGH)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_HIGH);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_MIDDLE)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_MIDDLE);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_FOCUS)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_FOCUS);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_DIFFUSE)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_DIFFUSE);
    if (this->fan_modes_ & 1 << CLIMATE_FAN_QUIET)
      this->traits_.add_supported_fan_mode(CLIMATE_FAN_QUIET);

    if (this->swing_modes_ >= 1) {
      if (this->swing_modes_ & 1 << CLIMATE_SWING_VERTICAL)
        this->traits_.add_supported_swing_mode(CLIMATE_SWING_VERTICAL);
      if (this->swing_modes_ & 1 << CLIMATE_SWING_HORIZONTAL)
        this->traits_.add_supported_swing_mode(CLIMATE_SWING_HORIZONTAL);
      if (this->swing_modes_ & (1 << CLIMATE_SWING_VERTICAL | 1 << CLIMATE_SWING_HORIZONTAL))
        this->traits_.add_supported_swing_mode(CLIMATE_SWING_BOTH);
    }

    if (this->preset_modes_ >= 1) {
      this->traits_.set_supported_presets({
          climate::CLIMATE_PRESET_NONE,
      });
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_HOME)
        this->traits_.add_supported_preset(CLIMATE_PRESET_HOME);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_AWAY)
        this->traits_.add_supported_preset(CLIMATE_PRESET_AWAY);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_BOOST)
        this->traits_.add_supported_preset(CLIMATE_PRESET_BOOST);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_COMFORT)
        this->traits_.add_supported_preset(CLIMATE_PRESET_COMFORT);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_ECO)
        this->traits_.add_supported_preset(CLIMATE_PRESET_ECO);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_SLEEP)
        this->traits_.add_supported_preset(CLIMATE_PRESET_SLEEP);
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_ACTIVITY)
        this->traits_.add_supported_preset(CLIMATE_PRESET_ACTIVITY);

      if (this->preset_modes_ & 1 << CLIMATE_PRESET_AIR_DETECT)
        this->traits_.add_supported_custom_preset("Air Detect");
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_ANTI_MILDEW)
        this->traits_.add_supported_custom_preset("Anti Mildew");
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_SELF_CLEAN)
        this->traits_.add_supported_custom_preset("Self Clean");
      if (this->preset_modes_ & 1 << CLIMATE_PRESET_BODY_MOTION)
        this->traits_.add_supported_custom_preset("Body Motion");
    }

    this->traits_.set_visual_min_temperature(this->max_temp_);
    this->traits_.set_visual_max_temperature(this->min_temp_);
    this->traits_.set_visual_temperature_step(this->temp_step_);
    this->traits_.set_visual_target_temperature_step(this->temp_step_);
    this->traits_.set_visual_current_temperature_step(this->temp_step_);
    this->traits_.set_supports_two_point_target_temperature(false);

    return this->traits_;
}

void TaiXiaClimate::handle_response(std::vector<uint8_t> &response) {
  uint8_t i;

  ESP_LOGV(TAG, " handle_response %x %x %x %x %x %x %x %x %x", \
      response[0], response[1], response[2], response[3], \
      response[4], response[5], response[6], response[7], response[8]);

  for (i = 3; i < response[0] - 3; i+=3) {
    switch (i) {
      case SERVICE_ID_CLIMATE_MODE:
        if ((response[i + 1] != 0xFF) && (response[i + 2] != 0xFF)) {
          switch (response[i + 2]) {
            case 0:
              this->mode = CLIMATE_MODE_COOL;
              break;
            case 4:
              this->mode = CLIMATE_MODE_HEAT;
              break;
            case 1:
              this->mode = CLIMATE_MODE_DRY;
              break;
            case 2:
              this->mode = CLIMATE_MODE_FAN_ONLY;
              break;
            case 3:
            default:
              this->mode = CLIMATE_MODE_AUTO;
              break;
          }
        }
      break;
      case SERVICE_ID_CLIMATE_FAN_SPEED:
        if ((response[i + 1] != 0xFF) && (response[i + 2] != 0xFF)) {
          switch (response[i + 2]) {
            case 0:
              this->fan_mode = CLIMATE_FAN_AUTO;
            break;
            case 2:
            case 3:
              this->fan_mode = CLIMATE_FAN_LOW;
            break;
            case 4:
            case 5:
            case 6:
              this->fan_mode = CLIMATE_FAN_MEDIUM;
            break;
            case 13:
            case 14:
            case 15:
              this->fan_mode = CLIMATE_FAN_HIGH;
            break;
            case 11:
            case 12:
              this->fan_mode = CLIMATE_FAN_MIDDLE;
            break;
            case 7:
            case 8:
            case 9:
              this->fan_mode = CLIMATE_FAN_FOCUS;
            break;
            case 10:
              this->fan_mode = CLIMATE_FAN_DIFFUSE;
            break;
            case 1:
              this->fan_mode = CLIMATE_FAN_QUIET;
            break;
          }
        } else {
          this->fan_mode = CLIMATE_FAN_AUTO;
        }
      break;
    }
  }
  this->publish_state();
}

}  // namespace taixia
}  // namespace esphome
