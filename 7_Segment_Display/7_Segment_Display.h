#ifndef _Segment_Display_h
#define _Segment_Display_h

#include "Arduino.h"

class SegDisplay {
public:
    SegDisplay(byte clockPin, byte latchPin, byte dataPin, byte digitDisplayPins[], byte nDigitDisplays);
    ~SegDisplay();

    void setNumber(float number);
    void setNumber(String number);

    void setSymbol(byte ledValues, byte digitIndex = 0);
    void setSymbols(byte ledValues[]);

	byte *getDisplayValues();

    void show(int duration = 10);
    void clear();

private:
	byte *displayPins;
    byte *displayValues;
    byte nDisplays;

	byte clockPin, dataPin, latchPin;

    void setByte(byte value, byte displayIndex);
    void setDigit(byte digit, byte displayIndex, bool decimal = false);
    void enableDisplay(byte displayIndex);
};

#endif