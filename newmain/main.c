/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "helpers.h"

osSemaphoreId_t brainSem, musicSem, moveSem;
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 int volatile data = 0;
 int movement_data = 0;
 enum State currentstate = IDLE;
 
 //ISR
 void UART2_IRQHandler(void) {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        data = UART2->D;
    }
}
 
const osThreadAttr_t priorityAbNormal = {
  .priority = osPriorityAboveNormal
};
const osThreadAttr_t priorityHigh = {
  .priority = osPriorityHigh,
	.stack_size = 512
};
const osThreadAttr_t priorityMax = {
  .priority = osPriorityRealtime
};

const osThreadAttr_t priorityHighled = {
  .priority = osPriorityHigh,
	.stack_size = 128
};
enum State state = RUNNING;

void play_music_thread (void *argument) {
 
  // ...
  for (;;) {
		playOdeToJoy();
	}
}

void decoder_thread (void *argument) {
 
  // ...
  for (;;) {
		if(data % 64 != movement_data){
			movement_data = data % 64;
		};
		if(data > 63){
			currentstate = FINISHED;
		}
		else if(movement_data > 16){
			currentstate = RUNNING;
		} else {
			currentstate = IDLE;
		}
	}
}

void move_thread (void*argument) {
	
	  for (;;) {
			move(movement_data);
		}
}

void front_green_led_thread (void*argument) {
		for (;;){
			greenLEDControl(currentstate);
		}
}

void back_red_led_thread (void*argument) {
		for (;;){
			redLEDControl(currentstate);		
		}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initUART2(9600);
	initPWM();
	initMotors();
	initLEDs();

	musicSem = osSemaphoreNew(1, 1, NULL);
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(decoder_thread, NULL, &priorityHigh);
  osThreadNew(play_music_thread, NULL, &priorityHigh);    // Create application main thread
	osThreadNew(move_thread, NULL, &priorityHigh);
	osThreadNew(back_red_led_thread, NULL, &priorityHighled);
	osThreadNew(front_green_led_thread, NULL, &priorityHighled);

  osKernelStart();                      // Start thread execution
	for (;;) {
	}
}
