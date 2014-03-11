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
    void begin(void);
    void addLight(void);
    void clockwiseDrop(void);
    void clockwiseSpin(void);
    void clockwiseWipe(void);
    void updateLights(void);
    boolean allDotsLanded(void);
    boolean discEmpty(void);
    boolean discFull(void);

  private:
    byte clockPin, dataPin, latchPin;
    byte pwmMaxLevel, pwmStep;
    std::vector<byte> ledStates;
    void latchDown(void);
    void latchUp(void);
};
