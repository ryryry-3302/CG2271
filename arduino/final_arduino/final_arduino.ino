// Load Wi-Fi library
#include <WiFi.h>                         
#include <PS4Controller.h>
#define RXD2 16
#define TXD2 17

int mapJoyStickToNum(int x, int y) {
    float a = (float) x / 128;
    float b = (float) y / 128;
    
    // Calculate speed as a normalized value between 0 and 1
    float speed = sqrt((a * a) + (b * b)) / sqrt(2);

    // Define speed zones based on normalized speed
    int speed_code;
    if (speed < 0.2) { // Deadzone
        return 0;
    } else if (speed < 0.4) { // Low speed
        speed_code = 16;
    } else if (speed < 0.7) { // Medium speed
        speed_code = 32;
    } else { // High speed
        speed_code = 48;
    }

    // Calculate angle in radians and map it to 16 possible directions
    float angle = atan2(-a, -b);
    int angle_code = ((int) round((angle + M_PI) / (2 * M_PI) * 16)) % 16;

    // Combine speed and angle codes
    int code = speed_code + angle_code;
    return fmin(63, code);
}


int done = 0;
// Auxiliar variables to store the current output state
String output26State = "off";               

// Assign output variables to GPIO pins                       
const int output26 = 26;

// Current time
unsigned long currentTime = millis();       
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int wait30 = 30000; // time to reconnect when connection is lost.


void setup() {
  PS4.begin("A0:DD:6C:10:01:10");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);
  
}
int xaxis = 0;
int yaxis = 0;
void loop() {

  int code = 0;

  if (PS4.isConnected()){
    if(PS4.Circle() == 1){
      done = 1;
    }
    if(PS4.LStickX()){
      xaxis = PS4.LStickX();
    }
    if(PS4.LStickY()){
      yaxis = PS4.LStickY();
    }
  }
  code = mapJoyStickToNum(xaxis, yaxis);


  digitalWrite(output26, HIGH);
  int shifted_code = code << 0; 


  int buzzer_encoding;
  if(done == 1)
  {
    digitalWrite(output26, HIGH);
    // response = "Stopped turning";
    buzzer_encoding = 1 << 7;
  } else {
    buzzer_encoding = 0 << 7;
  }

  int finalEncoding = shifted_code | buzzer_encoding;


  Serial2.write(finalEncoding);
  
}
