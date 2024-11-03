
#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"


enum State {
    IDLE,      // 0 (default value)
    RUNNING,   // 1    // 2
    FINISHED    // 3
};

extern osSemaphoreId_t brainSem;
extern osSemaphoreId_t musicSem;
extern osSemaphoreId_t moveSem;

typedef enum {
    RED,
    GREEN,
    BLUE
} color_t;

//Music Helpers
typedef enum {
		F3 = 175,
		G3 = 196,
		GS3 = 208,
		AS3 = 233,
    C4 = 261,
    D4 = 294,
		DS4 = 311,
    E4 = 330,
    F4 = 349,
    G4 = 392,
    A4 = 440,
    B4 = 494
} C_Octave_Notes;

typedef enum {
    EIGHTH = (uint8_t)1,
    QUARTER = (uint8_t)2,
		QUARTERPLUS = (uint8_t)3,
    HALF = (uint8_t)4
} NoteDuration;

void setFreq(C_Octave_Notes note);
void initPWM(void);
void playOdeToJoy(void);
void playEnding(void);

//Motor Helpers
void initMotors(void);
void move(int movedata);
void stop(void);

//Front and Back Helpers
void initLEDs(void);
void greenLEDControl(enum State state);
void redLEDControl(enum State state);
void initUART2 (uint32_t baud_rate);
