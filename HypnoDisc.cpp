#include "HypnoDisc.h"

HypnoDisc::HypnoDisc(byte ledCountInit, byte pwmLevels, byte latch, byte clock, byte data) {
  setLength(ledCountInit);
  pwmMaxLevel = pow(2, (pwmLevels - 1));
  pwmStep = 0;
  latchPin = latch;
  clockPin = clock;
  dataPin = data;
}

void HypnoDisc::begin(void) {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  updateLights();
}

void HypnoDisc::setLength(byte length) {
  if (ledStates != NULL) {
    free(ledStates);
  }
  if (NULL != (ledStates = (byte *)malloc(length))) {
    memset(ledStates, 0x00, length); // Init to RGB 'off' state
    ledCount = length;
  } else {  // malloc failed
    ledCount = 0;
  }
}

void HypnoDisc::latchDown(void) {
  // Start clocking in data
  digitalWrite(latchPin, LOW);
}

void HypnoDisc::latchUp(void) {
  // Stop clocking in data
  digitalWrite(latchPin, HIGH);
}

void HypnoDisc::addLight(void) {
  ledStates[0] = pwmMaxLevel;
}

boolean HypnoDisc::allDotsLanded(void) {
  boolean endOfLanded = false;
  for (byte i = ledCount; i-- > 0;) {
    if (ledStates[i] < pwmMaxLevel) {
      endOfLanded = true;
    } else if (ledStates[i] > 0 && endOfLanded) {
      return false;
    }
  }
  return true;
}

boolean HypnoDisc::discEmpty(void) {
  for (byte i = ledCount; i-- > 0;) {
    if (ledStates[i] > 0) return false;
  }
  return true;
}

boolean HypnoDisc::discFull(void) {
  for (byte i = ledCount; i-- > 0;) {
    if (ledStates[i] < pwmMaxLevel) return false;
  }
  return true;
}

void HypnoDisc::clockwiseDrop(void) {
  // Shifts individual dots down to their spot, or around the ring
  byte current, next;
  for (current = ledCount; --current > 0;) {
    next = current - 1;
    if (ledStates[current] < pwmMaxLevel) {
      ledStates[current] = ledStates[next];
      ledStates[next] >>= 1;
    } else if (ledStates[next] < pwmMaxLevel) {
      ledStates[next] >>= 1;
    }
  }
}

void HypnoDisc::clockwiseSpin(void) {
  byte current, next;
  byte lastValue = ledStates[ledCount - 1];
  for (current = ledCount; --current > 0;) {
    next = current - 1;
    ledStates[current] = ledStates[next];
    ledStates[next] >>= 1;
  }
  ledStates[0] = max(ledStates[0], lastValue);
  ledStates[ledCount - 1] = max(ledStates[ledCount - 1], lastValue >> 1);
}

void HypnoDisc::clockwiseWipe(void) {
  // Shifts the entire ring out from the array, or turns it round
  byte current, next;
  for (current = ledCount; --current > 0;) {
    next = current - 1;
    ledStates[current] = ledStates[next];
    ledStates[next] >>= 1;
  }
}

void HypnoDisc::updateLights(void) {
  latchDown();
  byte i, j, shiftData;
  for (i = 0; i < ledCount / 8; i++) {
    for (j = 0; j < 8; j++) {
      bitWrite(shiftData, j, (ledStates[i * 8 + j] > pwmStep));
    }
    shiftOut(dataPin, clockPin, LSBFIRST, shiftData);
  }
  latchUp();
  pwmStep = ++pwmStep % pwmMaxLevel;
}
