#ifndef MATH_DISCRETE_CALCULUS
#define MATH_DISCRETE_CALCULUS

#include <algorithm>

namespace Math{
  struct Range{
    double min;
    double max;
  };

  double map(double value, Range oldRange, Range newRange);
  double bound(double value, Range range);
}

#endif
