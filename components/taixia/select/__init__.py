import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    CONF_ID,
    CONF_OPTIONS,
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
CONF_SOUND = "sound"

CONF_WASH_PROGRAM = "wash_program"
CONF_WASH_OTHER_FUNCTION = "wash_other_function"
CONF_WASH_MODE = "wash_mode"
CONF_WARM_WATER_PROGRAM = "warm_water_program"

DEFAULT_ICON = "mdi:format-list-bulleted"
ICONS = {
    CONF_FUZZY_MODE: "mdi:motion-sensor",
    CONF_DISPLAY_MODE: "mdi:overscan",
    CONF_MOTION_DETECT: "mdi:motion-sensor",
    CONF_OPERATING_PROGRAM: "mdi:state-machine",
    CONF_AIR_PURIFIER: "mdi:air-purifier",
    CONF_SOUND: "mdi:volume-source",
    CONF_WASH_PROGRAM: "mdi:washing-machine",
    CONF_WASH_OTHER_FUNCTION: "mdi:state-machine",
    CONF_WASH_MODE: "mdi:dishwasher",
    CONF_WARM_WATER_PROGRAM: "mdi:thermometer-water"
}

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
    "normal": 1,
    "away": 2,
    "boost": 3,
    "baby": 4,
    "sleep": 5,
    "fan": 6,
    "comfort": 7,
    "home": 8,
    "eco": 9
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

OPTIONS_SOUND = {
    "Off": 0,
    "Key Sound": 1,
    "Full and Key sounds": 2
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
                    AirConditionerSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_FUZZY_MODE, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_DISPLAY_MODE): select.select_schema(
                    AirConditionerSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_DISPLAY_MODE, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_MOTION_DETECT): select.select_schema(
                    AirConditionerSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_MOTION_DETECT, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                )
            }
        ),
        CONF_AIRPURIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AirPurifierSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    AirPurifierSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_OPERATING_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                )
            }
        ),
        CONF_DEHUMIDIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(DehumidifierSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    DehumidifierSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_OPERATING_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_AIR_PURIFIER): select.select_schema(
                    DehumidifierSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_OPERATING_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_SOUND): select.select_schema(
                    DehumidifierSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_OPERATING_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                )
            }
        ),
        CONF_WASHING_MACHINE: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(WashingMachineSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_WASH_PROGRAM): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_WASH_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_WASH_OTHER_FUNCTION): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_WASH_OTHER_FUNCTION, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_WASH_MODE): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_WASH_MODE, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                ),
                cv.Optional(CONF_WARM_WATER_PROGRAM): select.select_schema(
                    WashingMachineSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_WARM_WATER_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                )
            }
        ),
        CONF_ELECTRIC_FAN: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(ElectricFanSelect),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_OPERATING_PROGRAM): select.select_schema(
                    ElectricFanSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICONS.get(CONF_OPERATING_PROGRAM, DEFAULT_ICON)
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS): cv.All(
                            cv.ensure_list(cv.string_strict), cv.Length(min=1)
                        )
                    }
                )
            }
        ),
    },
    key=CONF_TYPE,
    lower=True,
    space="_",
)


