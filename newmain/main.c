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

void move_thread (void*argument) {
	
	  for (;;) {
			move(48);
		}
}

void front_green_led_thread (void*argument) {
		for (;;){
			greenLEDControl(state);
		}
}

void back_red_led_thread (void*argument) {
		for (;;){
			redLEDControl(state);		
		}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initPWM();
	initMotors();
	initLEDs();
	//InitGPIO();
  // 
	musicSem = osSemaphoreNew(1, 1, NULL);
  osKernelInitialize();                 // Initialize CMSIS-RTOS

  osThreadNew(play_music_thread, NULL, &priorityHigh);    // Create application main thread
	osThreadNew(move_thread, NULL, &priorityHigh);
	osThreadNew(back_red_led_thread, NULL, &priorityHighled);
	osThreadNew(front_green_led_thread, NULL, &priorityHighled);

  osKernelStart();                      // Start thread execution
	for (;;) {
	}
}
