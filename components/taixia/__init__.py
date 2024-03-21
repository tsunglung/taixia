import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@tsunglung"]

DEPENDENCIES = ["uart", "binary_sensor", "number", "switch", "text_sensor"]

MULTI_CONF = True

taixia_ns = cg.esphome_ns.namespace("taixia")
TaiXia = taixia_ns.class_("TaiXia", uart.UARTDevice, cg.Component)

CONF_TAIXIA_ID = "taixia_id"
CONF_SA_ID = "sa_id"
CONF_MAX_LENGTH = "max_length"
CONF_RESPONSE_TIME = "response_time"
CONF_AIR_CONDITIONER = "airconditioner"
CONF_AIRPURIFIER = "airpurifier"
CONF_DEHUMIDIFIER = "dehumidifier"
CONF_WASHING_MACHINE = "washingmachine"
CONF_ELECTRIC_FAN = "electricfan"

CONF_SUPPORTED_SA = [
    CONF_AIR_CONDITIONER,
    CONF_DEHUMIDIFIER,
#    CONF_WASHING_MACHINE,
    CONF_ELECTRIC_FAN
]

CONF_SUPPORTED_SA_ID = [1, 3, 4, 15]

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(TaiXia),
        cv.Optional(CONF_SA_ID): cv.int_range(min=1, max=255),
        cv.Optional(CONF_MAX_LENGTH, default=6): cv.int_range(min=6, max=100),
        cv.Optional(CONF_RESPONSE_TIME): cv.int_range(min=0, max=90000)
    }
)


async def to_code(config):
    cg.add_global(taixia_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_SA_ID in config:
        if config[CONF_SA_ID] not in CONF_SUPPORTED_SA_ID:
            raise ValueError(f"Not support SA ID {config[CONF_SA_ID]}")
        cg.add(var.set_sa_id(config[CONF_SA_ID]))

    if CONF_MAX_LENGTH in config:
        cg.add(var.set_max_length(config[CONF_MAX_LENGTH]))
    if CONF_RESPONSE_TIME in config:
        cg.add(var.set_response_time(config[CONF_RESPONSE_TIME]))