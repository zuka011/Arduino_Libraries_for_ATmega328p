#ifndef _Buzzer_h
#define _Buzzer_h

#include "Arduino.h"

#define INDEFINITE -1

enum Letter {C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B};

struct Note {

    Letter name;
    uint8_t octave;

    Note(Letter name, uint8_t octave) : name(name), octave(octave) {};
};

class Buzzer {
public:

    Buzzer(float referencePitch = 440);
    ~Buzzer();

    void attach(uint8_t buzzerPin);
    void setReference(float referencePitch);
    
    void playTone(float frequency, long duration = INDEFINITE);
    void playNote(Note note, long duration = INDEFINITE);

    void playSequence(float *frequencies, long *duration, int nFrequencies);
    void playSequence(Note *notes, long *duration, int nNotes);

    void play();
private:

    int buzzerPin, sequenceSize;
    volatile int currNote;
    volatile unsigned long playTimer;
    float referencePitch, **noteFrequencies;

    volatile int playState;

    float *currSequence;
    long *currDuration;

    void startTone(float frequency);
    void stopTone();
    
    void getNoteFrequencies();

    void getSequence(float *frequencies, long *duration, int nFrequencies);
    void getSequence(Note *notes, long *duration, int nNotes);

    void addBuzzer();
    void removeBuzzer();
    void enableTimer();
};

#endif