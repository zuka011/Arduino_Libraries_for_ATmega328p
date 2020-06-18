#ifndef _Rotary_Encoder_h
#define _Rotary_Encoder_h

#include "Arduino.h"

enum Direction {Clockwise, CounterClockwise};

struct Encoder {
public:

    Encoder(uint8_t pinA, uint8_t pinB);
    ~Encoder();

    void init();

    void attachFunction(void (*func)(void), Direction direction);

    long getTickCount();
    Direction getLastDirection();

    void checkEncoder();

private:

    uint8_t pinA, pinB;
    uint8_t pinABit, pinBBit;
    uint8_t *PINRegisterA;
    uint8_t *PINRegisterB;

    long tickCount;
    Direction turnDirection;

    void (*clockWiseFunc)(void);
    void (*cClockWiseFunc)(void);

    uint8_t *getPin(uint8_t &pin);

    void enableCallback();
    void disableCallback();
};

#endif 