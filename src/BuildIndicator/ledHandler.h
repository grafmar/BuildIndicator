#ifndef LEDHANDLER_H
#define LEDHANDLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "color.h"

class LedHandler {
public:
  LedHandler();
  ~LedHandler();

  void begin();
  void setLedColor(uint8_t led, Color color);
  void show();

private:

  Adafruit_NeoPixel m_ledStrip;
};

#endif // LEDHANDLER_H

