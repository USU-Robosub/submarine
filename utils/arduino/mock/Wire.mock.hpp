#ifndef WIRE_MOCK
#define WIRE_MOCK


namespace Mock{
  class Wire{
  public:
    void begin() { }
    void beginTransmission(int address) {}
    void endTransmission() {}
    void send(int message) {}
    void requestFrom(int address, int length) {}
    int receive() {}
    int available() {}
  private:
  };
}

extern Mock::Wire Wire;

#ifdef MOCK_INCLUDE_SOURCE

Mock::Wire Wire;

#endif
#endif

