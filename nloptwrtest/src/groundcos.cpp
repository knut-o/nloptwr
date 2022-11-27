
#include "nloptwrtest/groundcos.h"
#include "nloptwrtest/groundif.h"
#include "nloptwrtest/xh.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

namespace opttest {

GroundCos::GroundCos(const XH &p_1, const XH &p_2, double d_x)
    : p1(p_1), p2(p_2), dx(d_x) {
  a = (p2.h + p1.h) * 0.5;
  b = (p1.h - p2.h) * 0.5;

  factor = (std::atan(1.0) * 4.0) /
           (p2.x - p1.x); // M_PI=3.14159265358979323846=(std::atan(1.0) * 4.0)

  dx = 0.01 / 3.14 * (p2.x - p1.x);
}

GroundCos::~GroundCos() {}

// virtual
double GroundCos::ground(double x) {
  return (a + b * cos(factor * (x - p1.x)));
}

// virtual
double GroundCos::getDx() { return dx; }

// virtual
int GroundCos::getId() { return 2; }

// virtual
GroundIf *GroundCos::clone() { return new GroundCos(*this); }

} // namespace opttest
