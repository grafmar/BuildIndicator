#include "ledHandler.h"
#include "BuildIndicator.h"



LedHandler::LedHandler() :
  m_ledStrip(NUM_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800)
{
}

LedHandler::~LedHandler() {
}

void LedHandler::begin() {
  m_ledStrip.begin();
  //m_ledStrip.setBrightness(50);
  m_ledStrip.show();

  //int red = m_ledStrip.Color(255, 0, 0);
  //int green = m_ledStrip.Color(0, 255, 0);
  //int white = m_ledStrip.Color(255, 255, 255);
  //int blue = m_ledStrip.Color(0, 0, 255);
  //int pink = m_ledStrip.Color(255, 0, 255);
  //int dark = m_ledStrip.Color(0, 0, 0);

  //m_ledStrip.setPixelColor(0, blue);
  //m_ledStrip.setPixelColor(1, blue);
  //m_ledStrip.setPixelColor(2, green);
  //m_ledStrip.setPixelColor(3, green);
  //m_ledStrip.setPixelColor(4, green);
  //m_ledStrip.setPixelColor(5, red);
  //m_ledStrip.setPixelColor(6, green);
  //m_ledStrip.setPixelColor(7, green);
  //m_ledStrip.setPixelColor(8, pink);
  //m_ledStrip.setPixelColor(9, pink);
  //m_ledStrip.setPixelColor(10, green);
  //m_ledStrip.setPixelColor(11, red);
  //m_ledStrip.setPixelColor(12, green);
  //m_ledStrip.setPixelColor(13, red);
  //m_ledStrip.setPixelColor(14, green);
  //m_ledStrip.setPixelColor(15, green);
  //m_ledStrip.show();
}

void LedHandler::setLedColor(uint8_t led, Color color) {
  uint8_t row = led % NUM_OF_PIXEL_ROWS;
  uint8_t col = led / NUM_OF_PIXEL_ROWS;
  uint8_t ledNum = col * NUM_OF_PIXEL_ROWS + (NUM_OF_PIXEL_ROWS - row - 1);
  m_ledStrip.setPixelColor(ledNum, color.r, color.g, color.b);
  Serial.printf("setLedColor(): %d = %d, %d %d)\n", ledNum, color.r, color.g, color.b);
}

void LedHandler::show() {
  m_ledStrip.show();
}

