/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "helpers.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

enum State state = RUNNING;

void play_music_thread (void *argument) {
 
  // ...
  for (;;) {
		//osSemaphoreAcquire(mySem, osWaitForever);
		if(state != FINISHED){
			playOdeToJoy();
		}
		else {
			//insert other tone
		}
		//osSemaphoreRelease(mySem);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initPWM();
  // ...
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
//  mySem = osSemaphoreNew(2,2,NULL);
	osThreadNew(play_music_thread, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
	for (;;) {}
}
