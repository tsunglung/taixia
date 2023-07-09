from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_ID,
    CONF_POWER,
    DEVICE_CLASS_POWER,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_CHIP
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    TaiXia
)

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["taixia"]

TaiXiaBinarySensor = taixia_ns.class_(
    "TaiXiaBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TaiXiaBinarySensor),
    cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
    cv.Required(CONF_POWER): binary_sensor.binary_sensor_schema(
        icon=ICON_CHIP,
        device_class=DEVICE_CLASS_POWER,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
     )
}).extend(cv.polling_component_schema('60s'))

async def to_code(config):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    sens1 = await binary_sensor.new_binary_sensor(config[CONF_POWER])
    cg.add(var.set_power_sensor(sens1))

    cg.add(taixia.register_listener(var))
    cg.add(var.set_taixia_parent(taixia))
