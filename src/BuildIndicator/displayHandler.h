#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "arduino.h"
#include <Adafruit_SSD1306.h>


class DisplayHandler {
public:
  DisplayHandler();
  ~DisplayHandler();

  void begin();
  void handle();
  Adafruit_SSD1306& getDisplay();
  Adafruit_SSD1306& setMessage(String message);

private:

  const uint32_t DISPLAY_TIMEOUT = 60000;

  Adafruit_SSD1306 m_display;
  uint32_t m_lastActualize;
  bool m_startup;
};

#endif // DISPLAYHANDLER_H

