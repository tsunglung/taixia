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
CONF_AIR_CONDITIONER = "airconditioner"
CONF_AIRPURIFIER = "airpurifier"
CONF_DEHUMIDIFIER = "dehumidifier"
CONF_WASHING_MACHINE = "washingmachine"
CONF_ELECTRIC_FAN = "electricfan"

CONF_SUPPORTED_SA = [
    CONF_AIR_CONDITIONER,
#    CONF_DEHUMIDIFIER,
#    CONF_WASHING_MACHINE,
    CONF_ELECTRIC_FAN
]
CONF_MAX_LENGTH = "max_length"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(TaiXia),
        cv.Optional(CONF_MAX_LENGTH, default=6): cv.int_range(min=6, max=100),
    }
)


async def to_code(config):
    cg.add_global(taixia_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_MAX_LENGTH in config:
        cg.add(var.set_max_length(config[CONF_MAX_LENGTH]))