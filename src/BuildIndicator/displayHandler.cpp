#include "displayHandler.h"
#include "BuildIndicator.h"
#include <Wire.h>

DisplayHandler::DisplayHandler() :
  m_display(OLED_RESET),
  m_lastActualize(0),
  m_startup(true)
{
}

DisplayHandler::~DisplayHandler() {
}



void DisplayHandler::begin() {
  // OLED Display
  Wire.begin(OLED_SDA, OLED_SCK);             // Initialize I2C for OLED Display (GPIO0 and GPIO2)
  m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  m_lastActualize = millis();
}

void DisplayHandler::handle() {
  if ((millis() - m_lastActualize) > DISPLAY_TIMEOUT) {
    m_lastActualize = millis();
    m_startup = false;
    m_display.clearDisplay();
    m_display.display();
  }
}

Adafruit_SSD1306 & DisplayHandler::getDisplay() {
  return m_display;
}

Adafruit_SSD1306 & DisplayHandler::setMessage(String message) {
  if (!m_startup) {
    m_lastActualize = millis();
    m_display.clearDisplay();
    m_display.setCursor(0, 0);
    m_display.setTextSize(2);
    m_display.println(message);
    m_display.display();
  }
}

