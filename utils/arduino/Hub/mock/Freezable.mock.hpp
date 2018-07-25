#ifndef FREEZABLE_MOCK
#define FREEZABLE_MOCK

#include <Freezable.hpp>

namespace Mock{
  class Freezable;
}

class Mock::Freezable : public ::Freezable{
public:
  Freezable(){}

  void freeze(){
    isFrozen = true;
    stateChanges++;
  }

  void unfreeze(){
    isFrozen = false;
    stateChanges++;
  }

  bool $isFrozen(){
    return isFrozen;
  }

  int $stateChanges(){
    return stateChanges;
  }

  void $makeUnfrozen(){
    isFrozen = false;
  }

private:
  int stateChanges = 0;
  bool isFrozen = true;
};

#endif
