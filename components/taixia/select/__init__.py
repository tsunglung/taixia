import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    CONF_ID,
    CONF_TYPE,
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    taixia_ns,
    TaiXia,
    CONF_TAIXIA_ID,
    CONF_AIR_CONDITIONER,
    CONF_AIRPURIFIER,
    CONF_DEHUMIDIFIER,
    CONF_WASHING_MACHINE,
    CONF_ELECTRIC_FAN,
    CONF_SUPPORTED_SA
)

DEPENDENCIES = ["taixia"]
CODEOWNERS = ["@tsunglung"]

AirConditionerSelect = taixia_ns.class_("AirConditionerSelect", select.Select, cg.Component)
DehumidifierSelect = taixia_ns.class_("DehumidifierSelect", select.Select, cg.Component)
WashingMachineSelect = taixia_ns.class_("WashingMachineSelect", select.Select, cg.Component)
AirPurifierSelect = taixia_ns.class_("AirPurifierSelect", select.Select, cg.Component)
ElectricFanSelect = taixia_ns.class_("ElectricFanSelect", select.Select, cg.Component)

CONF_FUZZY_MODE = "fuzzy_mode"
CONF_DISPLAY_MODE = "display_mode"
CONF_MOTION_DETECT = "motion_detect"

CONF_OPERATING_PROGRAM = "operating_program"
CONF_AIR_PURIFIER = "air_purifier"

CONF_WASH_PROGRAM = "wash_program"
CONF_WASH_OTHER_FUNCTION = "wash_other_function"
CONF_WASH_MODE = "wash_mode"
CONF_WARM_WATER_PROGRAM = "warm_water_program"

OPTIONS_FUZZY_MODE = {
    "comfort": 0,
    "too cold": 1,
    "too hot": 2,
    "off": 3,
    "on": 4
}

OPTIONS_DISPLAY_MODE= {
    "light": 0,
    "dark": 1,
    "off": 2,
    "all off": 3
}

OPTIONS_MOTION_DETECT = {
    "off": 0,
    "to people": 1,
    "not to people": 2,
    "auto": 3
}

OPTIONS_WASH_PROGRAM = {
    "Standard": 0,
    "Soft": 1,
    "Strong": 2,
    "Shirts": 3,
    "Blankets": 4,
    "Premium": 5,
    "Wool": 6,
    "User-defined": 7,
    "Soak": 8,
    "Dry": 9,
    "Quick Wash": 10,
    "Tank wash":11,
    "Warm Water Wash":12
}

OPTIONS_WASH_OTHER_FUNCTION = {
    "Ozone Sterilization": 0,
    "Night Lighting": 1,
    "Ion Sterilization and Deodorization": 2,
    "Child Safety Mode": 3,
    "Door Lock":4
}

OPTIONS_WASH_MODE = {
    "Standard": 0,
    "Over Flow": 1,
    "Spray": 2
}

OPTIONS_WARM_WATER_PROGRAM = {
    "Standard": 0,
    "Sterilization": 1,
    "Deodorization": 2,
    "Large": 3,
    "Small": 4
}

OPTIONS_OPERATING_PROGRAM = {
    "auto": 0,
    "config": 1,
    "continuous": 2,
    "dry": 3,
    "air purifier": 4,
    "defrost": 5,
    "fan": 6,
    "comfort": 7,
    "low humidity": 8
}

OPTIONS_AIR_PURFIFIER = {
    "off": 0,
    "1": 1,
    "2": 2,
    "3": 3,
    "4": 4,
    "5": 5,
    "6": 6,
    "7": 7,
    "8": 8,
    "9": 9,
    "10": 10,
    "11": 11,
    "12": 12,
    "13": 13,
    "14": 14,
    "15": 15
}

OPTIONS_FAN_OPERATING_PROGRAM = {
    "Mode 1": 0,
    "Mode 2": 1,
    "Mode 3": 2,
    "Mode 4": 3,
    "Mode 5": 4
}

