# include "MKL25Z4.h"

# define LF 8 //PortC pin 8
# define LB 9 //PortC pin 9
# define RF 4 //PortA pin 4
# define RB 5 //PortA pin 5

# define RIGHT_FORWARD TPM0_C4V
# define RIGHT_REVERSE TPM0_C5V
# define LEFT_FORWARD	 TPM0_C2V
# define LEFT_REVERSE  TPM0_C1V 




# define MOD_VALUE 7500 //Up to 16 bits
# define CALC_CNV(x) ((MOD_VALUE*x)/100) //Input is duty cycle in %

void turnRight(int speed){
		TPM0_C5V = CALC_CNV(speed); // Right Forward
		TPM0_C4V = CALC_CNV(0); // Right Reverse
		TPM0_C2V = CALC_CNV(speed); // Left Reverse
		TPM0_C1V = CALC_CNV(0); // Left Forward
}
void turnLeft(int speed){
		TPM0_C4V = CALC_CNV(speed); //
		TPM0_C5V = CALC_CNV(0); // 
		TPM0_C1V = CALC_CNV(speed); // 
		TPM0_C2V = CALC_CNV(0); // 
}

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
enum Direction {
	NORTHEAST = 0,
	EASTSOUTH = 1,
	SOUTHWEST = 2,
	WESTNORTH = 3 
};


void move(int movebits){
    // Scale x and y to range -3 to 3 (centered at 0)
		int direction = ((movebits >> 2) % 4);
		int speed = ((movebits >> 4) * 100 /3) ;
		int ratio = (movebits % 4);
		int weakspeed;
	
		switch(direction){
			case NORTHEAST:
					if(ratio == 0){
						LEFT_FORWARD = CALC_CNV(speed);
						LEFT_REVERSE = CALC_CNV(0);
						
						RIGHT_FORWARD = CALC_CNV(speed);
						RIGHT_REVERSE = CALC_CNV(0);
					}
					else {
						weakspeed = ((4 - ratio) * speed)/(4*5);
						LEFT_FORWARD = CALC_CNV(speed);
						LEFT_REVERSE = CALC_CNV(0);
						
						RIGHT_FORWARD = CALC_CNV(weakspeed);
						RIGHT_REVERSE = CALC_CNV(0);
					}
				break;
			case EASTSOUTH:
					weakspeed = ((ratio) * speed)/(4*5);
					RIGHT_FORWARD = CALC_CNV(0);
					if (weakspeed == 0){
						turnRight(speed);
					}
					else {
						LEFT_FORWARD = CALC_CNV(0);
						LEFT_REVERSE = CALC_CNV(speed);
						RIGHT_REVERSE = CALC_CNV(weakspeed);
					}
				
				break;
			case SOUTHWEST:
					if (ratio == 0){
						RIGHT_FORWARD = CALC_CNV(0);
						RIGHT_REVERSE = CALC_CNV(speed);
						
						LEFT_FORWARD = CALC_CNV(0);
						LEFT_REVERSE = CALC_CNV(speed);
					}
					else {
						weakspeed = ((4 - ratio) * speed)/(4*5);
						RIGHT_FORWARD = CALC_CNV(0);
						RIGHT_REVERSE = CALC_CNV(speed);
						
						LEFT_FORWARD = CALC_CNV(0);
						LEFT_REVERSE = CALC_CNV(weakspeed);
					}
				break;
			case WESTNORTH:
			
					weakspeed = ((ratio) * speed)/(4*5);
					if (weakspeed == 0){
						turnLeft(speed);
					}
					else {

						RIGHT_FORWARD = CALC_CNV(speed);
						RIGHT_REVERSE = CALC_CNV(0);
						
						LEFT_FORWARD = CALC_CNV(weakspeed);
						LEFT_REVERSE = CALC_CNV(0);
					}
					
				break;	
			
		}
		
}





void stop (){
		TPM0_C4V = CALC_CNV(0); // Right Forward
		TPM0_C5V = CALC_CNV(0); // Right Reverse
		TPM0_C1V = CALC_CNV(0); // Left Reverse
		TPM0_C2V = CALC_CNV(0); // Left Forward
}
/*
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
*/
		
	


