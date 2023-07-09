from esphome.components import fan
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_OUTPUT_ID, \
    CONF_MODE, CONF_SPEED, CONF_SPEED_COUNT
from .. import taixia_ns, CONF_TAIXIA_ID, TaiXia

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["taixia"]

CONF_OSCILLATIONT = "oscillation"

TaiXiaFan = taixia_ns.class_("TaiXiaFan", cg.Component, fan.Fan)

CONFIG_SCHEMA = cv.All(
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(TaiXiaFan),
            cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
            cv.Optional(CONF_OSCILLATIONT): cv.boolean,
            cv.Optional(CONF_SPEED): cv.boolean,
            cv.Optional(CONF_MODE, default=5): cv.int_range(min=0, max=5),
            cv.Optional(CONF_SPEED_COUNT, default=5): cv.int_range(min=1, max=16),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.has_at_least_one_key(CONF_SPEED, CONF_MODE),
)

async def to_code(config):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])

    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], taixia, config[CONF_SPEED_COUNT])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)

    if CONF_SPEED in config:
        cg.add(var.set_speed(config[CONF_SPEED]))
    if CONF_SPEED_COUNT in config:
        cg.add(var.set_speeds(config[CONF_SPEED_COUNT]))
    if CONF_OSCILLATIONT in config:
        cg.add(var.set_oscillation(config[CONF_OSCILLATIONT]))
    if CONF_MODE in config:
        cg.add(var.set_mode(config[CONF_MODE]))

    cg.add(taixia.register_listener(var))
    cg.add(var.set_taixia_parent(taixia))