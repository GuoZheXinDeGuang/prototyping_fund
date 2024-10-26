#include <Servo.h>

#define SAMPLE_RATE 100        // Sampling rate 
#define BAUD_RATE 115200       // Serial baud rate
#define INPUT_PIN A0           // Signal input pin
#define DETECT_PIN 2           // Detection input pin
#define Vref (1.65 / 5 * 1024) // Reference voltage
#define THRESHOLD 340          // Set an appropriate threshold for activation

Servo esc;  // Create a Servo object to control the ESC

void setup() {
  Serial.begin(BAUD_RATE);      // Initialize serial communication with the specified baud rate
  pinMode(DETECT_PIN, INPUT);   // Set detect pin as input
  esc.attach(9);                // Attach the ESC signal wire to pin 9
}

void loop() {
  // Calculate elapsed time
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  static long timer = 0;
  timer -= interval;

  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;  // Reset timer based on sampling rate
    int sensor_value = analogRead(INPUT_PIN);    // Read analog value from input pin
    int detect_value = digitalRead(DETECT_PIN);  // Read digital value from detect pin
    int signal = Filter(sensor_value);           // Apply filtering to the sensor value
    int emgRaw = signal - Vref;                  // Subtract reference voltage to get EMG raw value

    // Check if the absolute EMG value exceeds the threshold for activation
  // if (detect_value == HIGH && (abs(emgRaw) > THRESHOLD-100||abs(emgRaw)<THRESHOLD+100))
    if(!(signal > -40 && signal < 40)) {
      Serial.println(String(emgRaw) + "," + String(signal) + ",1");  // Print EMG raw, filtered signal, and detection status
      esc.write(110);  // Rotate motor clockwise at full speed
      // delay(1000);
    } else {  // If detection signal is LOW or below threshold
      Serial.println(String(emgRaw) + ",0,0");  // Print EMG raw value and detection status
      esc.write(89);  // Set motor to stop position
    }
    // delay(1000);
  }
}

/**************************** Filtering ************************************/

// Simplified Butterworth IIR Digital Filter: bandpass
float Filter(float input) {
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - (-0.55195385 * z1) - (0.60461714 * z2);  // Apply filter coefficients
    output = 0.00223489 * x + (0.00446978 * z1) + (0.00223489 * z2);  // Calculate output
    z2 = z1;  // Update state variables
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-0.86036562 * z1) - (0.63511954 * z2);  // Apply filter coefficients
    output = 1.00000000 * x + (2.00000000 * z1) + (1.00000000 * z2);  // Calculate output
    z2 = z1;  // Update state variables
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-0.37367240 * z1) - (0.81248708 * z2);  // Apply filter coefficients
    output = 1.00000000 * x + (-2.00000000 * z1) + (1.00000000 * z2);  // Calculate output
    z2 = z1;  // Update state variables
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-1.15601175 * z1) - (0.84761589 * z2);  // Apply filter coefficients
    output = 1.00000000 * x + (-2.00000000 * z1) + (1.00000000 * z2);  // Calculate output
    z2 = z1;  // Update state variables
    z1 = x;
  }

  return output;  // Return the filtered output
}
