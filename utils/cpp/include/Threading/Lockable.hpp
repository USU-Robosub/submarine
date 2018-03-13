#ifndef THREADING_LOCKABLE
#define THREADING_LOCKABLE

namespace Threading{
  class Lockable;
}

class Threading::Lockable{
public:
  virtual void lock() = 0;
  virtual void unlock() = 0;
};

#endif
