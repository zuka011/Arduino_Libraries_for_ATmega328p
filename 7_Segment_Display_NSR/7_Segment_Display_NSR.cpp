#include "Arduino.h"
#include "7_Segment_Display_NSR.h"

static const uint8_t DIGIT[10] = {
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

static const uint8_t MAX_DISPLAYS = 5;

static SegDisplay *callback_displays[MAX_DISPLAYS];
static uint8_t last_display = 0;

SegDisplay::SegDisplay(uint8_t segmentPins[], uint8_t digitDisplayPins[], uint8_t nDigitDisplays) {

    this->segmentPins = copyArray(segmentPins, N_SEGMENTS);
    this->displayPins = copyArray(digitDisplayPins, nDigitDisplays);
    this->nDisplays = nDigitDisplays;

    displayValues = new uint8_t[nDisplays];
    dynamicDisplayValues = NULL;

    for(int i = 0; i < N_SEGMENTS; i++) pinMode(segmentPins[i], OUTPUT);
    for(int i = 0; i < nDisplays; i++) {
     
        pinMode(displayPins[i], OUTPUT);
        displayValues[i] = 0;
    }

    currentIndex = 0;
    dynamicMode = false;
}

SegDisplay::~SegDisplay() {

    delete[] segmentPins;
    delete[] displayPins;
    delete[] displayValues;
    if(dynamicDisplayValues != NULL) delete[] dynamicDisplayValues;

    turnOff();
}

void SegDisplay::setNumber(float number, uint8_t digits) {

    setNumber(String(number, digits));
}

void SegDisplay::setNumber(String number) {

    int indexOfDecimal = number.indexOf('.');
    if(indexOfDecimal > -1) number = number.substring(0, indexOfDecimal) + number.substring(indexOfDecimal + 1);
    indexOfDecimal--;

    number.trim();
    int len = number.length();

    for(int i = 0; i < nDisplays; i++) {
     
        if(i >= len) displayValues[i] = DIGIT[0];
        else if(i == indexOfDecimal) displayValues[i] = DIGIT[number[i] - '0'] + DECIMAL; 
        else if(number[i] == '-') displayValues[i] = MINUS_SIGN;
        else displayValues[i] = DIGIT[number[i] - '0'];
    }
}

void SegDisplay::setSymbol(uint8_t ledValues, uint8_t digitIndex) {
    
    if(digitIndex < nDisplays) displayValues[digitIndex] = ledValues;
}

void SegDisplay::setSymbols(uint8_t ledValues[]) {

    delete[] displayValues;
    displayValues = copyArray(ledValues, nDisplays);
}

void SegDisplay::clear() {

    for(int i = 0; i < nDisplays; i++) displayValues[i] = 0;
}

uint8_t *SegDisplay::getDisplayValues() {

    return copyArray(displayValues, nDisplays);
}

void SegDisplay::turnOn(unsigned long duration) {

    enableTimer2();

    if(last_display == MAX_DISPLAYS) return;
    for(int i = 0; i < last_display; i++) if(callback_displays[i] == this) return;
    
    callback_displays[last_display++] = this;
}

void SegDisplay::turnOff() {

    int removedDisplayIndex = -1;

    for(int i = 0; i < last_display; i++) {

        if (callback_displays[i] == this) {

            removedDisplayIndex = i;
            break;
        }
    }

    if(removedDisplayIndex != -1) {

        for(int i = removedDisplayIndex + 1; i < MAX_DISPLAYS; i++) callback_displays[i - 1] = callback_displays[i];
        last_display--;
    }

    displayByte(0, 0);
}

void SegDisplay::showNumber(float number, uint8_t digits, unsigned long holdDuration, int nRepeats) {

    showNumber(String(number, digits), holdDuration, nRepeats);
}

void SegDisplay::showNumber(String number, unsigned long holdDuration, int nRepeats) {

    dynamicMode = true;
    this->holdDuration = holdDuration;
    this->nRepeats = nRepeats;

    setDynamicValues(number);

    currentDynamicIndex = 0;
    currentIndex = 0;
    dynamicTimer = millis();
}

void SegDisplay::showSymbol(uint8_t ledValues[], uint8_t nSymbols,  unsigned long holdDuration, int nRepeats) {

    dynamicMode = true;
    this->holdDuration = holdDuration;
    this->nRepeats = nRepeats;

    if(dynamicDisplayValues != NULL) delete[] dynamicDisplayValues;
    dynamicDisplayValues = copyArray(ledValues, nSymbols);

    currentDynamicIndex = 0;
    currentIndex = 0;
    dynamicTimer = millis();
}

bool SegDisplay::isDynamic() {
    return dynamicMode;
}

void SegDisplay::disableDynamicMode() {
    dynamicMode = false;
}

void SegDisplay::show() {

    if(dynamicMode) {
        
        displayByte(dynamicDisplayValues[currentIndex + currentDynamicIndex], currentDynamicIndex);
        currentDynamicIndex++;

        if(currentDynamicIndex == nDisplays) {
            
            currentDynamicIndex = 0;
            if(millis() - dynamicTimer > holdDuration) {
                if(currentIndex + nDisplays < nDynamicValues) currentIndex++;
                else {
                    
                    currentIndex = 0;
                    if(nRepeats == 0) dynamicMode = false;
                    else if(nRepeats > 0) nRepeats--;
                }  
                dynamicTimer = millis();
            }
        }

    } else {
     
        if(currentIndex == nDisplays) currentIndex = 0;
        displayByte(displayValues[currentIndex], currentIndex);
        currentIndex++;
    }
}

void SegDisplay::displayByte(uint8_t value, uint8_t displayIndex) {

    for(int i = 0; i < N_SEGMENTS; i++) {

        digitalWrite(segmentPins[i], value % 2);
        value = value >> 1;
    }

    for(int i = 0; i < nDisplays; i++) digitalWrite(displayPins[i], i != displayIndex);
}

uint8_t *SegDisplay::copyArray(uint8_t *array, int n) {

    uint8_t *newArray = new uint8_t[n];
    for(int i = 0; i < n; i++) newArray[i] = array[i];
    return newArray;    
}

void SegDisplay::setDynamicValues(String number) {

    if(dynamicDisplayValues != NULL) delete[] dynamicDisplayValues;

    int indexOfDecimal = number.indexOf('.');
    if(indexOfDecimal > -1)  number = number.substring(0, indexOfDecimal) + number.substring(indexOfDecimal + 1);
    indexOfDecimal--;

    number.trim();

    int len = number.length();
    nDynamicValues = len + 2*nDisplays;
    dynamicDisplayValues = new uint8_t[len + 2*nDisplays];

    for(int i = 0; i < len; i++) {

        if(i == indexOfDecimal) dynamicDisplayValues[i + nDisplays] = DIGIT[number[i] - '0'] + DECIMAL; 
        else if(number[i] == '-') dynamicDisplayValues[i + nDisplays] = MINUS_SIGN;
        else dynamicDisplayValues[i + nDisplays] = DIGIT[number[i] - '0'];
    }

    for(int i = 0; i < nDisplays; i++) dynamicDisplayValues[i] = 0;
    for(int i = len + nDisplays; i < nDynamicValues; i++) dynamicDisplayValues[i] = 0;
}

void SegDisplay::enableTimer2() {

    if(TCCR2B & B00000111 == 0) TCCR2B |= B00000101;

    OCR2A |= B01111111;
    TIMSK2 |= _BV(OCIE2A); 
}

ISR(TIMER2_COMPA_vect) {

    static const int DELAY = 2;
    static volatile unsigned long timer = 0;

    if(millis() - timer > DELAY) {
        for(int i = 0; i < last_display; i++) callback_displays[i]->show();
        timer = millis();
    }
}