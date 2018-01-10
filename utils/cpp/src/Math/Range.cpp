#include "Math/Range.h"

double Math::map(double value, Math::Range oldRange, Math::Range newRange){
  return (((value - oldRange.min) * (newRange.max - newRange.min)) / (oldRange.max - oldRange.min)) + newRange.min;
}

double Math::bound(double value, Math::Range range){
  return std::min(std::max(value, range.min), range.max);
}
