/* Flying dots on a circular board */
#include <StandardCplusplus.h>
#include <HypnoDisc.h>

const unsigned int updateInterval = 25;
const unsigned int ringSize = 24;
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
  while (!disc.isFull()) {
    disc.addDot();
    while (disc.hasFallingDots()) {
      repeatedUpdate();
      disc.clockwiseDrop();
    }
  }
  wipe();
}

void rotorDots() {
  byte divisors[] = {12, 8, 6};
  for (byte div = 0; div < sizeof(divisors); div++) {
    for (unsigned int i = ringSize * 5; i-- > 0;) {
      if (i % divisors[div] == 0)
        disc.addDot();
      repeatedUpdate();
      disc.clockwiseSpin();
    }
    wipe();
  }
}

void slowFallingDots() {
  while (!disc.isFull()) {
    disc.addDot();
    for (byte j = ringSize; j-- > 0;) {
      repeatedUpdate();
      disc.clockwiseDrop();
    }
  }
  wipe();
}

void spinningTrail() {
  for (byte i = 9; i < ringSize; i++) {
    disc.addDot();
    for (byte j = ringSize; --j > 0;) {
      repeatedUpdate();
      disc.clockwiseSpin();
    }
  }
  wipe();
}

void wipe() {
  while (!disc.isEmpty()) {
    disc.clockwiseWipe();
    repeatedUpdate();
  }
}

void repeatedUpdate() {
  long currentMillis = millis();
  while (millis() - currentMillis < updateInterval)
    disc.update();
}
