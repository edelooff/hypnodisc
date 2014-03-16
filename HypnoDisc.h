#include <StandardCplusplus.h>
#include <vector>
#include <Arduino.h>

class HypnoDisc {
  public:
    HypnoDisc(
        byte discSize,
        byte trailSize = 5,
        byte latchPin = 7,
        byte clockPin = 6,
        byte dataPin = 5);
    void begin();
    // Movement and display methods
    void addDot();
    void clockwiseDrop();
    void clockwiseSpin();
    void clockwiseWipe();
    void updateLights();
    // Query methods
    bool allDropped();
    bool empty();
    bool full();
    unsigned int dotsDropped();

  private:
    struct latch{
      latch(byte pin);
      ~latch();
      const byte pin;
    };
    const byte clockPin, dataPin, latchPin, pwmMaxLevel;
    byte pwmStep = 0;
    std::vector<byte> ledStates;
};
