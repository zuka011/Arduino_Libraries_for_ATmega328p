#include "Arduino.h"
#include "Timer_Interrupts.h"


TimerInterrupt::TimerInterrupt(Timer timer, Prescaler scaler) {

    this->timer = timer;
    this->scaler = scaler;

    getTimer();
}

void TimerInterrupt::getTimer() {

    switch(timer) {

        case Timer0: 

            TimerControlRegisterA = &TCCR0A;
            TimerControlRegisterB = &TCCR0B;
            TimerCounterRegister = &TCNT0;
            OutputCompareA = &OCR0A;
            OutputCompareB = &OCR0B;
            TimerInterruptMask = &TIMSK0;
            break;
        case Timer1:

            TimerControlRegisterA = &TCCR1A;
            TimerControlRegisterB = &TCCR1B;
            TimerCounterRegister16 = &TCNT1;
            OutputCompareA16 = &OCR1A;
            OutputCompareB16 = &OCR1B;
            TimerInterruptMask = &TIMSK1;
            break;
        case Timer2:

            TimerControlRegisterA = &TCCR2A;
            TimerControlRegisterB = &TCCR2B;
            TimerCounterRegister = &TCNT2;
            OutputCompareA = &OCR2A;
            OutputCompareB = &OCR2B;
            TimerInterruptMask = &TIMSK2;
            break;
    }
}

void TimerInterrupt::initTimer(uint16_t initValue) {

    noInterrupts();

    *TimerControlRegisterA = 0;

    *TimerControlRegisterB &= B11111000; // Clears the first three bits
    *TimerControlRegisterB |= scaler;

    load(initValue);

    interrupts();
}

void TimerInterrupt::enableCompareA(uint16_t compareValueA) {

    if(timer == Timer1) *OutputCompareA16 = compareValueA;
    else *OutputCompareA = compareValueA;

    *TimerInterruptMask |=  B00000010;
}

void TimerInterrupt::enableCompareB(uint16_t compareValueB) {

    if(timer == Timer1) *OutputCompareB16 = compareValueB;
    else *OutputCompareB = compareValueB;

    *TimerInterruptMask |=  B00000100;
}

void TimerInterrupt::enableOVF() {
    
    *TimerInterruptMask |=  B00000001;
}

void TimerInterrupt::disableCompareA() {

    *TimerInterruptMask &=  ~(B00000010);
}

void TimerInterrupt::disableCompareB() {

    *TimerInterruptMask &=  ~(B00000100);
}

void TimerInterrupt::disableOVF() {

    *TimerInterruptMask &=  ~(B00000001);
}

void TimerInterrupt::load(uint16_t value) {

    if(timer == Timer1) *TimerCounterRegister16 = value;
    else *TimerCounterRegister = value;
}