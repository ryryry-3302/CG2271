# include "MKL25Z4.h"

# define LF 8 //PortC pin 8
# define LB 9 //PortC pin 9
# define RF 4 //PortA pin 4
# define RB 5 //PortA pin 5

# define MOD_VALUE 7500 //Up to 16 bits
# define CALC_CNV(x) ((MOD_VALUE*x)/100) //Input is duty cycle in %

void initMotors(){
	//Enable clock to Port C & A
	SIM_SCGC5 |= (SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK);
	
	//Clear Mux
	PORTC->PCR[LF] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[LB] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[RF] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[RB] &= ~PORT_PCR_MUX_MASK;
	
	//Setting Mux
	PORTC->PCR[LF] |= PORT_PCR_MUX(3);
	PORTC->PCR[LB] |= PORT_PCR_MUX(3);
	PORTA->PCR[RF] |= PORT_PCR_MUX(3);
	PORTA->PCR[RB] |= PORT_PCR_MUX(3);
	
	//Enable Clock to TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	//Clears TPM clock selection
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	//Selects MCGFLLCLK clock or MCGPLLCLK/2
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM0->MOD = MOD_VALUE; 
	
	//SC contains overflow status flag and control bits
	//CMOD is clock mode selection
	//PS is prescale factor selection for clock
	
	//Clears the clock selection and prescale factor
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	//Divide by 128
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	//Center-aligned PWM Select
	// LPTPM counter operates in up counting mode.
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//Channel Status and Control
	TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM0_C4V = CALC_CNV(0);
	TPM0_C5V = CALC_CNV(0);
	TPM0_C1V = CALC_CNV(0);
	TPM0_C2V = CALC_CNV(0);
}

int main(void)
{
	SystemCoreClockUpdate();
	initMotors();

	while(1)
	{
		//Going forward
		TPM0_C4V = CALC_CNV(10); // Right Forward
		TPM0_C5V = CALC_CNV(0); // Right Reverse
		TPM0_C1V = CALC_CNV(50); // Left Reverse
		TPM0_C2V = CALC_CNV(0); // Left Forward
	}
}
	
		
	


