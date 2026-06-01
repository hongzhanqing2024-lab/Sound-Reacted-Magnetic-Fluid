#define MIC_PIN A0

//测试麦克风是否能用

void setup() {
  pinMode(13, OUTPUT); // 大多数 Pro Mini 的 13 脚连接了板载 LED
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  Serial.begin(115200); // 必须设置高波特率，否则数据传输速度跟不上声音频率
  Serial.println("System Start!"); // 这一行能证明板子在运行
}

void loop() {
  int rawValue = analogRead(MIC_PIN);
  
  // // 直接输出数值，供 Serial Plotter 绘制折线图
  Serial.println(rawValue); 
  
  // 极短的延迟，防止串口崩溃，同时保证足够高的采样率
  // delayMicroseconds(100); 

  Serial.println(analogRead(A0)); // 持续输出 A0 数值
  delay(100);
}