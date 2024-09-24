#define SAMPLE_RATE 500        // 采样率
#define BAUD_RATE 115200       // 串口波特率
#define INPUT_PIN A0           // 信号输入
#define DETECT_PIN 2           // 检测输入
#define Vref (1.65 / 5 * 1024) // 抬升电压

// 窗口
#define BUFFER_SIZE 128           // 窗口大小
int circular_buffer[BUFFER_SIZE]; // 环形数组
int data_index, sum;              // 数据索引
  
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(DETECT_PIN, INPUT); // 设置DETECT_PIN为输入模式
}

void loop() {
  // 计算经过的时间
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  static long timer = 0;
  timer -= interval;

  static bool ledoffFlag = 1;

  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;
    int sensor_value = analogRead(INPUT_PIN);
    int detect_value = digitalRead(DETECT_PIN); // 读取DETECT_PIN的数字信号
    int signal = Filter(sensor_value);
    int emgRaw = signal - Vref;

    if (detect_value == HIGH) {
      Serial.println(String(emgRaw) + "," + String(signal) + ",1");

    } else {
      Serial.println(String(emgRaw) + ",0,0");
    }
  }
}

/****************************滤波************************************/

// >>> Butterworth IIR Digital Filter: bandpass
// 	Sampling Rate:500.0 Hz ,Frequency:[70.0, 110.0] Hz
// 	Order: 4.0 ,implemented as second-order sections (biquads)
float Filter(float input) {
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - (-0.55195385 * z1) - (0.60461714 * z2);
    output = 0.00223489 * x + (0.00446978 * z1) + (0.00223489 * z2);
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-0.86036562 * z1) - (0.63511954 * z2);
    output = 1.00000000 * x + (2.00000000 * z1) + (1.00000000 * z2);
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-0.37367240 * z1) - (0.81248708 * z2);
    output = 1.00000000 * x + (-2.00000000 * z1) + (1.00000000 * z2);
    z2 = z1;
    z1 = x;
  }

  {
    static float z1, z2; // filter section state
    float x = output - (-1.15601175 * z1) - (0.84761589 * z2);
    output = 1.00000000 * x + (-2.00000000 * z1) + (1.00000000 * z2);
    z2 = z1;
    z1 = x;
  }

  return output;
}
