#include <Arduino.h>

class HypnoDisc {
  public:
    HypnoDisc(
        byte ledCount,
        byte pwmLevels = 5,
        byte latch = 7,
        byte clock = 6,
        byte data = 5);
    void
      begin(void),
      addLight(void),
      clockwiseDrop(void),
      clockwiseSpin(void),
      clockwiseWipe(void),
      updateLights(void);
    boolean
      allDotsLanded(void),
      discEmpty(void),
      discFull(void);
    byte
      ledCount;

  private:
    byte
      clockPin, latchPin, dataPin,
      pwmMaxLevel,
      pwmStep,
      *ledStates;
    void
      latchDown(void),
      latchUp(void),
      setLength(byte length);
};
