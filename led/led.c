#include <MKL25Z4.h>
#define MASK(x) (1 << (x))
#define DELAY_LED_RUNNING 100
#define DELAY_RLED_FLASHING_R 500
#define DELAY_RLED_FLASHING_S 250

#include "helpers.h"

int greenPins[] = {8, 9, 10, 11, 2, 3, 4, 5, 20, 21};
int greenPinsB[] = {8, 9, 10, 11};
int greenPinsE[] = {2, 3, 4, 5, 20, 21};
int redPins[] = {22, 23, 29, 30, 1, 2, 3, 2, 1, 0};
int redPinsE[] = {22, 23, 29, 30};
int redPinsC[] = {1, 2};
int redPinsB[] = {3, 2, 1, 0};

void initLEDs(void) 
{
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTC_MASK) | (SIM_SCGC5_PORTE_MASK)) ;
	
	//Green
	for(int i = 0; i < 4; i++){
		PORTB->PCR[greenPinsB[i]] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[greenPinsB[i]] |= PORT_PCR_MUX(1);
		PTB->PDDR |= MASK(greenPinsB[i]);
	}
	for(int i = 0; i < 6; i++){
		PORTE->PCR[greenPinsE[i]] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[greenPinsE[i]] |= PORT_PCR_MUX(1);
		PTE->PDDR |= MASK(greenPinsE[i]);
	}
	
	//Red
	for(int i = 0; i < 4; i++){
		PORTE->PCR[redPinsE[i]] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[redPinsE[i]] |= PORT_PCR_MUX(1);
		PTE->PDDR |= MASK(redPinsE[i]);
	}
	for(int i = 0; i < 2; i++){
		PORTC->PCR[redPinsC[i]] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[redPinsC[i]] |= PORT_PCR_MUX(1);
		PTC->PDDR |= MASK(redPinsC[i]);
	}
	for(int i = 0; i < 4; i++){
		PORTB->PCR[redPinsB[i]] &= ~PORT_PCR_MUX_MASK;
		PORTB->PCR[redPinsB[i]] |= PORT_PCR_MUX(1);
		PTB->PDDR |= MASK(redPinsB[i]);
	}
}

void setLED(GPIO_Type *gpio, int pinNumber, int on){
	if(on){
		gpio->PSOR |= MASK(pinNumber); 
	} else{
		gpio->PCOR |= MASK(pinNumber); 
	}
}

void flashLED(GPIO_Type *gpio, int pinNumber){
	setLED(gpio, pinNumber, 1);
	osDelay(100);
	setLED(gpio, pinNumber, 0);
}

void offAllGreenLEDs(){
	for(int i = 0; i < 4; i++){
		setLED(PTB, greenPinsB[i], 0);
	}
	for(int i = 0; i < 6; i++){
		setLED(PTE, greenPinsE[i], 0);
	}
}

void onAllGreenLEDS(){
	for(int i = 0; i < 4; i++){
		setLED(PTB, greenPinsB[i], 1);
	}
	for(int i = 0; i < 6; i++){
		setLED(PTE, greenPinsE[i], 1);
	}
}

void greenRunning(){
	offAllGreenLEDs();
	for(int i = 0; i < 4; i++){
		flashLED(PTB, greenPinsB[i]);
	}
	for(int i = 0; i < 6; i++){
		flashLED(PTE, greenPinsE[i]);
	}
}

void greenStationary(){
	onAllGreenLEDS();
}

void greenLEDControl(enum State state){
	if(state == RUNNING){
		greenRunning();
	} else{
		greenStationary();
	}
}

void onAllRedLEDs(){
	for(int i = 0; i < 4; i++){
		setLED(PTE, redPinsE[i], 1);
	}
	for(int i = 0; i < 2; i++){
		setLED(PTC, redPinsC[i], 1);
	}
	for(int i = 0; i < 4; i++){
		setLED(PTB, redPinsB[i], 1);
	}	
}

void offAllRedLEDs(){
	for(int i = 0; i < 4; i++){
		setLED(PTE, redPinsE[i], 0);
	}
	for(int i = 0; i < 2; i++){
		setLED(PTC, redPinsC[i], 0);
	}
	for(int i = 0; i < 4; i++){
		setLED(PTB, redPinsB[i], 0);
	}	
}

void redRunning(){
	onAllRedLEDs();
	osDelay(DELAY_RLED_FLASHING_R);
	offAllRedLEDs();
}

void redStationary(){
	onAllRedLEDs();
	osDelay(DELAY_RLED_FLASHING_S);
	offAllRedLEDs();
}	
	
void redLEDControl(enum State state){
	if(state == RUNNING){
		redRunning();
	} else{
		redStationary();
	}
}

/*
static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
	}
}

int main (void) {
	initLEDs();
	SystemCoreClockUpdate();

	enum State state = IDLE;
	
	while(1){	
		redLEDControl(state);
		greenLEDControl(state);
		delay(0x80000);//osDelay(DELAY_RLED_FLASHING_R);
	}
}
*/
