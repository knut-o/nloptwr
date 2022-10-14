
#include "nloptwrtest/groundlin.h"
#include "nloptwrtest/groundif.h"
#include "nloptwrtest/xh.h"

namespace opttest {

GroundLin::GroundLin(const XH &p_1, const XH &p_2, double d_x)
    : p1(p_1), p2(p_2), dx(d_x) {
  b = (p2.h - p1.h) / (p2.x - p1.x);
  dx = 10000000.0;
}

GroundLin::~GroundLin() {}

// virtual
double GroundLin::ground(double x) { return (p1.h + b * (x - p1.x)); }

// virtual
double GroundLin::getDx() { return dx; }

// virtual
int GroundLin::getId() { return 1; }

// virtual
GroundIf *GroundLin::clone() { return new GroundLin(*this); }

} // namespace opttest
