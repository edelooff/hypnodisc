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
  ledStates[0] = pwmMaxLevel;
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
    if (*iter > 0)
      return false;
  return true;
}

boolean HypnoDisc::discFull() {
  for(byte *iter = ledStates.begin(); iter != ledStates.end(); ++iter)
    if (*iter < pwmMaxLevel)
      return false;
  return true;
}

void HypnoDisc::clockwiseDrop() {
  // Shifts individual dots down to their spot, or around the ring
  byte current, next;
  for (current = ledStates.size(); --current > 0;) {
    next = current - 1;
    if (ledStates[current] < pwmMaxLevel) {
      ledStates[current] = ledStates[next];
      ledStates[next] >>= 1;
    } else if (ledStates[next] < pwmMaxLevel) {
      ledStates[next] >>= 1;
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
  byte current, next;
  for (current = ledStates.size(); --current > 0;) {
    next = current - 1;
    ledStates[current] = ledStates[next];
    ledStates[next] >>= 1;
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

latch::latch(byte pin)
  // Pull down the latch to start clocking in data
  : pin(pin) {
  digitalWrite(pin, LOW);
}

latch::~latch() {
  // Pushes the latch high again, displaying the clocked in data
  digitalWrite(pin, HIGH);
}
