import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    CONF_MODEL,
    CONF_SERVICES,
    CONF_VERSION,
    ICON_CHIP,
    ICON_NEW_BOX
)

from .. import taixia_ns, CONF_TAIXIA_ID, TaiXia

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["taixia"]

AUTO_LOAD = ["text_sensor"]

CONF_SA_ID = "sa_id"
CONF_BRAND = "brand"

TaiXiaTextSensor = taixia_ns.class_("TaiXiaTextSensor", cg.Component)


CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TaiXiaTextSensor),
    cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
    cv.Optional(CONF_SA_ID): text_sensor.text_sensor_schema(
        icon=ICON_CHIP),
    cv.Optional(CONF_MODEL): text_sensor.text_sensor_schema(
        icon=ICON_CHIP),
    cv.Optional(CONF_BRAND): text_sensor.text_sensor_schema(
        icon=ICON_CHIP),
    cv.Optional(CONF_VERSION): text_sensor.text_sensor_schema(
        icon=ICON_NEW_BOX),
    cv.Optional(CONF_SERVICES): text_sensor.text_sensor_schema(
        icon=ICON_CHIP),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    cg.add(var.set_taixia_parent(taixia))
    for cfg in config:
        if isinstance(config[cfg], dict):
            sens1 = await text_sensor.new_text_sensor(config[cfg])
            cg.add(getattr(taixia, f"set_{cfg}_textsensor")(sens1))
