#include <Arduino.h>
#include <Servo.h>  // Include Servo library for PWM control

Servo esc;  // Create a Servo object to control the ESC

int x; 

void setup() {
  esc.attach(9);  // Attach the ESC signal wire to pin 9
  Serial.begin(115200);
  Serial.setTimeout(1); 
}

//value x+1
//90 means stop, 90-180 is counterclockwise, 0-90 is clockwise, 0 means full speed clockwise, 80 is slowly clockwise, 110 is slowly counterclockwise
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read until a newline character is received
    input.trim();  // Remove any extraneous newline or space characters

    x = input.toInt();  // Convert the input string to an integer
    
    // Print the received and interpreted value back to the serial port
    Serial.println(x);
    esc.write(x);
  }

  // while (!Serial.available()){
  //  x = Serial.readString().toInt();
  //  Serial.print(x + 1);
  //  esc.write(x);  
  // }
}
