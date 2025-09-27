import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    CONF_TYPE,
    ENTITY_CATEGORY_CONFIG
)
from .. import (
    taixia_ns,
    CONF_TAIXIA_ID,
    TaiXia,
    CONF_AIR_CONDITIONER,
    CONF_AIRPURIFIER,
    CONF_ELECTRIC_FAN,
    CONF_ERV,
    CONF_DEHUMIDIFIER,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]

ICON_BUTTON_POINTER = "mdi:button-pointer"
ICON_HELP_NETWORK_OUTLINE = "mdi:help-network-outline"

CONF_FILTER_CLEAN = "filter_clean"
CONF_ENERGY_RESET = "energy_reset"
CONF_GET_INFO = "get_info"

CLIMATE_TYPES = {
    CONF_ENERGY_RESET: 0x28,
    CONF_FILTER_CLEAN: 0x30
}

DEHUMIDIFIER_TYPES = {
    CONF_FILTER_CLEAN: 0x0B,
    CONF_ENERGY_RESET: 0x1D
}

AIRPURIFIER_TYPES = {
    CONF_ENERGY_RESET: 0x12
}

FAN_TYPES = {
    CONF_FILTER_CLEAN: 0x05
}

ERV_TYPES = {
    CONF_FILTER_CLEAN: 0x14,
}

COMMON_TYPES = {
    CONF_GET_INFO: 0x00
}

TYPES = CLIMATE_TYPES | DEHUMIDIFIER_TYPES | FAN_TYPES

TaiXiaButton = taixia_ns.class_("TaiXiaButton", button.Button, cg.Component)

TAIXIA_BUTTON_SCHEMA = button.button_schema(
    TaiXiaButton, icon=ICON_BUTTON_POINTER
).extend(cv.COMPONENT_SCHEMA)

TAIXIA_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TaiXiaButton),
        cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
        cv.Required(CONF_TYPE): cv.string,
    }
).extend(
    {
        cv.Required(CONF_GET_INFO): button.button_schema(
            TaiXiaButton,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_HELP_NETWORK_OUTLINE
        ).extend(cv.COMPONENT_SCHEMA)
    }
)

CONFIG_SCHEMA = TAIXIA_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): TAIXIA_BUTTON_SCHEMA for type in TYPES}
)

async def add_button(config, sa_type, service_id, sa_id):
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    if sa_type in config:
        conf = config[sa_type]
        var = await button.new_button(conf)
        await cg.register_component(var, conf)
        cg.add(getattr(taixia, f"set_{sa_type}_button")(var))
        cg.add(var.set_parent(taixia))
        cg.add(var.set_service_id(service_id))
        cg.add(var.set_sa_id(sa_id))

async def to_code(config):

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        sa_id = 1
        for sa_type, service_id in CLIMATE_TYPES.items():
            await add_button(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        sa_id = 4
        for sa_type, service_id in DEHUMIDIFIER_TYPES.items():
            await add_button(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_AIRPURIFIER:
        sa_id = 8
        for sa_type, service_id in AIRPURIFIER_TYPES.items():
            await add_button(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_ERV:
        sa_id = 14
        for sa_type, service_id in ERV_TYPES.items():
            await add_button(config, sa_type, service_id, sa_id)

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        sa_id = 15
        for sa_type, service_id in FAN_TYPES.items():
            await add_button(config, sa_type, service_id, sa_id)

    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    var = await button.new_button(config[CONF_GET_INFO])
    await cg.register_component(var, config[CONF_GET_INFO])
    cg.add(getattr(taixia, f"set_get_info_button")(var))
    cg.add(var.set_parent(taixia))
    cg.add(var.set_service_id(0x00))
    cg.add(var.set_sa_id(sa_id))
