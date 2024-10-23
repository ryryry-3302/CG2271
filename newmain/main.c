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
	.stack_size = 1024
};
const osThreadAttr_t priorityMax = {
  .priority = osPriorityRealtime
};

enum State state = RUNNING;

void play_music_thread (void *argument) {
 
  // ...
  for (;;) {
		playOdeToJoy();
		led_control(GREEN);
	}
}

void move_thread (void*argument) {
	
	  for (;;) {
		//osSemaphoreAcquire(mySem, osWaitForever);	
		}
		//osSemaphoreRelease(mySem);
}

 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initPWM();
	initMotors();
	InitGPIO();
  // 
	musicSem = osSemaphoreNew(1, 1, NULL);
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(play_music_thread, NULL, &priorityHigh);    // Create application main thread
	osThreadNew(move_thread, NULL, NULL);  
  osKernelStart();                      // Start thread execution
	for (;;) {
	offRGB();
	}
}
