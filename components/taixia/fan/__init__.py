from esphome.components import fan
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components.fan import validate_preset_modes
from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_MODE,
    CONF_PRESET_MODES,
    CONF_SPEED,
    CONF_SPEED_COUNT,
    CONF_TYPE
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    CONF_ELECTRIC_FAN,
    CONF_DEHUMIDIFIER,
    TaiXia
)

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["taixia"]

CONF_OSCILLATIONT = "oscillation"
CONF_FAN_MODES = "fan_modes"
CONF_DEHUMIDIFIER_MODES = "dehumidifier_modes"

SUPPORTED_FAN_MODES_OPTIONS = {
    "mode 1": 0,
    "mode 2": 1,
    "mode 3": 2,
    "mode 4": 3,
    "mode 5": 4,
}
SUPPORTED_DEHUMIDIFIER_MODES_OPTIONS = {
    "auto": 0,
    "normal": 1,
    "away": 2,
    "boost": 3,
    "baby": 4,
    "defrost": 5,
    "fan": 6,
    "comfort": 7,
    "home": 8,
    "eco": 9
}

TaiXiaFan = taixia_ns.class_("TaiXiaFan", cg.PollingComponent, fan.Fan)

CONFIG_SCHEMA = cv.All(
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(TaiXiaFan),
            cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
            cv.Optional(CONF_TYPE): cv.string,
            cv.Optional(CONF_OSCILLATIONT): cv.boolean,
            cv.Optional(CONF_SPEED): cv.boolean,
            cv.Optional(CONF_MODE, default=5): cv.int_range(min=0, max=5),
            cv.Optional(CONF_SPEED_COUNT, default=5): cv.int_range(min=1, max=16),
            cv.Optional(CONF_PRESET_MODES): validate_preset_modes,
        }
    ).extend(cv.COMPONENT_SCHEMA).extend(cv.polling_component_schema("30s")),
    cv.has_at_least_one_key(CONF_SPEED, CONF_MODE),
)

async def to_code(config):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])

    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], taixia, config[CONF_SPEED_COUNT])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)

    if CONF_TYPE in config:
        if config[CONF_TYPE] == CONF_DEHUMIDIFIER:
            cg.add(var.set_sa_id(4))
        if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
            cg.add(var.set_sa_id(15))
    else:
        cg.add(var.set_sa_id(15))
    if CONF_SPEED in config:
        cg.add(var.set_speed(config[CONF_SPEED]))
    if CONF_SPEED_COUNT in config:
        cg.add(var.set_speeds(config[CONF_SPEED_COUNT]))
    if CONF_OSCILLATIONT in config:
        cg.add(var.set_oscillation(config[CONF_OSCILLATIONT]))
    if CONF_MODE in config:
        cg.add(var.set_mode(config[CONF_MODE]))
    if CONF_PRESET_MODES in config:
        cg.add(var.set_preset_modes(config[CONF_PRESET_MODES]))

    cg.add(taixia.register_listener(var))
    cg.add(var.set_taixia_parent(taixia))