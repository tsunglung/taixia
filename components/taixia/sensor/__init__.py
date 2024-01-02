import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_COMMAND,
    CONF_CUSTOM,
    CONF_HUMIDITY,
    CONF_LAMBDA,
    CONF_SENSORS,
    CONF_TYPE,
    CONF_TEMPERATURE,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_POWER,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_CHIP,
    ICON_CURRENT_AC,
    ICON_FLASH,
    ICON_THERMOMETER,
    ICON_TIMER,
    ICON_WATER_PERCENT,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_HOUR,
    UNIT_PERCENT,
    UNIT_KILOWATT_HOURS,
    UNIT_VOLT,
    UNIT_WATT
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

CODEOWNERS = ["@tsunglung"]

Air_Conditioner = taixia_ns.class_("AirConditionerSensor", cg.PollingComponent)
Dehumidifier = taixia_ns.class_("DehumidifierSensor", cg.PollingComponent)
Washing_Machine = taixia_ns.class_("WashingMachineSensor", cg.PollingComponent)
AirPurifier = taixia_ns.class_("AirPurifierSensor", cg.PollingComponent)
ElectricFan = taixia_ns.class_("ElectricFanSensor", cg.PollingComponent)
TaiXiaCustom = taixia_ns.class_("TaiXiaCustomSensor", cg.PollingComponent)
TaiXiaCustomSub = taixia_ns.class_("TaiXiaCustomSubSensor", cg.PollingComponent)


CONF_TEMPERATURE_INDOOR = "temperature_indoor"
CONF_HUMIDITY_INDOOR = "humidity_indoor"
CONF_TEMPERATURE_OUTDOOR = "temperature_outdoor"
CONF_HUMIDITY_OUTDOOR = "humidity_outdoor"
CONF_OPERATING_CURRENT = "operating_current"
CONF_OPERATING_VOLTAGE = "operating_voltage"
CONF_OPERATING_WATT = "operating_watt"
CONF_ENERGY_CONSUMPTON = "energy_consumption"
CONF_OPERATING_HOURS = "operating_hours"
CONF_ERROR_CODE = "error_code"
CONF_FILITER_CLEAN_HOURS = "filiter_clean_hours"

CONF_WASH_LEFT_COUNT = "wash_left_count"
CONF_WASH_LEFT_HOURS = "wash_left_hours"
CONF_SPIN_LEFT_HOURS = "spin_left_hours"
CONF_SOAK_LEFT_HOURS = "soak_left_hours"
CONF_DRY_LEFT_HOURS = "dry_left_hours"
CONF_TANK_CLEAN_LEFT_HOURS = "tank_clean_left_hours"
CONF_DRY_CLEAN_LEFT_HOURS = "dry_clean_left_hours"
CONF_TOTAL_LEFT_HOURS = "total_left_hours"
CONF_APPOINT_LEFT_HOURS = "appoint_left_hours"

CONF_WATER_FULL = "water_full"
CONF_FILITER_CLEAN = "filiter_clean"
CONF_SIDE_AIR_VENT = "side_air_vent"
CONF_DEFROST = "defrost"