OPTIONS_PURIFIER_OPERATING_PROGRAM = {
    "Auot": 0,
    "Mute": 1,
    "Low": 2,
    "Standard": 3,
    "High": 4,
    "Super": 5
}

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_AIR_CONDITIONER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AirConditionerSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_FUZZY_MODE): select.select_schema(
                    AirConditionerSelect
                ),
                cv.Optional(CONF_DISPLAY_MODE): select.select_schema(
                    AirConditionerSelect
                ),
                cv.Optional(CONF_MOTION_DETECT): select.select_schema(
                    AirConditionerSelect
                ),
            }
        ),
        CONF_AIRPURIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AirPurifierSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    AirPurifierSelect
                ),
            }
        ),
        CONF_DEHUMIDIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(DehumidifierSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    DehumidifierSelect
                ),
                cv.Optional(CONF_AIR_PURIFIER): select.select_schema(
                    DehumidifierSelect
                ),
            }
        ),
        CONF_WASHING_MACHINE: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(WashingMachineSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_WASH_PROGRAM): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
                cv.Optional(CONF_WASH_OTHER_FUNCTION): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
                cv.Optional(CONF_WASH_MODE): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
                cv.Optional(CONF_WARM_WATER_PROGRAM): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
            }
        ),
        CONF_ELECTRIC_FAN: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(ElectricFanSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    ElectricFanSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                ),
            }
        ),
    },
    key=CONF_TYPE,
    lower=True,
    space="_",
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise (f"SA TYPE {config[CONF_TYPE]} is not supported yet")
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        cg.add(var.set_sa_id(0x01))
        if CONF_FUZZY_MODE in config:
            options_map = OPTIONS_FUZZY_MODE
            sel = await select.new_select(config[CONF_FUZZY_MODE],
                    options=list(options_map.keys()))
            cg.add(var.set_fuzzy_mode_select(sel))
            cg.add(sel.set_service_id(0x07))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_DISPLAY_MODE in config:
            options_map = OPTIONS_DISPLAY_MODE
            sel = await select.new_select(config[CONF_DISPLAY_MODE],
                    options=list(options_map.keys()))
            cg.add(var.set_display_mode_select(sel))
            cg.add(sel.set_service_id(0x1F))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_MOTION_DETECT in config:
            options_map = OPTIONS_MOTION_DETECT
            sel = await select.new_select(config[CONF_MOTION_DETECT],
                    options=list(options_map.keys()))
            cg.add(var.set_motion_detect_select(sel))
            cg.add(sel.set_service_id(0x19))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    elif config[CONF_TYPE] == CONF_WASHING_MACHINE:
        cg.add(var.set_sa_id(0x03))
        if CONF_WASH_PROGRAM in config:
            options_map = OPTIONS_WASH_PROGRAM
            sel = await select.new_select(config[CONF_WASH_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_wash_program_select(sel))
            cg.add(sel.set_service_id(0x02))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WASH_OTHER_FUNCTION in config:
            options_map = OPTIONS_WASH_OTHER_FUNCTION
            sel = await select.new_select(config[CONF_WASH_OTHER_FUNCTION],
                    options=list(options_map.keys()))
            cg.add(var.set_wash_other_function_select(sel))
            cg.add(sel.set_service_id(0x16))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WASH_MODE in config:
            options_map = OPTIONS_WASH_MODE
            sel = await select.new_select(config[CONF_WASH_MODE],
                    options=list(options_map.keys()))
            cg.add(sel.set_service_id(0x17))
            cg.add(sel.set_wash_mode_select(sel))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WARM_WATER_PROGRAM in config:
            options_map = OPTIONS_WARM_WATER_PROGRAM
            sel = await select.new_select(config[CONF_WARM_WATER_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_warm_water_program_select(sel))
            cg.add(sel.set_service_id(0x18))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    elif config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        cg.add(var.set_sa_id(0x04))
        if CONF_OPERATING_PROGRAM in config:
            options_map = OPTIONS_OPERATING_PROGRAM
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_AIR_PURIFIER in config:
            options_map = OPTIONS_AIR_PURFIFIER
            sel = await select.new_select(config[CONF_AIR_PURIFIER],
                    options=list(options_map.keys()))
            cg.add(var.set_air_purfifier_select(sel))
            cg.add(sel.set_service_id(0x0D))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    elif config[CONF_TYPE] == CONF_AIRPURIFIER:
        cg.add(var.set_sa_id(0x08))
        if CONF_OPERATING_PROGRAM in config:
            options_map = OPTIONS_PURIFIER_OPERATING_PROGRAM
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        cg.add(var.set_sa_id(0x0F))
        if CONF_OPERATING_PROGRAM in config:
            options_map = OPTIONS_FAN_OPERATING_PROGRAM
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))
