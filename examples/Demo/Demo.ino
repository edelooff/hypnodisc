/* Flying dots on a circular board */
#include <StandardCplusplus.h>
#include <HypnoDisc.h>

const int updateInterval = 30;
const int ringSize = 24;
HypnoDisc disc = HypnoDisc(ringSize);

void setup() {
  disc.begin();
}

void loop() {
  slowFallingDots();
  rapidFallingDots();
  spinningTrail();
  rotorDots();
}

void rapidFallingDots() {
  while (!disc.discFull()) {
    disc.addLight();
    while (!disc.allDotsLanded()) {
      repeatedUpdate();
      disc.clockwiseDrop();
    }
  }
  wipe();
}

void rotorDots() {
  byte divisors[] = {12, 8, 6};
  for (byte div = 0; div < sizeof(divisors); div++) {
    for (unsigned int i = ringSize * 4; i-- > 0;) {
      if (i % divisors[div] == 0)
        disc.addLight();
      repeatedUpdate();
      disc.clockwiseSpin();
    }
    wipe();
  }
}

void slowFallingDots() {
  while (!disc.discFull()) {
    disc.addLight();
    for (byte j = ringSize; j-- > 0;) {
      repeatedUpdate();
      disc.clockwiseDrop();
    }
  }
  wipe();
}

void spinningTrail() {
  for (byte i = 9; i < ringSize; i++) {
    disc.addLight();
    for (byte j = ringSize; --j > 0;) {
      repeatedUpdate();
      disc.clockwiseSpin();
    }
  }
  wipe();
}

void wipe() {
  while (!disc.discEmpty()) {
    disc.clockwiseWipe();
    repeatedUpdate();
  }
}

void repeatedUpdate() {
  long currentMillis = millis();
  while (millis() - currentMillis < updateInterval)
    disc.updateLights();
}
