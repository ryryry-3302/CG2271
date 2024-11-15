#include "MKL25Z4.h"
#include "cmsis_os2.h"
#include "helpers.h"





#define PTA12_Pin 12
#define FREQ_2_MOD(x) (375000 / x)

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))

C_Octave_Notes melody[] = {
		E4, E4, F4, G4, G4, F4, E4, D4,
		C4, C4, D4, E4, E4, D4, D4,
	
		E4, E4, F4, G4, G4, F4, E4, D4,
		C4, C4, D4, E4, D4, C4, C4,
	
		D4, D4, E4, C4, D4, E4, F4, E4, C4,
		D4, E4, F4, E4, D4, C4, D4, G3,
	
		E4, E4, F4, G4, G4, F4, E4, D4,
		C4, C4, D4, E4, D4, C4, C4
};

uint8_t noteDurations[] = {
	QUARTER, QUARTER, QUARTER, QUARTER, // E4, E4, F4, G4
	QUARTER, QUARTER, QUARTER, QUARTER, // G4, F4, E4, D4
	QUARTER, QUARTER, QUARTER, QUARTER, // C4, C4, D4, E4
	QUARTER, QUARTER, HALF,          // E4, D4, D4

	QUARTER, QUARTER, QUARTER, QUARTER, // E4, E4, F4, G4
	QUARTER, QUARTER, QUARTER, QUARTER, // G4, F4, E4, D4
	QUARTER, QUARTER, QUARTER, QUARTER, // C4, C4, D4, E4
	QUARTER, QUARTER, HALF,          // D4, C4, C4

	QUARTER, QUARTER, QUARTER, QUARTER, // D4, D4, E4, C4
	QUARTER, EIGHTH, EIGHTH, QUARTER, // D4, E4, F4, E4
	QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER,  // C4, D4, E4, F4, E4, G3
	QUARTER, QUARTER, QUARTER,QUARTER, //D4, C4, D4, G3,
	
	
	QUARTER, QUARTER, QUARTER, QUARTER, // E4, E4, F4, G4
	QUARTER, QUARTER, QUARTER, QUARTER, // G4, F4, E4, D4
	QUARTER, QUARTER, QUARTER, QUARTER, // C4, C4, D4, E4
	QUARTER, QUARTER, HALF          // D4, C4, C4
};




void setFreq(C_Octave_Notes note) {
    TPM1->MOD = FREQ_2_MOD(note);
    TPM1_C0V = (FREQ_2_MOD(note)) / 6;
}

/* Initialize PWM */
void initPWM(void) {
    // Enable Clock to PORTA
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    // Configure PTA12 for TPM1_CH0 function (ALT3)
    PORTA->PCR[PTA12_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[PTA12_Pin] |= PORT_PCR_MUX(3);  // Set MUX to TPM1_CH0 (ALT3)

    // Enable Clock for Timer 1 (TPM1)
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

    // Select Clock for TPM (using MCGFLLCLK, 48 MHz)
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // Set initial frequency
    setFreq(G4);
		
    // Set TPM1 to edge-aligned PWM, up-counting mode
    TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
    TPM1->SC |= (TPM_SC_PS(7));  // Set prescaler to 128
    TPM1->SC |= TPM_SC_CMOD(1);  // Enable TPM counter

    // Configure TPM1 Channel 0 for edge-aligned PWM (high-true pulses)
    TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // High-true pulses
		TPM1_C0V = 0;
}


void playOdeToJoy() {
    for (int i = 0; i <63; i++) {
				osSemaphoreAcquire(musicSem, osWaitForever);
        setFreq(melody[i]); // Set frequency for the current note
				osSemaphoreRelease(musicSem);
        // Adjust delay based on note duration for better cadence

				switch(noteDurations[i]){
						case QUARTER:
								//delay2(25000);
								osDelay(200);
								break;
						case HALF:
								//delay2(50000);
								osDelay(400);
								break;
						case EIGHTH:
								//delay2(12500);
								osDelay(100);
								break;
						default:
								// handle other cases if necessary
								break;
				}
				TPM1_C0V = 0;
				osDelay(10);
				
				
        // Small pause between notes
    }
}
C_Octave_Notes melodyPumped[] = {
    F3, F3, F3, G3,
    GS3, GS3, AS3, C4, DS4,
    DS4, DS4, C4, AS3,
    AS3, AS3, G3, GS3
};

uint8_t pumpedDurations[] = {
	QUARTERPLUS,QUARTER,EIGHTH,QUARTER,
	QUARTERPLUS,QUARTER,EIGHTH,QUARTER,
	QUARTERPLUS,QUARTER,EIGHTH,QUARTER,
	QUARTERPLUS,QUARTER,EIGHTH, EIGHTH, EIGHTH
};

void playEnding() {
  
  for (int i = 0; i <17; i++) {
				
        setFreq(melodyPumped[i]); // Set frequency for the current note

        // Adjust delay based on note duration for better cadence

				switch(pumpedDurations[i]){
						case QUARTERPLUS:
								osDelay(300*2);
								break;
						case QUARTER:
								//delay2(25000);
								osDelay(200*2);
								break;
						case HALF:
								//delay2(50000);
								osDelay(400*2);
								break;
						case EIGHTH:
								//delay2(12500);
								osDelay(100*2);
								break;
						default:
								// handle other cases if necessary
								break;
				}
				TPM1_C0V = 0;
				osDelay(10);

        // Small pause between notes
    }
  
}