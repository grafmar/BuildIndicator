#ifndef COLOR_H
#define COLOR_H

struct Color {
  Color(uint8_t rr=0, uint8_t gg=0, uint8_t bb=0) :
    r(rr),
    g(gg),
    b(bb)
  {}

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#endif // COLOR_H

