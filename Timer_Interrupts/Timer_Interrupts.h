#ifndef _Timer_Interrupts_h
#define _Timer_Interrupts_h

#include "Arduino.h"

enum Timer { Timer0, Timer1, Timer2 };
enum Prescaler { None = 1, _8th, _64th, _256th, _1024th };

class TimerInterrupt {
public:

    /** 
     * Constructor:
     * Creates a TimerInterrupt object that is associated with the timer specified by [timer].
     * Available timers are: Timer0, Timer1, Timer2;
     * Available prescalers are: None, _8th, _64th, _256th, _1024th;
     * 
     * !! USING TIMERS MAY CAUSE ISSUES WITH OTHER FUNCTIONS THAT USE THE SAME TIMER !! 
     * Make sure you know what the timer you're using is responsible for.
     */
    TimerInterrupt(Timer timer, Prescaler scaler);

    /**
     * initTimer():
     * ------------------------------------------------------------------------------------------------
     * Initializes the timer with the given value. After calling this method the timer will start counting
     * according to the prescaler specified in the objects constructor.
     */
    void initTimer(uint16_t initValue);

    /**
     * enableCompareA()/enableCompareB()/enableOVF(),
     * disableCompareA()/disableCompareB()/disableOVF():
     * ------------------------------------------------------------------------------------------------
     * This methods enable or disable the interrupts associated with the timer. In case of the compare 
     * match interrupts, a compare value must be specified.
     * 
     * !! MAKE SURE TO DEFINE AN ISR FUNCTION WHEN ENABLING AN INTERRUPT !!
     * Otherwise your code may get trapped in a loop of doing nothing.
     * 
     * Define an ISR in the following way (usually after void loop()):
     * 
     * ISR([Interrupt address in the interrupt vector table for timer here]) {
     *      ...
     *      // Body of ISR function
     *     ...
     * }
     * 
     * Available options are:
     * 
     * Timer2:
     *  TIMER2_COMPA_vect
     *  TIMER2_COMPB_vect
     *  TIMER2_OVF_vect
     * 
     * Timer1:
     *  TIMER1_COMPA_vect
     *  TIMER1_COMPB_vect
     *  TIMER1_OVF_vect
     * 
     * Timer0:
     *  TIMER0_COMPA_vect
     *  TIMER0_COMPB_vect
     *  TIMER0_OVF_vect
     * 
     * Example:
     *  ISR(TIMER2_COMPA_vect) {
     * 
     *      Serial.println("Interrupt has occured.");
     *  }
     */
    void enableCompareA(uint16_t compareValueA);
    void enableCompareB(uint16_t compareValueB);
    void enableOVF();

    void disableCompareA();
    void disableCompareB();
    void disableOVF();

    /**
     * load():
     * ------------------------------------------------------------------------------------------------
     * Loads a new value into the timer.
     */
    void load(uint16_t value);

private:

    Timer timer;
    Prescaler scaler;

    uint8_t *TimerControlRegisterA;
    uint8_t *TimerControlRegisterB;
    uint8_t *TimerCounterRegister;
    uint8_t *OutputCompareA;
    uint8_t *OutputCompareB;
    uint8_t *TimerInterruptMask;
    
    uint16_t *TimerCounterRegister16;
    uint16_t *OutputCompareA16;
    uint16_t *OutputCompareB16;

    void getTimer();

};


#endif