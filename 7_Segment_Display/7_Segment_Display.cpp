#include "Arduino.h"
#include "7_Segment_Display.h"

static const byte DIGIT[10] = {
    B11111100,
    B01100000,
    B11011010,
    B11110010,
    B01100110,
    B10110110,
    B10111110,
    B11100000,
    B11111110,
    B11110110,
};

static const byte MINUS_SIGN = B00000010;

SegDisplay::SegDisplay(byte clockPin, byte latchPin, byte dataPin, byte displayPins[], byte nDisplays) {

	this->clockPin = clockPin;
	this->dataPin = dataPin;
	this->latchPin = latchPin;
	this->displayPins = displayPins;
    this->nDisplays = nDisplays;

    displayValues = new byte[nDisplays];

    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);

    for (int i = 0; i < nDisplays; i++) pinMode(displayPins[i], OUTPUT);
}

SegDisplay::~SegDisplay() {
    delete[] displayValues;
}

void SegDisplay::setNumber(float number) {
    setNumber(String(number));
}

void SegDisplay::setNumber(String number) {

    byte indexOfDecimal = number.indexOf('.');
    number = number.substring(0, indexOfDecimal) + number.substring(indexOfDecimal + 1);
    indexOfDecimal--;

	for (int i = 0; i < nDisplays; i++) {
	
		if(number[i] == '-') setSymbol(MINUS_SIGN);
		else setDigit(number[i] - '0', i, i == indexOfDecimal && i != (nDisplays - 1));
	}
}

void SegDisplay::setSymbol(byte ledValues, byte displayIndex) {
    displayValues[displayIndex] = ledValues;
}

void SegDisplay::setSymbols(byte ledValues[]) {
    for(int i = 0; i < nDisplays; i++) displayValues[i] = ledValues[i];
}

byte * SegDisplay::getDisplayValues() {
	return displayValues;
}

void SegDisplay::show(int duration) {

    long timer = millis();
    while(millis() - timer < duration) {

        for(int i = 0; i < nDisplays; i++) {

			digitalWrite(clockPin, LOW);
			digitalWrite(latchPin, LOW);

			shiftOut(dataPin, clockPin, LSBFIRST, displayValues[i]);

            enableDisplay(i);
			digitalWrite(latchPin, HIGH);
			delay(3);
        }
    }
	digitalWrite(displayPins[nDisplays - 1], HIGH);
}

void SegDisplay::clear() {
    for(int i = 0; i < nDisplays; i++) displayValues[i] = 0;
}

void SegDisplay::setDigit(byte digit, byte displayIndex, bool decimal) {
    
	if (digit > 9 || digit < 0) setSymbol(DIGIT[0], displayIndex);
    else setSymbol(DIGIT[digit] + (decimal ? 1 : 0), displayIndex);
}

void SegDisplay::enableDisplay(byte displayIndex) {

    for (int i = 0; i < nDisplays; i++) {
        
		if (i == displayIndex) digitalWrite(displayPins[i], LOW);
        else digitalWrite(displayPins[i], HIGH);
    }
}