CONF_AIR_QUALITY = "air_quality"
ICON_THOUGHT_BUBBLE = "mdi:thought-bubble"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_AIR_CONDITIONER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Air_Conditioner),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_TEMPERATURE_INDOOR): sensor.sensor_schema(
                    unit_of_measurement=UNIT_CELSIUS,
                    icon=ICON_THERMOMETER,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_HUMIDITY_INDOOR): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_WATER_PERCENT,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_HUMIDITY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_TEMPERATURE_OUTDOOR): sensor.sensor_schema(
                    unit_of_measurement=UNIT_CELSIUS,
                    icon=ICON_THERMOMETER,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_HUMIDITY_OUTDOOR): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_WATER_PERCENT,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_HUMIDITY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_CURRENT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    icon=ICON_FLASH,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_WATT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_WATT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ENERGY_CONSUMPTON): sensor.sensor_schema(
                    unit_of_measurement=UNIT_KILOWATT_HOURS,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_ENERGY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ERROR_CODE): sensor.sensor_schema(
                    icon=ICON_CHIP,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_FILITER_CLEAN_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                )
            }
        ).extend(cv.polling_component_schema('60s')),
        CONF_WASHING_MACHINE: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Washing_Machine),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_WASH_LEFT_COUNT): sensor.sensor_schema(
                    icon=ICON_CHIP,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_WASH_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_SPIN_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_SOAK_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_DRY_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_TANK_CLEAN_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_DRY_CLEAN_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_TOTAL_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_APPOINT_LEFT_HOURS): sensor.sensor_schema(
                    unit_of_measurement=UNIT_HOUR,
                    icon=ICON_TIMER,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_DURATION,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_CURRENT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    icon=ICON_FLASH,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_WATT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_WATT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ENERGY_CONSUMPTON): sensor.sensor_schema(
                    unit_of_measurement=UNIT_KILOWATT_HOURS,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_ENERGY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
            }
        ).extend(cv.polling_component_schema('60s')),
        CONF_DEHUMIDIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Dehumidifier),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_WATER_FULL): sensor.sensor_schema(
                    icon=ICON_WATER_PERCENT,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_FILITER_CLEAN): sensor.sensor_schema(
                    icon=ICON_CHIP,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_SIDE_AIR_VENT): sensor.sensor_schema(
                    icon=ICON_CHIP,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_DEFROST): sensor.sensor_schema(
                    icon=ICON_CHIP,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_OPERATING_CURRENT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    icon=ICON_FLASH,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_WATT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_WATT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ENERGY_CONSUMPTON): sensor.sensor_schema(
                    unit_of_measurement=UNIT_KILOWATT_HOURS,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_ENERGY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
            }
        ).extend(cv.polling_component_schema('60s')),
        CONF_AIRPURIFIER: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AirPurifier),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_AIR_QUALITY): sensor.sensor_schema(
                    icon=ICON_THOUGHT_BUBBLE,
                    device_class=DEVICE_CLASS_EMPTY,
                    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                ),
                cv.Optional(CONF_OPERATING_CURRENT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    icon=ICON_FLASH,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_WATT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_WATT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ENERGY_CONSUMPTON): sensor.sensor_schema(
                    unit_of_measurement=UNIT_KILOWATT_HOURS,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_ENERGY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
            }
        ).extend(cv.polling_component_schema('60s')),
        CONF_ELECTRIC_FAN: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(ElectricFan),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_WATER_PERCENT,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_HUMIDITY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_CELSIUS,
                    icon=ICON_THERMOMETER,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_CURRENT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PERCENT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    icon=ICON_FLASH,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_OPERATING_WATT): sensor.sensor_schema(
                    unit_of_measurement=UNIT_WATT,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_POWER,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                cv.Optional(CONF_ENERGY_CONSUMPTON): sensor.sensor_schema(
                    unit_of_measurement=UNIT_KILOWATT_HOURS,
                    icon=ICON_CURRENT_AC,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_ENERGY,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
            }
        ).extend(cv.polling_component_schema('60s')),
        CONF_CUSTOM: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(TaiXiaCustom),
                cv.GenerateID(CONF_TAIXIA_ID): cv.use_id(TaiXia),
                cv.Optional(CONF_COMMAND): cv.uint16_t,
                cv.Optional(CONF_SENSORS): cv.ensure_list(
                    sensor.sensor_schema().extend(
                        {
                            cv.GenerateID(): cv.declare_id(TaiXiaCustomSub),
                            cv.Required(CONF_LAMBDA): cv.lambda_,
                        }
                    )
                ),
            }
        ).extend(cv.polling_component_schema('60s')),
    },
    key=CONF_TYPE,
    lower=True,
    space="_",
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if config[CONF_TYPE] not in CONF_SUPPORTED_SA:
        raise ("SA TYPE is not supported yet")

    if config[CONF_TYPE] == CONF_AIR_CONDITIONER:
        cg.add(var.set_sa_id(0x01))
        if CONF_TEMPERATURE_INDOOR in config:
            sens = await sensor.new_sensor(config[CONF_TEMPERATURE_INDOOR])
            cg.add(var.set_temperature_indoor_sensor(sens))
        if CONF_HUMIDITY_INDOOR in config:
            sens = await sensor.new_sensor(config[CONF_HUMIDITY_INDOOR])
            cg.add(var.set_humidity_indoor_sensor(sens))
        if CONF_TEMPERATURE_OUTDOOR in config:
            sens = await sensor.new_sensor(config[CONF_TEMPERATURE_OUTDOOR])
            cg.add(var.set_temperature_outdoor_sensor(sens))
        if CONF_HUMIDITY_OUTDOOR in config:
            sens = await sensor.new_sensor(config[CONF_HUMIDITY_OUTDOOR])
            cg.add(var.set_humidity_outdoor_sensor(sens))
        if CONF_OPERATING_CURRENT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_CURRENT])
            cg.add(var.set_operating_current_sensor(sens))
        if CONF_OPERATING_VOLTAGE in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_VOLTAGE])
            cg.add(var.set_operating_voltage_sensor(sens))
        if CONF_OPERATING_WATT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_WATT])
            cg.add(var.set_operating_watt_sensor(sens))
        if CONF_ENERGY_CONSUMPTON in config:
            sens = await sensor.new_sensor(config[CONF_ENERGY_CONSUMPTON])
            cg.add(var.set_energy_consumption_sensor(sens))
        if CONF_OPERATING_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_HOURS])
            cg.add(var.set_operating_hours_sensor(sens))

    elif config[CONF_TYPE] == CONF_WASHING_MACHINE:
        cg.add(var.set_sa_id(0x03))
        if CONF_WASH_LEFT_COUNT in config:
            sens = await sensor.new_sensor(config[CONF_WASH_LEFT_COUNT])
            cg.add(var.set_wash_left_count_sensor(sens))
        if CONF_WASH_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_WASH_LEFT_HOURS])
            cg.add(var.set_wash_left_hours_sensor(sens))
        if CONF_SPIN_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_SPIN_LEFT_HOURS])
            cg.add(var.set_spin_left_hours_sensor(sens))
        if CONF_SOAK_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_SOAK_LEFT_HOURS])
            cg.add(var.set_soak_left_hours_sensor(sens))
        if CONF_DRY_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_DRY_LEFT_HOURS])
            cg.add(var.set_dry_left_hours_sensor(sens))
        if CONF_TANK_CLEAN_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_TANK_CLEAN_LEFT_HOURS])
            cg.add(var.set_tank_clean_left_hours_sensor(sens))
        if CONF_DRY_CLEAN_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_DRY_CLEAN_LEFT_HOURS])
            cg.add(var.set_dry_clean_left_hours_sensor(sens))
        if CONF_TOTAL_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_TOTAL_LEFT_HOURS])
            cg.add(var.set_total_left_hours_sensor(sens))
        if CONF_APPOINT_LEFT_HOURS in config:
            sens = await sensor.new_sensor(config[CONF_APPOINT_LEFT_HOURS])
            cg.add(var.set_appoint_left_hours_sensor(sens))
        if CONF_OPERATING_CURRENT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_CURRENT])
            cg.add(var.set_operating_current_sensor(sens))
        if CONF_OPERATING_VOLTAGE in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_VOLTAGE])
            cg.add(var.set_operating_voltage_sensor(sens))
        if CONF_OPERATING_WATT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_WATT])
            cg.add(var.set_operating_watt_sensor(sens))
        if CONF_ENERGY_CONSUMPTON in config:
            sens = await sensor.new_sensor(config[CONF_ENERGY_CONSUMPTON])
            cg.add(var.set_energy_consumption_sensor(sens))

    elif config[CONF_TYPE] == CONF_DEHUMIDIFIER:
        cg.add(var.set_sa_id(0x04))
        if CONF_WATER_FULL in config:
            sens = await sensor.new_sensor(config[CONF_WATER_FULL])
            cg.add(var.set_appoint_left_hours_sensor(sens))
        if CONF_FILITER_CLEAN in config:
            sens = await sensor.new_sensor(config[CONF_FILITER_CLEAN])
            cg.add(var.set_filiter_clean_sensor(sens))
        if CONF_SIDE_AIR_VENT in config:
            sens = await sensor.new_sensor(config[CONF_SIDE_AIR_VENT])
            cg.add(var.set_side_air_vent_sensor(sens))
        if CONF_DEFROST in config:
            sens = await sensor.new_sensor(config[CONF_DEFROST])
            cg.add(var.set_defrost_sensor(sens))
        if CONF_OPERATING_CURRENT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_CURRENT])
            cg.add(var.set_operating_current_sensor(sens))
        if CONF_OPERATING_VOLTAGE in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_VOLTAGE])
            cg.add(var.set_operating_voltage_sensor(sens))
        if CONF_OPERATING_WATT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_WATT])
            cg.add(var.set_operating_watt_sensor(sens))
        if CONF_ENERGY_CONSUMPTON in config:
            sens = await sensor.new_sensor(config[CONF_ENERGY_CONSUMPTON])
            cg.add(var.set_energy_consumption_sensor(sens))

    if config[CONF_TYPE] == CONF_AIRPURIFIER:
        cg.add(var.set_sa_id(0x08))
        if CONF_AIR_QUALITY in config:
            sens = await sensor.new_sensor(config[CONF_AIR_QUALITY])
            cg.add(var.set_air_quality_sensor(sens))
        if CONF_OPERATING_CURRENT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_CURRENT])
            cg.add(var.set_operating_current_sensor(sens))
        if CONF_OPERATING_VOLTAGE in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_VOLTAGE])
            cg.add(var.set_operating_voltage_sensor(sens))
        if CONF_OPERATING_WATT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_WATT])
            cg.add(var.set_operating_watt_sensor(sens))
        if CONF_ENERGY_CONSUMPTON in config:
            sens = await sensor.new_sensor(config[CONF_ENERGY_CONSUMPTON])
            cg.add(var.set_energy_consumption_sensor(sens))

    if config[CONF_TYPE] == CONF_ELECTRIC_FAN:
        cg.add(var.set_sa_id(0x0F))
        if CONF_HUMIDITY in config:
            sens = await sensor.new_sensor(config[CONF_HUMIDITY])
            cg.add(var.set_humidity_sensor(sens))
        if CONF_TEMPERATURE in config:
            sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
            cg.add(var.set_temperature_sensor(sens))
        if CONF_OPERATING_CURRENT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_CURRENT])
            cg.add(var.set_operating_current_sensor(sens))
        if CONF_OPERATING_VOLTAGE in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_VOLTAGE])
            cg.add(var.set_operating_voltage_sensor(sens))
        if CONF_OPERATING_WATT in config:
            sens = await sensor.new_sensor(config[CONF_OPERATING_WATT])
            cg.add(var.set_operating_watt_sensor(sens))
        if CONF_ENERGY_CONSUMPTON in config:
            sens = await sensor.new_sensor(config[CONF_ENERGY_CONSUMPTON])
            cg.add(var.set_energy_consumption_sensor(sens))

    elif config[CONF_TYPE] == CONF_CUSTOM:
        if CONF_COMMAND in config:
            cg.add(var.set_sa_id(config[CONF_COMMAND]))
        sensors = []
        for conf in config[CONF_SENSORS]:
            sens = await sensor.new_sensor(conf)
            lambda_ = await cg.process_lambda(
                conf[CONF_LAMBDA],
                [(cg.std_vector.template(cg.uint8), "x")],
                return_type=cg.float_,
            )
            cg.add(sens.set_message_parser(lambda_))
            sensors.append(sens)
        cg.add(var.set_sensors(sensors))

    taixia = await cg.get_variable(config[CONF_TAIXIA_ID])
    cg.add(taixia.register_listener(var))
    cg.add(var.set_taixia_parent(taixia))