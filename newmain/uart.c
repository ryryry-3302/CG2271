#include "MKL25Z4.h"
#define UART2_RX_PORTD2 2
#define UART_INTERRUPT_PRIORITY 128
#define BAUD_RATE 9600
#include "helpers.h"
#define MASK(x) (1 << (x))


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


