#include <StandardCplusplus.h>
#include <vector>
#include <Arduino.h>

class HypnoDisc {
  public:
    HypnoDisc(
        byte discSize,
        byte pwmLevels = 5,
        byte latchPin = 7,
        byte clockPin = 6,
        byte dataPin = 5);
    void begin();
    // Movement and display methods
    void addLight();
    void clockwiseDrop();
    void clockwiseSpin();
    void clockwiseWipe();
    void updateLights();
    // Query methods
    bool allDotsLanded();
    bool discEmpty();
    bool discFull();
    int landedDots();

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
