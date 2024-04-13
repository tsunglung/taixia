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
    CONF_AIRPURIFIER,
    CONF_DEHUMIDIFIER,
    CONF_ELECTRIC_FAN,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]

ICON_NUMERIC = "mdi:numeric"

CONF_SLEEP_TIMER = "sleep_timer"
CONF_OFF_TIMER = "off_timer"
CONF_ON_TIMER = "on_timer"
CONF_VERTICAL_FAN_SPEED = "vertical_fan_speed"
CONF_HORIZONTAL_FAN_SPEED = "horizontal_fan_speed"

CONF_OPERATING_TIME = "operating_time"
CONF_RELATIVE_HUMIDITY = "relative_humidity"
CONF_DEHUMIDIFIER_LEVEL = "dehumidifier_level"
CONF_DRY_LEVEL = "dry_level"
CONF_FAN_ANGLE = "fan_angle"
CONF_AIR_PURIFIER_LEVEL = "air_purifier"
CONF_FAN_LEVEL = "fan_level"
CONF_SOUND_MODE = "sound_mode"
CONF_HIGH_HUMIDITY_LEVEL = "high_humidity_level"
CONF_LIGHT_LEVEL = "light_level"

ICONS = {
    CONF_OFF_TIMER: "mdi:timer-off-outline",
    CONF_ON_TIMER: "mdi:timer-settings-outline",
    CONF_VERTICAL_FAN_SPEED: "mdi:heat-waves",
    CONF_HORIZONTAL_FAN_SPEED: "mdi:waves",
    CONF_OPERATING_TIME: "mdi:developer-board",
    CONF_RELATIVE_HUMIDITY: "mdi:water-percent",
    CONF_DEHUMIDIFIER_LEVEL: "mdi:water-percent",
    CONF_DRY_LEVEL: "mdi:tumble-dryer",
    CONF_FAN_ANGLE: "mdi:fan-chevron-up",
    CONF_AIR_PURIFIER_LEVEL: "mdi:air-purifier",
    CONF_FAN_LEVEL: "mdi:fan-speed-3",
    CONF_SOUND_MODE: "mdi:volume-source",
    CONF_HIGH_HUMIDITY_LEVEL: "mdi:water-percent",
    CONF_LIGHT_LEVEL: "mdi:brightness-6"
}

AIRPURIFIER_TYPES = {
    CONF_ON_TIMER: (0x02, 0, 24, 1),
    CONF_OFF_TIMER: (0x03, 0, 24, 1),
}

CLIMATE_TYPES = {
    CONF_SLEEP_TIMER: (0x06, 0, 1440, 1),
    CONF_ON_TIMER: (0x0B, 0, 1440, 1),
    CONF_OFF_TIMER: (0x0C, 0, 1440, 1),
    CONF_VERTICAL_FAN_SPEED: (0x0F, 0, 15, 1),
    CONF_HORIZONTAL_FAN_SPEED: (0x11, 0, 15, 1),
}

DEHUMIDIFIER_TYPES = {
    CONF_OPERATING_TIME: (0x02, 0, 99, 1),
    CONF_RELATIVE_HUMIDITY: (0x03, 40, 70, 1),
    CONF_DEHUMIDIFIER_LEVEL: (0x04, 0, 15, 1),
    CONF_DRY_LEVEL: (0x05, 0, 15, 1),
    CONF_FAN_ANGLE: (0x09, 0, 15, 1),
    CONF_AIR_PURIFIER_LEVEL: (0x0D, 0, 1, 1),
    CONF_FAN_LEVEL: (0x0E, 0, 15, 1),
    CONF_SOUND_MODE: (0x10, 0, 2, 1),
    CONF_HIGH_HUMIDITY_LEVEL: (0x15, 0, 99, 1),
    CONF_LIGHT_LEVEL: (0x27, 0, 3, 1),
}

FAN_TYPES = {
    CONF_ON_TIMER: (0x0B, 0, 1440, 1),
    CONF_OFF_TIMER: (0x0C, 0, 1440, 1),
}

TYPES = FAN_TYPES | DEHUMIDIFIER_TYPES | CLIMATE_TYPES | AIRPURIFIER_TYPES

TaiXiaNumber = taixia_ns.class_("TaiXiaNumber", number.Number, cg.Component)

TAIXIA_NUMBER_SCHEMA = number.number_schema(
    TaiXiaNumber, icon=ICON_NUMERIC
).extend(cv.COMPONENT_SCHEMA).extend(
    {
        cv.Optional(CONF_MIN_VALUE): cv.int_range(min=0, max=1440),
        cv.Optional(CONF_MAX_VALUE): cv.int_range(min=1, max=1440),
    }
)

TAIXIA_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TaiXiaNumber),
        cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
        cv.Required(CONF_TYPE): cv.string,
    }
)

CONFIG_SCHEMA = TAIXIA_COMPONENT_SCHEMA.extend(
    {cv.Optional(config_type): TAIXIA_NUMBER_SCHEMA for config_type in TYPES}
)


async def add_number(config, config_type, service_id, sa_id, max, min, step):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if max <= min:
        print("max_value must be greater than min_value")

    if config_type in config:
        conf = config[config_type]
        conf['icon'] = ICONS.get(config_type, ICON_NUMERIC)
        var = await number.new_number(
            conf,
            max_value=max if CONF_MAX_VALUE not in conf else conf[CONF_MAX_VALUE],
            min_value=min if CONF_MIN_VALUE not in conf else conf[CONF_MIN_VALUE],
            step=step,
        )
        await cg.register_component(var, config)
        cg.add(getattr(taixia, f"set_{config_type}_number")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(service_id))
        cg.add(var.set_sa_id(sa_id))
        cg.add(taixia.register_listener(var))

async def to_code(config):

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        for config_type, (service_id, min, max, step) in CLIMATE_TYPES.items():
            await add_number(config, config_type, service_id, 1, max, min, step)

    if config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        for config_type, (service_id, min, max, step) in DEHUMIDIFIER_TYPES.items():
            await add_number(config, config_type, service_id, 4, max, min, step)

    if config[CONF_TYPE] == CONF_AIRPURIFIER:
        for config_type, (service_id, min, max, step) in AIRPURIFIER_TYPES.items():
            await add_number(config, config_type, service_id, 8, max, min, step)

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        for config_type, (service_id, min, max, step) in FAN_TYPES.items():
            await add_number(config, config_type, service_id, 15, max, min, step)
