# include "MKL25Z4.h"

# define UART_TX_PORTD2 2
# define UART_RX_PORTD3 3

volatile uint8_t rx_info;

void init_UART(uint32_t baud_rate){
	uint32_t divisor, bus_clock;
	
	//Enabling power for uart and portd
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	//Setting pins to uart
	PORTD->PCR[UART_TX_PORTD2] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_TX_PORTD2] |= PORT_PCR_MUX(3);
	PORTD->PCR[UART_RX_PORTD3] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[UART_RX_PORTD3] |= PORT_PCR_MUX(3);

	//Uart2 control register 2: Disabling transmit and receive
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	//Calculating and setting divisor for a particular baud_rate
	bus_clock = DEFAULT_SYSTEM_CLOCK/2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	//Uart2 control register 2: Enable receive
	//UART2->C2 &= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	UART2->C2 |= UART_C2_RE_MASK;
	
	//Setting and enabling UART IRQ
	NVIC_SetPriority(UART2_IRQn, 128); 
	NVIC_ClearPendingIRQ(UART2_IRQn); 
	NVIC_EnableIRQ(UART2_IRQn);
	
	//Enable interrupts for UART receiving
	//UART2->C2 |= UART_C2_TIE_MASK | UART_C2_RIE_MASK;
	UART2->C2 |= UART_C2_RIE_MASK;
}

void UART2_IRQHandler(){
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	//When rx_info is received, update global volatile variable
	if(UART2->S1 & UART_S1_RDRF_MASK){
		rx_info = UART2->D;
	}
}


	
	
	
	
	
	
	
	
	
	
	
}