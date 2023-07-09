import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_TYPE
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    TaiXia,
    CONF_AIR_CONDITIONER,
    CONF_ELECTRIC_FAN,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]

ICON_NUMERIC = "mdi:numeric"

CONF_OFF_TIMER = "off_timer"
CONF_ON_TIMER = "on_timer"
CONF_VERTICAL_FAN_SPEED = "vertical_fan_speed"
CONF_HORIZONTAL_FAN_SPEED = "horizontal_fan_speed"


CLIMATE_TYPES = {
    CONF_ON_TIMER: (0x0B, 0, 1440, 1),
    CONF_OFF_TIMER: (0x0C, 0, 1440, 1),
    CONF_VERTICAL_FAN_SPEED: (0x0F, 0, 15, 1),
    CONF_HORIZONTAL_FAN_SPEED: (0x11, 0, 15, 1),
}

FAN_TYPES = {
    CONF_ON_TIMER: (0x0B, 0, 1440, 1),
    CONF_OFF_TIMER: (0x0C, 0, 1440, 1),
}

TYPES = FAN_TYPES | CLIMATE_TYPES

TaiXiaNumber = taixia_ns.class_("TaiXiaNumber", number.Number, cg.Component)

TAIXIA_NUMBER_SCHEMA = number.number_schema(
    TaiXiaNumber, icon=ICON_NUMERIC
).extend(cv.COMPONENT_SCHEMA)

TAIXIA_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TaiXiaNumber),
        cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
        cv.Required(CONF_TYPE): cv.string,
    }
)

CONFIG_SCHEMA = TAIXIA_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): TAIXIA_NUMBER_SCHEMA for type in TYPES}
)


async def add_number(config, type, service_id, sa_id, max, min, step):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if max <= min:
        print("max_value must be greater than min_value")

    if type in config:
        conf = config[type]
        var = await number.new_number(
            conf,
            max_value=max,
            min_value=min,
            step=step,
        )
        await cg.register_component(var, config)
        cg.add(getattr(taixia, f"set_{type}_number")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(service_id))
        cg.add(var.set_sa_id(sa_id))

async def to_code(config):

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        for type, (service_id, min, max, step) in CLIMATE_TYPES.items():
            await add_number(config, type, service_id, 1, max, min, step)

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        for type, (service_id, min, max, step) in FAN_TYPES.items():
            await add_number(config, type, service_id, 15, max, min, step)
