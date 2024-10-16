# CG2271

## RTOS Architecture Minimum Requirements:
The architecture should have a minimum of 4 tasks and 1 Interrupt. The names of the Tasks and ISR
are for reference only. You can choose your own names.
- tBrain: Decode the data from the Serial Port and perform the necessary action
- tMotorControl: Control the Action of the Motors
- tLED: Control the LED’s
- tAudio: Provide Audio Output
- Serial_ISR: The Serial Data received by the ESP32 device. The serial data MUST be captured through
the use of Interrupts.

# Pinout
## Motor Driver
1side
PTA1 2
PTA2 4

the other side
PTA4 10
PTA5 12

## UART
PTD2 RX 8
PTD3 TX 10
