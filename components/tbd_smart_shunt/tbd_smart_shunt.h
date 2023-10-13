#pragma once

#include <esphome/core/component.h>
#include <esphome/core/application.h>
#include <esphome/components/sensor/sensor.h>
#include <esphome/components/uart/uart.h>

namespace esphome {
namespace tbd_smart_shunt {

class TBDSmartShuntComponent : public uart::UARTDevice, public Component {
 public:
  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) {
    battery_voltage_sensor_ = battery_voltage_sensor;
  }
  void set_battery_current_sensor(sensor::Sensor *battery_current_sensor) {
    battery_current_sensor_ = battery_current_sensor;
  }
  void set_instantaneous_power_sensor(sensor::Sensor *instantaneous_power_sensor) {
    instantaneous_power_sensor_ = instantaneous_power_sensor;
  }

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

protected:

  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
  sensor::Sensor *instantaneous_power_sensor_{nullptr};

  char buffer_[38];
  uint8_t index_{0};
  uint32_t last_transmission_{0};
};

}  // namespace tbd_smart_shunt
}  // namespace esphome
