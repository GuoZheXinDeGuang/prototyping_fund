// This combines the part to operate the motor and the emg

#include <Servo.h>

#define SAMPLE_RATE 500        // Sampling rate
#define BAUD_RATE 115200       // Serial baud rate
#define INPUT_PIN A0           // Signal input pin
#define DETECT_PIN 2           // Detection input pin
#define Vref (1.65 / 5 * 1024) // Reference voltage

// Buffer for data processing
#define BUFFER_SIZE 128           // Buffer size
int circular_buffer[BUFFER_SIZE]; // Circular buffer
int data_index, sum;

Servo esc;  // Create a Servo object to control the ESC

void setup() {
  Serial.begin(BAUD_RATE);          // Initialize serial communication with the specified baud rate
  pinMode(DETECT_PIN, INPUT);       // Set detect pin as input
  esc.attach(9);                    // Attach the ESC signal wire to pin 9
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

    if (detect_value == HIGH) {  // If detection signal is HIGH
      Serial.println(String(emgRaw) + "," + String(signal) + ",1");  // Print EMG raw, filtered signal, and detection status
      int motor_value = map(emgRaw, -512, 512, 0, 180);  // Map EMG value to motor control value (0-180)
      motor_value = constrain(motor_value, 0, 180);      // Constrain the value to be within 0-180
      esc.write(motor_value);                            // Set motor to the mapped value
    } else {  // If detection signal is LOW
      Serial.println(String(emgRaw) + ",0,0");  // Print EMG raw value and detection status
      esc.write(90);  // Set motor to stop position when not detected
    }
  }
}

/**************************** Filtering ************************************/

// >>> Butterworth IIR Digital Filter: bandpass
//  Sampling Rate: 500.0 Hz, Frequency: [70.0, 110.0] Hz
//  Order: 4.0, implemented as second-order sections (biquads)
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
