import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor
from esphome.const import (
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    ICON_FLASH,
    ICON_CURRENT_AC,
    ICON_POWER,
    ICON_TIMELAPSE,
    UNIT_AMPERE,
    UNIT_VOLT,
    UNIT_WATT
)

from . import CONF_TBD_SMART_SHUNT_ID, TBDSmartShuntComponent

CONF_BATTERY_CURRENT = "battery_current"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_INSTANTANEOUS_POWER = "instantaneous_power"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TBD_SMART_SHUNT_ID): cv.use_id(
            TBDSmartShuntComponent
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_INSTANTANEOUS_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_POWER,
        ),
    }
)


def to_code(config):
    var = yield cg.get_variable(config[CONF_TBD_SMART_SHUNT_ID])

    if CONF_BATTERY_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))

    if CONF_BATTERY_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_CURRENT])
        cg.add(var.set_battery_current_sensor(sens))

    if CONF_INSTANTANEOUS_POWER in config:
        sens = yield sensor.new_sensor(config[CONF_INSTANTANEOUS_POWER])
        cg.add(var.set_instantaneous_power_sensor(sens))
