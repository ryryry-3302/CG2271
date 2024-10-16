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
- PTC8 TPM0_CH4 mux3 purple
- PTC9 TPM0_CH5 mux3 grey

the other side
- PTA4 10 TPM0_CH1 mux3 green
- PTA5 12 TPM0_CH2 mux3 blue

## UART
PTD2 RX 8 yellow tx on esp
PTD3 TX 10 orange rx on esp

## Buzzer
PTA12 8 white TPM1