def get_options(options, all_options):
    new_options = {}
    for option in options:
        if option in list(all_options.keys()):
            new_options[option] = all_options[option]
    if len(new_options) < 1:
        return all_options
    return new_options

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise (f"SA TYPE {config[CONF_TYPE]} is not supported yet")
    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])

    sel = None
    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        cg.add(var.set_sa_id(0x01))
        if CONF_FUZZY_MODE in config:
            options_map = get_options(config[CONF_FUZZY_MODE].get(
                CONF_OPTIONS, {}), OPTIONS_FUZZY_MODE)
            sel = await select.new_select(config[CONF_FUZZY_MODE],
                    options=list(options_map.keys()))
            cg.add(var.set_fuzzy_mode_select(sel))
            cg.add(sel.set_service_id(0x07))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_DISPLAY_MODE in config:
            options_map = get_options(config[CONF_DISPLAY_MODE].get(
                CONF_OPTIONS, {}), OPTIONS_DISPLAY_MODE)
            sel = await select.new_select(config[CONF_DISPLAY_MODE],
                    options=list(options_map.keys()))
            cg.add(var.set_display_mode_select(sel))
            cg.add(sel.set_service_id(0x1F))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_MOTION_DETECT in config:
            options_map = get_options(config[CONF_MOTION_DETECT].get(
                CONF_OPTIONS, {}), OPTIONS_MOTION_DETECT)
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
            options_map = get_options(config[CONF_WASH_PROGRAM].get(
                CONF_OPTIONS, {}), OPTIONS_WASH_PROGRAM)
            sel = await select.new_select(config[CONF_WASH_PROGRAM],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_wash_program_select(sel))
            cg.add(sel.set_service_id(0x02))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WASH_OTHER_FUNCTION in config:
            options_map = get_options(config[CONF_WASH_OTHER_FUNCTION].get(
                CONF_OPTIONS, {}), OPTIONS_WASH_OTHER_FUNCTION)
            sel = await select.new_select(config[CONF_WASH_OTHER_FUNCTION],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_wash_other_function_select(sel))
            cg.add(sel.set_service_id(0x16))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WASH_MODE in config:
            options_map = get_options(config[OPTIONS_WASH_MODE].get(
                CONF_OPTIONS, {}), CONF_WASH_MODE)
            sel = await select.new_select(config[CONF_WASH_MODE],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(sel.set_service_id(0x17))
            cg.add(sel.set_wash_mode_select(sel))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_WARM_WATER_PROGRAM in config:
            options_map = get_options(config[OPTIONS_WARM_WATER_PROGRAM].get(
                CONF_OPTIONS, {}), CONF_WARM_WATER_PROGRAM)
            sel = await select.new_select(config[CONF_WARM_WATER_PROGRAM],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_warm_water_program_select(sel))
            cg.add(sel.set_service_id(0x18))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    elif config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        cg.add(var.set_sa_id(0x04))
        if CONF_OPERATING_PROGRAM in config:
            options_map = get_options(config[CONF_OPERATING_PROGRAM].get(
                CONF_OPTIONS, {}), OPTIONS_OPERATING_PROGRAM)
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=list(options_map.keys()))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

        if CONF_AIR_PURIFIER in config:
            options_map = get_options(config[CONF_AIR_PURIFIER].get(
                CONF_OPTIONS, {}), OPTIONS_AIR_PURFIFIER)
            sel = await select.new_select(config[CONF_AIR_PURIFIER],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_air_purfifier_select(sel))
            cg.add(sel.set_service_id(0x0D))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    elif config[CONF_TYPE] == CONF_AIRPURIFIER:
        cg.add(var.set_sa_id(0x08))
        if CONF_OPERATING_PROGRAM in config:
            options_map = get_options(config[CONF_OPERATING_PROGRAM].get(
                CONF_OPTIONS, {}), OPTIONS_PURIFIER_OPERATING_PROGRAM)
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        cg.add(var.set_sa_id(0x0F))
        if CONF_OPERATING_PROGRAM in config:
            options_map = get_options(config[CONF_OPERATING_PROGRAM].get(
                CONF_OPTIONS, {}), OPTIONS_FAN_OPERATING_PROGRAM)
            sel = await select.new_select(config[CONF_OPERATING_PROGRAM],
                    options=config.get(
                        CONF_OPTIONS, list(options_map.keys())))
            cg.add(var.set_operating_program_select(sel))
            cg.add(sel.set_service_id(0x01))
            cg.add(sel.set_select_mappings(list(options_map.values())))
            cg.add(taixia.register_listener(sel))
            cg.add(sel.set_taixia_parent(taixia))
