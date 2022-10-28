/*
  This file is part of the Arduino_APDS9960 library.
  Copyright (c) 2019 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "APDS9960.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"


bool APDS9960_begin() {
    
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

  // Disable everything
  if (!setENABLE(0x00)) return false;       // write(ENABLE,val)
  if (!setWTIME(0xFF)) return false;
  if (!setGPULSE(0x8F)) return false; // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse
  if (!setPPULSE(0x8F)) return false; // 16us, 16 pulses // default is: 0x40 = 8us, 1 pulse
  if (!setGestureIntEnable(true)) return false;
  if (!setGestureMode(true)) return false;
  if (!enablePower()) return false;
  if (!enableWait()) return false;
  // set ADC integration time to 10 ms
  if (!setATIME(256 - (10 / 2.78))) return false;
  // set ADC gain 4x (0x00 => 1x, 0x01 => 4x, 0x02 => 16x, 0x03 => 64x)
  if (!setCONTROL(0x02)) return false;
  delay(10);
  // enable power
  if (!enablePower()) return false;
  return true;
}

void APDS9960_end() {
  // Disable everything
  setENABLE(0x00);

  _gestureEnabled = false;

}

void setGestureSensitivity(uint8_t sensitivity) {
  if (sensitivity > 100) sensitivity = 100;
  _gestureSensitivity = 100 - sensitivity;
}

bool setGestureIntEnable(bool en) {
    uint8_t r;
    if (!getGCONF4(&r)) return false;
    if (en) {
      r |= 0b00000010;
    } else {
      r &= 0b11111101;
    }
    return setGCONF4(r);
}

bool setGestureMode(bool en)
{
    uint8_t r;
    if (!getGCONF4(&r)) return false;
    if (en) {
      r |= 0b00000001;
    } else {
      r &= 0b11111110;
    }
    return setGCONF4(r);
}

bool enablePower() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b00000001) != 0) return true;
  r |= 0b00000001;
  return setENABLE(r);
}

bool disablePower() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b00000001) == 0) return true;
  r &= 0b11111110;
  return setENABLE(r);
}


bool enableWait() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b00001000) != 0) return true;
  r |= 0b00001000;
  return setENABLE(r);
}

bool disableWait() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b00001000) == 0) return true;
  r &= 0b11110111;
  return setENABLE(r);
}

bool enableGesture() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b01000000) != 0) {
    _gestureEnabled = true;
    return true;
  }
  r |= 0b01000000;
  bool res = setENABLE(r);
  _gestureEnabled = res;
  return res;
}

bool disableGesture() {
  uint8_t r;
  if (!getENABLE(&r)) return false;
  if ((r & 0b01000000) == 0) {
    _gestureEnabled = false;
    return true;
  }
  r &= 0b10111111;
  bool res = setENABLE(r);
  _gestureEnabled = !res; // (res == true) if successfully disabled
  return res;
}

#define APDS9960_ADDR 0x39

bool write(uint8_t val) {
  i2c_write_blocking(i2c_default, ADDRESS, val, 1, true);
  return true;
}

bool write(uint8_t reg, uint8_t val) {
    i2c_write_blocking(i2c_default, reg, val, 1, true);
    return true;
}

bool read(uint8_t reg, uint8_t *val) {
    i2c_read_blocking(i2c_default, reg, val, 2, false);

  return true;
}



int gestureFIFOAvailable() {
  uint8_t r;
  if (!getGSTATUS(&r)) return -1;
  if ((r & 0x01) == 0x00) return -2;
  if (!getGFLVL(&r)) return -3;
  return r;
}

int handleGesture() {
  const int gestureThreshold = 30;
  while (true) {
    int available = gestureFIFOAvailable();
    if (available <= 0) return 0;

    uint8_t fifo_data[128];
    uint8_t bytes_read = readGFIFO_U(fifo_data, available * 4);
    if (bytes_read == 0) return 0;

    for (int i = 0; i+3 < bytes_read; i+=4) {
      uint8_t u,d,l,r;
      u = fifo_data[i];
      d = fifo_data[i+1];
      l = fifo_data[i+2];
      r = fifo_data[i+3];
      // Serial.print(u);
      // Serial.print(",");
      // Serial.print(d);
      // Serial.print(",");
      // Serial.print(l);
      // Serial.print(",");
      // Serial.println(r);

      if (u<gestureThreshold && d<gestureThreshold && l<gestureThreshold && r<gestureThreshold) {
        _gestureIn = true;
        if (_gestureDirInX != 0 || _gestureDirInY != 0) {
          int totalX = _gestureDirInX - _gestureDirectionX;
          int totalY = _gestureDirInY - _gestureDirectionY;
          // Serial.print("OUT ");
          // Serial.print(totalX);
          // Serial.print(",");
          // Serial.println(totalY);
          if (totalX < -_gestureSensitivity) { _detectedGesture = GESTURE_LEFT; }
          if (totalX > _gestureSensitivity) { _detectedGesture = GESTURE_RIGHT; }
          if (totalY < -_gestureSensitivity) { _detectedGesture = GESTURE_DOWN; }
          if (totalY > _gestureSensitivity) { _detectedGesture = GESTURE_UP; }
          _gestureDirectionX = 0;
          _gestureDirectionY = 0;
          _gestureDirInX = 0;
          _gestureDirInY = 0;
        }
        continue;
      }

      _gestureDirectionX = r - l;
      _gestureDirectionY = u - d;
      if (_gestureIn) {
        _gestureIn = false;
        _gestureDirInX = _gestureDirectionX;
        _gestureDirInY = _gestureDirectionY;
        // Serial.print("IN ");
        // Serial.print(_gestureDirInX);
        // Serial.print(",");
        // Serial.print(_gestureDirInY);
        // Serial.print(" ");
      }
    }
  }
}

int gestureAvailable() {
  if (!_gestureEnabled) enableGesture();

    if (digitalRead(_intPin) != LOW) {
      return 0;
    }
  

  handleGesture();
  if (_proximityEnabled) {
    setGestureMode(false);
  }
  return (_detectedGesture == GESTURE_NONE) ? 0 : 1;
}

int readGesture() {
  int gesture = _detectedGesture;

  _detectedGesture = GESTURE_NONE;

  return gesture;
}


#if defined(APDS9960_INT_PIN)
APDS9960 APDS(APDS9960_WIRE_INSTANCE, APDS9960_INT_PIN);
#elif defined(ARDUINO_ARDUINO_NANO33BLE)
APDS9960 APDS(Wire1, PIN_INT_APDS);
#else
APDS9960 APDS(Wire, -1);
#endif
