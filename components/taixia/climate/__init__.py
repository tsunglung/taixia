import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_MAX_TEMPERATURE,
    CONF_MIN_TEMPERATURE,
    CONF_TEMPERATURE_STEP,
    CONF_SUPPORTED_FAN_MODES,
    CONF_SUPPORTED_MODES,
    CONF_SUPPORTED_SWING_MODES,
    CONF_SUPPORTED_PRESETS
)
from esphome.components.climate import (
    ClimateFanMode,
    ClimatePreset,
    ClimateMode,
    ClimateSwingMode
)
from .. import taixia_ns, CONF_TAIXIA_ID, CONF_SA_ID, TaiXia

DEFAULT_MIN_TEMPERATURE = 16.0
DEFAULT_MAX_TEMPERATURE = 35.0
DEFAULT_TEMPERATURE_STEP = 1.0
CONF_SUPPORTED_HUMIDITY = "supported_humidity"

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["taixia"]

TaiXiaClimate = taixia_ns.class_("TaiXiaClimate", cg.PollingComponent, climate.Climate)

SUPPORTED_SWING_MODES_OPTIONS = {
    "OFF": ClimateSwingMode.CLIMATE_SWING_OFF,  # always available
    "VERTICAL": ClimateSwingMode.CLIMATE_SWING_VERTICAL,  # always available
    "HORIZONTAL": ClimateSwingMode.CLIMATE_SWING_HORIZONTAL,
    "BOTH": ClimateSwingMode.CLIMATE_SWING_BOTH,
}

SUPPORTED_CLIMATE_FAN_MODES_OPTIONS = {
    "ON": ClimateFanMode.CLIMATE_FAN_ON,  # always available
    "OFF": ClimateFanMode.CLIMATE_FAN_OFF,  # always available
    "AUTO": ClimateFanMode.CLIMATE_FAN_AUTO,
    "LOW": ClimateFanMode.CLIMATE_FAN_LOW,
    "MEDIUM": ClimateFanMode.CLIMATE_FAN_MEDIUM,
    "HIGH": ClimateFanMode.CLIMATE_FAN_HIGH,
    "MIDDLE": ClimateFanMode.CLIMATE_FAN_MIDDLE,
    "FOCUS": ClimateFanMode.CLIMATE_FAN_FOCUS,
    "DIFFUSE": ClimateFanMode.CLIMATE_FAN_DIFFUSE,
    "QUIET": ClimateFanMode.CLIMATE_FAN_QUIET,
}

SUPPORTED_CLIMATE_MODES_OPTIONS = {
    "OFF": ClimateMode.CLIMATE_MODE_OFF,  # always available
    "AUTO": ClimateMode.CLIMATE_MODE_AUTO,  # always available
    "COOL": ClimateMode.CLIMATE_MODE_COOL,
    "HEAT": ClimateMode.CLIMATE_MODE_HEAT,
    "DRY": ClimateMode.CLIMATE_MODE_DRY,
    "FAN_ONLY": ClimateMode.CLIMATE_MODE_FAN_ONLY,
}

SUPPORTED_CLIMATE_PRESET_OPTIONS = {
    "NONE": ClimatePreset.CLIMATE_PRESET_NONE,
    "ECO": ClimatePreset.CLIMATE_PRESET_ECO,
    "AWAY": ClimatePreset.CLIMATE_PRESET_AWAY,
    "BOOST": ClimatePreset.CLIMATE_PRESET_BOOST,
    "COMFORT": ClimatePreset.CLIMATE_PRESET_COMFORT,
    "HOME": ClimatePreset.CLIMATE_PRESET_HOME,
    "SLEEP": ClimatePreset.CLIMATE_PRESET_SLEEP,
    "ACTIVITY": ClimatePreset.CLIMATE_PRESET_ACTIVITY,
}

OPTIONS_SWING_MODES = [
    "OFF",
    "VERTICAL",
    "HORIZONTAL",
    "BOTH"
]

CONFIG_SCHEMA = cv.All(
    climate.climate_schema(TaiXiaClimate).extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(TaiXiaClimate),
            cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
            cv.Optional(CONF_SA_ID, default=1): cv.int_,
            cv.Optional(CONF_MAX_TEMPERATURE, default=DEFAULT_MAX_TEMPERATURE): cv.float_,
            cv.Optional(CONF_MIN_TEMPERATURE, default=DEFAULT_MIN_TEMPERATURE): cv.float_,
            cv.Optional(CONF_TEMPERATURE_STEP, default=DEFAULT_TEMPERATURE_STEP): cv.float_,
            cv.Optional(CONF_SUPPORTED_MODES): cv.ensure_list(
                cv.enum(SUPPORTED_CLIMATE_MODES_OPTIONS, upper=True)
            ),
            cv.Optional(CONF_SUPPORTED_FAN_MODES): cv.ensure_list(
                cv.enum(SUPPORTED_CLIMATE_FAN_MODES_OPTIONS, upper=True)
            ),
            cv.Optional(
                CONF_SUPPORTED_SWING_MODES,
            ): cv.ensure_list(cv.enum(SUPPORTED_SWING_MODES_OPTIONS, upper=True)),
            cv.Optional(CONF_SUPPORTED_PRESETS): cv.ensure_list(
                cv.enum(SUPPORTED_CLIMATE_PRESET_OPTIONS, upper=True)
            ),
            cv.Optional(CONF_SUPPORTED_HUMIDITY, default=False): bool
        }
    ).extend(cv.polling_component_schema("30s"))
)


def _validate_sa_id(config, sa_id):
    if sa_id not in [1, 14]:
        raise cv.Invalid("SA ID must be values 1 or 14")

    if sa_id == 14:
        if ((CONF_SUPPORTED_FAN_MODES in config) or
            (CONF_SUPPORTED_SWING_MODES in config) or
            (CONF_SUPPORTED_PRESETS in config)):
            raise cv.Invalid("There are some unnecessary configurations for ERV.")


async def to_code(config):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])

    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)

    if CONF_SA_ID in config:
        _validate_sa_id(config, config[CONF_SA_ID])
        cg.add(var.set_sa_id(config[CONF_SA_ID]))
    if CONF_MAX_TEMPERATURE in config:
        cg.add(var.set_max_temperature(config[CONF_MAX_TEMPERATURE]))
    if CONF_MIN_TEMPERATURE in config:
        cg.add(var.set_min_temperature(config[CONF_MIN_TEMPERATURE]))
    if CONF_TEMPERATURE_STEP in config:
        cg.add(var.set_temperature_step(config[CONF_TEMPERATURE_STEP]))
    if CONF_SUPPORTED_MODES in config:
        cg.add(var.set_supported_modes(config[CONF_SUPPORTED_MODES]))
    if CONF_SUPPORTED_FAN_MODES in config:
        cg.add(var.set_supported_fan_modes(config[CONF_SUPPORTED_FAN_MODES]))
    if CONF_SUPPORTED_SWING_MODES in config:
        cg.add(var.set_supported_swing_modes(config[CONF_SUPPORTED_SWING_MODES]))
    if CONF_SUPPORTED_PRESETS in config:
        cg.add(var.set_supported_preset_modes(config[CONF_SUPPORTED_PRESETS]))
    if CONF_SUPPORTED_HUMIDITY in config:
        cg.add(var.set_supported_humidity(config[CONF_SUPPORTED_HUMIDITY]))

    cg.add(taixia.register_listener(var))
    cg.add(var.set_taixia_parent(taixia))
