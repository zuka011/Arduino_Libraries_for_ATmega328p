#ifndef _7_Segment_Display_NSR_h
#define _7_Segment_Display_NSR_h

#include "Arduino.h"

// Some symbols the user may want to use out of context.
static const uint8_t MINUS_SIGN = B00000010;
static const uint8_t DECIMAL = B00000001;

static const uint8_t N_SEGMENTS = 8;
const int INDEFINITE = -1;

// The NSR stands for No Shift Register, because this library doesn't use one.
class SegDisplay {
public:

    // DISCLAIMER: This library overrides the compare match A interrupt of Timer2, however it is compatible
    // with the <Servo_Motor.h> library.

    /** 
     * Constructor:
     * 
     * Creates a SegDisplay object that is associated with an N digit 7 segment display (8 with decimal)
     * by specifying the individual pins used to control the segments and the pins used to control the digits.
     * 
     * Specify the segment pins in the following order:
     * -------------------------------------------------------------
     * {DECIMAL_PIN, G_PIN, F_PIN, E_PIN, D_PIN, C_PIN, B_PIN, A_PIN}
     * ------------------------------------------------------------- 
     * where X_PIN is the pin connected to X segment.
     * 
     * Specify the digit pins in the following order:
     * -------------------------------
     * {D1_PIN, D2_PIN, D3_PIN, D4_PIN}
     * -------------------------------
     * where DX_PIN is the pin connected to the X-th digit.
     * 
     * This library supports two modes of operation - Static and Dynamic.
     * 
     * In Static mode you can use setNumber()/setSymbol() to set certain values to certain digits 
     * of the 7 segment display, these will always be displayed (unless Dynamic mode is on). 
     * 
     * In Dynamic mode you can use showNumber()/showSymbol() to temporarily (or indefinitely) show
     * a moving image (from right to left) of whatever you specified in the above mentioned methods.
     */
    SegDisplay(uint8_t segmentPins[], uint8_t digitDisplayPins[], uint8_t nDigitDisplays);
    ~SegDisplay();

    /**
     * setNumber():
     * ------------------------------------------------------------------------------------------------
     * Configures the object, so that it will display [number] after turnOn() is called (you can also turn
     * on the display before calling this method, it will still work). Optional [digits] parameter specifies 
     * how many digits it should display after the decimal point.
     */
    void setNumber(float number, uint8_t digits = 2);
    void setNumber(String number);

    /**
     * setSymbol():
     * ------------------------------------------------------------------------------------------------
     * Does the same as setNumber(), but with these you can either specify all of the digit values using 
     * a byte array, or specify the value for a certain digit (by specifying the parameter [digitIndex]).
     * 
     * !! [ledValues[]] ARRAY IS COPIED INTO AN INTERNAL ARRAY !!
     * if [ledValues[]] is dynamically allocated, getting rid of it is your responsibility. This library
     * will not do that for you. It only manages the internal copy.
     */
    void setSymbol(uint8_t ledValues, uint8_t digitIndex = 0);
    void setSymbols(uint8_t ledValues[]);

    /**
     * clear():
     * ------------------------------------------------------------------------------------------------
     * Clears the values of digits in Static mode.
     */
    void clear();

    /**
     * getDisplayValues():
     * ------------------------------------------------------------------------------------------------
     * Returns a dynamically allocated copy of the internal array holding the digit values.
     */
	uint8_t *getDisplayValues();

    /**
     * turnOn()/turnOff():
     * ------------------------------------------------------------------------------------------------
     * Turns on/off the display. The individual digit values of Static mode are kept and will reappear as
     * soon as you turn the display back on.
     */
    void turnOn(unsigned long duration = 0);
    void turnOff();
    
    /**
     * showNumber()/showSymbol():
     * ------------------------------------------------------------------------------------------------
     * Calling these methods enables Dynamic mode for this display. You can specify how long the delay
     * between transitions is ([holdDuration]), as well as how many times the animation is repeated ([nRepeats]).
     * You can pass INDEFINITE as the final parameter to indefinitely keep the animation going.
     */
    void showNumber(float number, uint8_t digits = 2, unsigned long holdDuration = 250, int nRepeats = 0);
    void showNumber(String number, unsigned long holdDuration = 250, int nRepeats = 0);
    void showSymbol(uint8_t ledValues[], uint8_t nSymbols, unsigned long holdDuration = 250, int nRepeats = 0);

    /**
     * disableDynamicMode():
     * ------------------------------------------------------------------------------------------------
     * This will prematurely disable Dynamic mode (The animation will not finish, if it is ongoing) and
     * revert the display to Static mode.
     */
    void disableDynamicMode();
    bool isDynamic(); // Returns true if the display is still in dynamic mode.

    /**
     * show():
     * ------------------------------------------------------------------------------------------------
     * This method perfroms one step of displaying a value on the 7 segment display. It is used by the 
     * Interrupt Service Routine. You will not need to call this directly.
     */
    void show();

private:
    uint8_t *segmentPins;
	uint8_t *displayPins;
    uint8_t *displayValues;
    uint8_t *dynamicDisplayValues;
    uint8_t nDisplays;
    uint8_t nDynamicValues;

    volatile uint8_t currentIndex;
    volatile uint8_t currentDynamicIndex;

    volatile bool dynamicMode;
    volatile int nRepeats;
    unsigned long holdDuration;
    volatile unsigned long dynamicTimer;

    uint8_t *copyArray(uint8_t *array, int n);

    void enableTimer2();
    void displayByte(uint8_t value, uint8_t displayIndex);
    void setDynamicValues(String number);
};

#endif