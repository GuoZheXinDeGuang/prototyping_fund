#include <Servo.h>  // Include Servo library for PWM control

Servo esc;  // Create a Servo object to control the ESC

void setup() {
  esc.attach(9);  // Attach the ESC signal wire to pin 9

  // Start with the motor in a safe, stopped state
  esc.write(0);   // Set the throttle to 0 (ESCs typically initialize at low throttle)
  delay(1000);    // Give the ESC time to initialize
}

//90 means stop, 90-180 is counterclockwise, 0-90 is clockwise, 0 means full speed clockwise, 80 is slowly clockwise, 110 is slowly counterclockwise
void loop() {
  esc.write(110);  
  delay(5000);    

  esc.write(90);   
  delay(5000);   
}
  
