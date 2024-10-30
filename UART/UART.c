#include "MKL25Z4.h"
#define UART2_RX_PORTD2 2
#define UART_INTERRUPT_PRIORITY 128
#define BAUD_RATE 9600

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))

int data = 0;

void InitGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}


static void delay(volatile uint32_t nof) {
	while(nof!=0) {
		__asm("NOP");
		nof--;
	}
}

void offRGB()
{
	//Pull the appropriate bits to 1, which turns off all the leds
	PTB->PSOR |= MASK(RED_LED) | MASK(GREEN_LED);   //PSOR sets the bit to 1
	PTD->PSOR |= MASK(BLUE_LED);              
}

// void led_control(color_t color)
// {
// 	offRGB();
// 	switch(color)  //PCOR sets the bit to 0
// 	{
// 		case RED:  
// 			PTB->PCOR |= MASK(RED_LED);
// 			break;
		
// 		case GREEN:  
// 			PTB->PCOR |= MASK(GREEN_LED);
// 			break;
		
// 		case BLUE:  
// 			PTD->PCOR |= MASK(BLUE_LED);
// 			break;
// 	}
// }

void initUART2 (uint32_t baud_rate) {
    uint32_t divisor, bus_clock;

    // Enable UART2 and PORT D
    SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
    // Enable UART2 RX Multiplex on Port D Pin 2
    PORTD->PCR[UART2_RX_PORTD2] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[UART2_RX_PORTD2] |= PORT_PCR_MUX(3);

    // Temporarily disable transmitter and receiver
    UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
		
    // Calculate divisor for UART
    bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
    divisor = bus_clock / (baud_rate * 16);
    UART2->BDH = UART_BDH_SBR(divisor >> 8);
    UART2->BDL = UART_BDL_SBR(divisor);

    // Disable Parity bit
    UART2->C1 = 0;
    
    // Misc
    UART2->S2 = 0;
    UART2->C3 = 0;
    
    // Setup Interrupt
    NVIC_SetPriority(UART2_IRQn, UART_INTERRUPT_PRIORITY);
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_EnableIRQ(UART2_IRQn);
		
    // Enable Receiver and Receiver Interrupt
    UART2->C2 |= ((UART_C2_RIE_MASK) | (UART_C2_RE_MASK));
}

void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        data = UART2->D;
    }
}

int main()
{
	InitGPIO();
    offRGB();

    initUART2(BAUD_RATE);
    while (1) {
        if (data > 30) {
		    PTB->PCOR |= MASK(RED_LED);
        }
        else {
            offRGB();
        }
    }
}

