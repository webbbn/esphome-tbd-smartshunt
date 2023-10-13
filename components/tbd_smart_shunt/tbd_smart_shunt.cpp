#include <limits>

#include "tbd_smart_shunt.h"

namespace esphome {
namespace tbd_smart_shunt {

static const char *const TAG = "tbd_smart_shunt";

// Convert a hexidecimal ASCII character to it's numeric value.
static uint8_t hex_char_to_int(char v) {
  // The values have already been verified, so there are only two possibilities
  if (v <= '9') {
    return v - '0';
  }
  return v - 'A' + 10;
}

static bool is_ascii_number(char v) {
  return (v >= '0') && (v <= '9');
}
static bool is_ascii_hex_number(char v) {
  return is_ascii_number(v) || ((v >= 'A') && (v <= 'Z'));
}

static bool hex_to_float(float &ret, const char *p) {
  uint8_t *rp = reinterpret_cast<uint8_t*>(&ret);
  // First verify that all the digits are valid
  for (uint8_t i = 0; i < 8; ++i) {
    if (!is_ascii_hex_number(p[i])) {
      return false;
    }
  }
  rp[0] = (hex_char_to_int(p[0]) << 4) + hex_char_to_int(p[1]);
  rp[1] = (hex_char_to_int(p[2]) << 4) + hex_char_to_int(p[3]);
  rp[2] = (hex_char_to_int(p[4]) << 4) + hex_char_to_int(p[5]);
  rp[3] = (hex_char_to_int(p[6]) << 4) + hex_char_to_int(p[7]);
  return true;
}

void TBDSmartShuntComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "TBDSmartShunt:");
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Current", battery_current_sensor_);
  LOG_SENSOR("  ", "Instantaneous power", instantaneous_power_sensor_);

  check_uart_settings(115200);
}

void TBDSmartShuntComponent::loop() {
  const uint32_t now = millis();
  if ((now - last_transmission_) >= 2000) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago.");
    index_ = 0;
    last_transmission_ = now;
  }

  if (!available())
    return;

  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if ((index_ == 0) && (c != ':')) {
      continue;
    }
    buffer_[index_++] = c;
    if (index_ == 38) {
      index_ = 0;
      float voltage, current;
      if (!hex_to_float(voltage, buffer_ + 11) || !hex_to_float(current, buffer_ + 19)) {
        ESP_LOGE(TAG, "Error parsing TBD Shunt data:");
        continue;
      }
      float power = voltage * current;
      battery_voltage_sensor_->publish_state(voltage);
      battery_current_sensor_->publish_state(current);
      instantaneous_power_sensor_->publish_state(power);
    }
  }
}

}  // namespace tbd_smart_shunt
}  // namespace esphome
