#include "HypnoDisc.h"

HypnoDisc::HypnoDisc(
    byte discSize, byte trailSize, byte latchPin, byte clockPin, byte dataPin)
  : ledStates(discSize),
    pwmMaxLevel(1 << (trailSize - 1)),
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

void HypnoDisc::addDot() {
  ledStates.front() = pwmMaxLevel;
}

bool HypnoDisc::allDropped() {
  // Returns whether or not all dots have dropped to the end of the disc
  bool endOfLanded = false;
  for(std::reverse_iterator<byte*> iter = ledStates.rbegin();
      iter != ledStates.rend(); ++iter)
    if (!endOfLanded && *iter < pwmMaxLevel)
      endOfLanded = true;
    else if (endOfLanded && *iter > 0)
      return false;
  return true;
}

bool HypnoDisc::empty() {
  // Returns whether or not the disc is completely empty
  for(std::vector<byte>::iterator iter = ledStates.begin();
      iter != ledStates.end(); ++iter)
    if (*iter)
      return false;
  return true;
}

bool HypnoDisc::full() {
  // Returns whether or not the disc is filled with fully lit dots
  for(std::vector<byte>::iterator iter = ledStates.begin();
      iter != ledStates.end(); ++iter)
    if (*iter != pwmMaxLevel)
      return false;
  return true;
}

unsigned int HypnoDisc::dotsDropped() {
  // Returns the number of dots that have landed
  unsigned int dotCount = 0;
  for(std::reverse_iterator<byte*> iter = ledStates.rbegin();
      iter != ledStates.rend(); ++iter) {
    if (*iter == pwmMaxLevel) dotCount++;
    else break;
  }
  return dotCount;
}

void HypnoDisc::clockwiseDrop() {
  // Shifts individual dots down to their spot, or around the ring
  unsigned int source, target;
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
  // Moves all dots on the disc by one position, wrapping across the ends
  byte lastValue = ledStates.back();
  clockwiseWipe();
  ledStates.front() = max(ledStates.front(), lastValue);
}

void HypnoDisc::clockwiseWipe() {
  // Moves all dots on the disc by one position without wrapping the ends
  unsigned int source, target;
  for (target = ledStates.size(); --target > 0;) {
    source = target - 1;
    ledStates[target] = ledStates[source];
    ledStates[source] >>= 1;
  }
}

void HypnoDisc::updateLights() {
  //
  latch l = latch(latchPin);
  byte position = 0, shiftData;
  for(std::vector<byte>::iterator iter = ledStates.begin();
      iter != ledStates.end(); ++iter) {
    bitWrite(shiftData, position++, (*iter > pwmStep));
    if (position == 8) {
      position = 0;
      shiftOut(dataPin, clockPin, LSBFIRST, shiftData);
      shiftData = 0;
    }
  }
  if (position) {
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
