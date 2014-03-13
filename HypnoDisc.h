#include <StandardCplusplus.h>
#include <vector>
#include <Arduino.h>

struct latch{
  latch(byte pin);
  ~latch();
  const byte pin;
};

class HypnoDisc {
  public:
    HypnoDisc(
        byte discSize,
        byte pwmLevels = 5,
        byte latchPin = 7,
        byte clockPin = 6,
        byte dataPin = 5);
    void begin();
    void addLight();
    void clockwiseDrop();
    void clockwiseSpin();
    void clockwiseWipe();
    void updateLights();
    boolean allDotsLanded();
    boolean discEmpty();
    boolean discFull();

  private:
    const byte clockPin, dataPin, latchPin, pwmMaxLevel;
    byte pwmStep = 0;
    std::vector<byte> ledStates;
    void latchDown();
    void latchUp();
    latch toggleLatch();
};
