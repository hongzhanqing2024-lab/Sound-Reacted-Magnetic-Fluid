#include "arduinoFFT.h" 

// --- 参数配置区 ---
#define AUDIO_IN_PIN A0 
#define PWM_OUT_PIN 3        
#define SAMPLES 128          
#define SAMPLING_FREQ 10000 
#define MAX_PWM 200          
#define IDLE_TIMEOUT 5000   

// --- FFT 所需的变量 ---
double vReal[SAMPLES];
double vImag[SAMPLES];

// 【关键修改点】：给 ArduinoFFT 加上了 <double>，告诉库我们要用 double 类型计算
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ); 

unsigned int sampling_period_us;
unsigned long microseconds;
unsigned long lastBeatTime = 0;

void setup() {
  pinMode(PWM_OUT_PIN, OUTPUT);
  Serial.begin(115200); 
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}

void loop() {
  // === 第 1 步：音频信号采集 ===
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN);
    vImag[i] = 0;
    while(micros() - microseconds < sampling_period_us) { }
  }

  // === 第 2 步：FFT 频谱分析 ===
  // 新版库 API 调用
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); 
  FFT.compute(FFT_FORWARD);                        
  FFT.complexToMagnitude();                        

  // === 第 3 步：提取低频能量 ===
  double bassEnergy = 0;
  for (int i = 1; i <= 3; i++) {
    bassEnergy += vReal[i]; 
  }

  // === 第 4 步：映射与控制 ===
  int pwmValue = map(bassEnergy, 500, 3000, 0, 255); 
  pwmValue = constrain(pwmValue, 0, MAX_PWM); 

  if (pwmValue > 15) {
    lastBeatTime = millis();
  }

  if (millis() - lastBeatTime > IDLE_TIMEOUT) {
    pwmValue = 0;
  }

  analogWrite(PWM_OUT_PIN, pwmValue); 
}