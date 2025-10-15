from esphome.components import binary_sensor
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_POWER,
    CONF_TYPE,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_RUNNING,
    DEVICE_CLASS_MOISTURE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_CHIP
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    TaiXia,
    CONF_DEHUMIDIFIER,
    CONF_ERV,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]

CONF_WATER_TANK_FULL = "water_tank_full"
CONF_FILTER_NOTIFY = "filter_notify"
CONF_SIDE_AIR_FLOW = "side_air_flow"
CONF_DEFROST = "defrost"
CONF_ODOURS = "odours"
CONF_FRONT_FILTER_NOTIFY = "front_filter_notify"
CONF_NOTIFY = "notify"

ICONS = {
    CONF_POWER: "mdi:chip",
    CONF_WATER_TANK_FULL: "mdi:cup-water",
    CONF_FILTER_NOTIFY: "mdi:air-filter",
    CONF_SIDE_AIR_FLOW: "mdi:waves",
    CONF_DEFROST: "mdi:snowflake-melt"
}

DEVICE_CLASS = {
    CONF_POWER: DEVICE_CLASS_POWER,
    CONF_WATER_TANK_FULL: DEVICE_CLASS_MOISTURE,
    CONF_FILTER_NOTIFY: DEVICE_CLASS_RUNNING,
    CONF_SIDE_AIR_FLOW: DEVICE_CLASS_RUNNING,
    CONF_DEFROST: DEVICE_CLASS_RUNNING
}

DEHUMIDIFIER_TYPES = {
    CONF_WATER_TANK_FULL: 0x0A,
    CONF_FILTER_NOTIFY: 0x0B,
    CONF_SIDE_AIR_FLOW: 0x0F,
    CONF_DEFROST: 0x11
}

ERV_TYPES = {
    CONF_FILTER_NOTIFY: 0x14,
    CONF_FRONT_FILTER_NOTIFY: 0x1C,
    CONF_NOTIFY: 0x1D,
}

COMMON_TYPES = {
    CONF_POWER: 0x00
}

TYPES = DEHUMIDIFIER_TYPES

TaiXiaBinarySensor = taixia_ns.class_("TaiXiaBinarySensor", binary_sensor.BinarySensor, cg.Component)

TAIXIA_BINARY_SENSOR_SCHEMA = binary_sensor.binary_sensor_schema(
    TaiXiaBinarySensor,
    icon=ICON_CHIP,
    device_class=DEVICE_CLASS_POWER,
    entity_category=ENTITY_CATEGORY_DIAGNOSTIC
).extend(cv.COMPONENT_SCHEMA)

TAIXIA_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TaiXiaBinarySensor),
        cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
        cv.Required(CONF_TYPE): cv.string,
        cv.Optional(CONF_POWER): TAIXIA_BINARY_SENSOR_SCHEMA
    }
)

CONFIG_SCHEMA = TAIXIA_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): TAIXIA_BINARY_SENSOR_SCHEMA for type in TYPES}
)

async def add_binay_sensor(config, sa_type, service_id, sa_id):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if sa_type in config:
        conf = config[sa_type]
        conf['icon'] = ICONS.get(sa_type, ICON_CHIP)
        conf['device_class'] = DEVICE_CLASS.get(
            sa_type, DEVICE_CLASS_RUNNING)
        var = await binary_sensor.new_binary_sensor(conf)
        await cg.register_component(var, conf)
        cg.add(getattr(taixia, f"set_{sa_type}_binary_sensor")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(service_id))
        cg.add(var.set_sa_id(sa_id))
        cg.add(taixia.register_listener(var))

async def to_code(config):

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        sa_id = 4
        for sa_type, service_id in DEHUMIDIFIER_TYPES.items():
            await add_binay_sensor(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_ERV:
        sa_id = 14
        for sa_type, service_id in ERV_TYPES.items():
            await add_binay_sensor(config, sa_type, service_id, sa_id)

    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if CONF_POWER in config:
        var = await binary_sensor.new_binary_sensor(config[CONF_POWER])
        await cg.register_component(var, config[CONF_POWER])
        cg.add(getattr(taixia, f"set_power_binary_sensor")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(0x00))
        cg.add(var.set_sa_id(sa_id))
        cg.add(taixia.register_listener(var))