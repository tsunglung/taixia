import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_LIGHT,
    CONF_POWER,
    CONF_TYPE,
    DEVICE_CLASS_SWITCH
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    TaiXia,
    CONF_AIR_CONDITIONER,
    CONF_AIRPURIFIER,
    CONF_ELECTRIC_FAN,
    CONF_DEHUMIDIFIER,
    CONF_ERV,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]

ICON_TOGGLE_SWITCH = "mdi:toggle-switch-variant"

CONF_IONS = "ions"
CONF_SLEEPY = "sleepy"
CONF_AIR_PURIFIER = "air_purifier"
CONF_SWING_VERTICAL = "swing_vertical"
CONF_SWING_HORIZONTAL = "swing_horizontal"
CONF_AIR_FLOW_AUTO = "air_flow_auto"
CONF_FILTER_NOTIFY = "filter_notify"
CONF_AIR_DETECT = "air_detect"
CONF_MILDEW_PROOF = "mildew_proof"
CONF_SUPER_MODE = "super_mode"
CONF_POWER_SAVING = "power_saving"
CONF_BEEPER = "beeper"
CONF_HUMIDITY_NOTIFY = "humidity_notify"
CONF_LOCK = "lock"
CONF_MOISTURIZE = "moisturize"
CONF_FILTER_RESET = "filter_reset"
CONF_PM25_DETECT = "pm25_detect"
CONF_FROST_WASH = "frost_wash"
CONF_SELF_CLEANING = "self_cleaning"

DEFAULT_ICON = "mdi:toggle-switch-variant"
ICONS = {
    CONF_IONS: "mdi:atom-variant",
    CONF_SLEEPY: "mdi:power-sleep",
    CONF_AIR_PURIFIER: "mdi:air-purifier",
    CONF_SWING_VERTICAL: "mdi:arrow-expand-vertical",
    CONF_SWING_HORIZONTAL: "mdi:arrow-expand-horizontal",
    CONF_AIR_FLOW_AUTO: "mdi:waves",
    CONF_FILTER_NOTIFY: "mdi:air-filter",
    CONF_AIR_DETECT: "mdi:hvac",
    CONF_MILDEW_PROOF: "mdi:weather-dust",
    CONF_SUPER_MODE: "mdi:lightning-bolt",
    CONF_POWER_SAVING: "mdi:leaf",
    CONF_BEEPER: "mdi:volume-high",
    CONF_HUMIDITY_NOTIFY: "mdi:water-percent",
    CONF_LOCK: "mdi:lock",
    CONF_FILTER_RESET: "filter-cog-outline",
    CONF_PM25_DETECT: "mdi:molecule",
    CONF_FROST_WASH: "mdi:snowflake-melt",
    CONF_SELF_CLEANING: "mdi:broom"
}

CLIMATE_TYPES = {
    CONF_SLEEPY: 0x05,
    CONF_AIR_PURIFIER: 0x08,
    CONF_SWING_VERTICAL: 0x0E,
    CONF_SWING_HORIZONTAL: 0x10,
    CONF_FILTER_NOTIFY: 0x12,
    CONF_AIR_DETECT: 0x16,
    CONF_MILDEW_PROOF: 0x17,
    CONF_SELF_CLEANING: 0x18,
    CONF_SUPER_MODE: 0x1A,
    CONF_POWER_SAVING: 0x1B,
    CONF_BEEPER: 0x1E,
    CONF_MOISTURIZE: 0x20,
    CONF_FROST_WASH: 0x3B
}

DEHUMIDIFIER_TYPES = {
    CONF_AIR_FLOW_AUTO: 0x08,
    CONF_FILTER_NOTIFY: 0x0B,
    CONF_LIGHT: 0x0C,
    CONF_AIR_PURIFIER: 0x0D,
    CONF_MILDEW_PROOF: 0x13,
    CONF_HUMIDITY_NOTIFY: 0x14,
    CONF_LOCK: 0x16,
    CONF_BEEPER: 0x18,
    CONF_PM25_DETECT: 0x29,
    CONF_FILTER_RESET: 0x52
}

AIRPURIFIER_TYPES = {
    CONF_IONS: 0x07,
    CONF_LOCK: 0x08
}

ERV_TYPES = {
    CONF_FILTER_RESET: 0x14
}

FAN_TYPES = {
    CONF_IONS: 0x06,
    CONF_LIGHT: 0x07,
}

COMMON_TYPES = {
    CONF_POWER: 0x00
}

TYPES = CLIMATE_TYPES | DEHUMIDIFIER_TYPES | FAN_TYPES

TaiXiaSwitch = taixia_ns.class_("TaiXiaSwitch", switch.Switch, cg.Component)

TAIXIA_SWITCH_SCHEMA = switch.switch_schema(
    TaiXiaSwitch, icon=ICON_TOGGLE_SWITCH, block_inverted=True, device_class=DEVICE_CLASS_SWITCH
).extend(cv.COMPONENT_SCHEMA)

TAIXIA_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TaiXiaSwitch),
        cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
        cv.Required(CONF_TYPE): cv.string,
        cv.Required(CONF_POWER): TAIXIA_SWITCH_SCHEMA
    }
)

CONFIG_SCHEMA = TAIXIA_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): TAIXIA_SWITCH_SCHEMA for type in TYPES}
)

async def add_switch(config, sa_type, service_id, sa_id):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if sa_type in config:
        conf = config[sa_type]
        conf['icon'] = ICONS.get(sa_type, DEFAULT_ICON)
        var = await switch.new_switch(conf)
        await cg.register_component(var, conf)
        cg.add(getattr(taixia, f"set_{sa_type}_switch")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(service_id))
        cg.add(var.set_sa_id(sa_id))
        cg.add(taixia.register_listener(var))

async def to_code(config):

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        sa_id = 1
        for sa_type, service_id in CLIMATE_TYPES.items():
            await add_switch(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        sa_id = 4
        for sa_type, service_id in DEHUMIDIFIER_TYPES.items():
            await add_switch(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_AIRPURIFIER:
        sa_id = 8
        for sa_type, service_id in AIRPURIFIER_TYPES.items():
            await add_switch(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_ERV:
        sa_id = 14
        for sa_type, service_id in ERV_TYPES.items():
            await add_switch(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        sa_id = 15
        for sa_type, service_id in FAN_TYPES.items():
            await add_switch(config, sa_type, service_id, sa_id)

    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    var = await switch.new_switch(config[CONF_POWER])
    await cg.register_component(var, config[CONF_POWER])
    cg.add(getattr(taixia, f"set_power_switch")(var))
    cg.add(var.set_parent(taixia))
    cg.add(var.set_service_id(0x00))
    cg.add(var.set_sa_id(sa_id))
    cg.add(taixia.register_listener(var))
