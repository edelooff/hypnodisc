/* Flying dots on a circular board */
#include <StandardCplusplus.h>
#include <HypnoDisc.h>

const byte updateInterval = 30;
const byte ringSize = 24;

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
  for (byte i = ringSize; i-- > 0;) {
    disc.addLight();
    while (!disc.allDotsLanded()) {
      disc.clockwiseDrop();
      updateFor(updateInterval);
    }
  }
  wipe();
}

void rotorDots() {
  byte divisors[] = {12, 8, 6};
  for (byte div = 0; div < sizeof(divisors); div++) {
    for (byte i = ringSize * 4; i-- > 0;) {
      if (i % divisors[div] == 0) {
        disc.addLight();
      }
      disc.clockwiseSpin();
      updateFor(updateInterval);
    }
    wipe();
  }
}

void slowFallingDots() {
  for (byte i = ringSize; i-- > 0;) {
    disc.addLight();
    for (byte j = ringSize; j-- > 0;) {
      disc.clockwiseDrop();
      updateFor(updateInterval);
    }
  }
  wipe();
}

void spinningTrail() {
  for (byte i = 6; i < ringSize; i++) {
    disc.addLight();
    for (byte j = ringSize; --j > 0;) {
      disc.clockwiseSpin();
      updateFor(updateInterval);
    }
  }
  wipe();
}

void wipe() {
  while (!disc.discEmpty()) {
    disc.clockwiseWipe();
    updateFor(updateInterval);
  }
}

void updateFor(byte timeout) {
  long currentMillis = millis();
  while (millis() - currentMillis < timeout) {
    disc.updateLights();
  }
}
