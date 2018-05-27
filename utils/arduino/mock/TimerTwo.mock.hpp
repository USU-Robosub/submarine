#ifndef TIMER_TWO_MOCK
#define TIMER_TWO_MOCK

namespace MsTimer2{
  extern int delay;
  extern bool isStopped;
  extern void (*func)();

  void start();
  void stop();
  void set(int delay, void (*func)());
  int $delay();
  void(*$attachedInterrupt())();
  bool $isStopped();
  void $reset();
}

#ifdef MOCK_INCLUDE_SOURCE

int MsTimer2::delay = -1;
bool MsTimer2::isStopped = false;
void (*MsTimer2::func)() = nullptr;

void MsTimer2::start(){
  MsTimer2::isStopped = false;
}

void MsTimer2::stop(){
  MsTimer2::isStopped = true;
}

void MsTimer2::set(int delay, void (*func)()){
  MsTimer2::delay = delay;
  MsTimer2::func = func;
  isStopped = true;
}

int MsTimer2::$delay(){
  return MsTimer2::delay;
}

void(*MsTimer2::$attachedInterrupt())() {
  return MsTimer2::func;
}

bool MsTimer2::$isStopped(){
  return MsTimer2::isStopped;
}

void MsTimer2::$reset(){
  MsTimer2::isStopped = false;
  MsTimer2::delay = -1;
  MsTimer2::func = nullptr;
}

#endif
#endif
