enum State {
    IDLE,      // 0 (default value)
    RUNNING,   // 1    // 2
    FINISHED    // 3
};

typedef enum {
		G3 = 196,
    C4 = 261,
    D4 = 294,
    E4 = 330,
    F4 = 349,
    G4 = 392,
    A4 = 440,
    B4 = 494
} C_Octave_Notes;

typedef enum {
    EIGHTH = 1,
    QUARTER = 2,
    HALF = 4
} NoteDuration;

void setFreq(C_Octave_Notes note);

void initPWM(void);
void playOdeToJoy();