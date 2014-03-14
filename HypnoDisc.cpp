#include "HypnoDisc.h"

HypnoDisc::HypnoDisc(
    byte discSize, byte pwmLevels, byte latchPin, byte clockPin, byte dataPin)
  : ledStates(discSize),
    pwmMaxLevel(1 << pwmLevels),
    latchPin(latchPin),
    clockPin(clockPin),
    dataPin(dataPin) {
}

void HypnoDisc::begin() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  updateLights();
}

void HypnoDisc::addLight() {
  ledStates.front() = pwmMaxLevel;
}

boolean HypnoDisc::allDotsLanded() {
  boolean endOfLanded = false;
  for(std::reverse_iterator<byte*> iter = ledStates.rbegin();
      iter != ledStates.rend(); ++iter)
    if (!endOfLanded && *iter < pwmMaxLevel)
      endOfLanded = true;
    else if (endOfLanded && *iter > 0)
      return false;
  return true;
}

boolean HypnoDisc::discEmpty() {
  for(byte *iter = ledStates.begin(); iter != ledStates.end(); ++iter)
    if (*iter)
      return false;
  return true;
}

boolean HypnoDisc::discFull() {
  for(byte *iter = ledStates.begin(); iter != ledStates.end(); ++iter)
    if (*iter != pwmMaxLevel)
      return false;
  return true;
}

void HypnoDisc::clockwiseDrop() {
  // Shifts individual dots down to their spot, or around the ring
  byte source, target;
  for (target = ledStates.size(); --target > 0;) {
    source = target - 1;
    if (ledStates[target] < pwmMaxLevel) {
      ledStates[target] = ledStates[source];
      ledStates[source] >>= 1;
    } else if (ledStates[source] < pwmMaxLevel) {
      ledStates[source] >>= 1;
    }
  }
}

void HypnoDisc::clockwiseSpin() {
  byte lastValue = ledStates.back();
  clockwiseWipe();
  ledStates.front() = max(ledStates.front(), lastValue);
  ledStates.back() = max(ledStates.back(), lastValue >> 1);
}

void HypnoDisc::clockwiseWipe() {
  // Shifts the entire ring out from the array, or turns it round
  byte source, target;
  for (target = ledStates.size(); --target > 0;) {
    source = target - 1;
    ledStates[target] = ledStates[source];
    ledStates[source] >>= 1;
  }
}

void HypnoDisc::updateLights() {
  latch l = latch(latchPin);
  byte ctr, shiftData;
  for(byte *iter = ledStates.begin(); iter != ledStates.end(); ++iter) {
    bitWrite(shiftData, ctr++, (*iter > pwmStep));
    if (ctr == 8) {
      ctr = 0;
      shiftOut(dataPin, clockPin, LSBFIRST, shiftData);
      shiftData = 0;
    }
  }
  if (ctr) {
    shiftOut(dataPin, clockPin, LSBFIRST, shiftData);
  }
  pwmStep = ++pwmStep % pwmMaxLevel;
}

HypnoDisc::latch::latch(byte pin)
  // Pull down the latch to start clocking in data
  : pin(pin) {
  digitalWrite(pin, LOW);
}

HypnoDisc::latch::~latch() {
  // Pushes the latch high again, displaying the clocked in data
  digitalWrite(pin, HIGH);
}
