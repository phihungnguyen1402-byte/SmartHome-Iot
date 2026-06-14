#define BLYNK_TEMPLATE_ID "TMPL6VAS2g1i2"
#define BLYNK_TEMPLATE_NAME "DIEU KHIEN THIET BI"
#define BLYNK_AUTH_TOKEN "xVEt-abYJSMXmqq0Ld-1TDlRFg4ecLya"
#include "wifiConfig.h"
#include <BlynkSimpleEsp32.h>
bool blynkConnect=0;
const int led1=14, led2=27, led3=26, led4=25;

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
// Định nghĩa các chân TFT
#define TFT_CS   5
#define TFT_DC   22
#define TFT_RST  21
// Định nghĩa các chân cảm ứng
#define TOUCH_CS 4
// Định nghĩa chân LED
#define led1 14
#define led2 27
#define led3 26
#define led4 25
// Khởi tạo màn hình và cảm ứng
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS);
// Tọa độ và kích thước của switch
int switch1X = 25, switch1Y = 100, switchWidth = 80, switchHeight = 80;
int switch2X = 130, switch2Y = 100;
int switch3X = 25, switch3Y = 205;
int switch4X = 130, switch4Y = 205;
// Trạng thái bật/tắt của đèn
bool isLed1On = false;
bool isLed2On = false;
bool isLed3On = false;
bool isLed4On = false;
// Hàm vẽ switch
void drawSwitch(int x, int y, bool state, const char* label) {
  if (state) {
    tft.fillRoundRect(x, y, switchWidth, switchHeight, 10, ILI9341_GREEN);
    tft.setTextColor(ILI9341_BLACK);
  } else {
    tft.fillRoundRect(x, y, switchWidth, switchHeight, 10, ILI9341_RED);
    tft.setTextColor(ILI9341_WHITE);
  }
  tft.setCursor(x + 18, y + 30);
  tft.print(label);
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1,V2,V3,V4);
}
BLYNK_WRITE(V1) {
  int p = param.asInt();
  digitalWrite(led1, p);
  isLed1On = digitalRead(led1);
  drawSwitch(switch1X, switch1Y, isLed1On, "LED1");
}
BLYNK_WRITE(V2) {
  int p = param.asInt();
  digitalWrite(led2, p);
  isLed2On = digitalRead(led2);
  drawSwitch(switch2X, switch2Y, isLed2On, "LED2");
}
BLYNK_WRITE(V3) {
  int p = param.asInt();
  digitalWrite(led3, p);
  isLed3On = digitalRead(led3);
  drawSwitch(switch3X, switch3Y, isLed3On, "LED3");
}
BLYNK_WRITE(V4) {
  int p = param.asInt();
  digitalWrite(led4, p);
  isLed4On = digitalRead(led4);
  drawSwitch(switch4X, switch4Y, isLed4On, "LED4");
}

void setup() {
  Serial.begin(115200);
  wifiConfig.begin();

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);

  // Khởi động màn hình và cảm ứng
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);

  if (ts.begin()) {
    Serial.println("Touchscreen started.");
  } else {
    Serial.println("Failed to initialize touchscreen.");
  }
  ts.setRotation(0);
  // Hiển thị văn bản
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(50, 10);
  tft.println("DTM E-SMART");
  // Vẽ switch ban đầu
  drawSwitch(switch1X, switch1Y, isLed1On, "LED1");
  drawSwitch(switch2X, switch2Y, isLed2On, "LED2");
  drawSwitch(switch3X, switch3Y, isLed3On, "LED3");
  drawSwitch(switch4X, switch4Y, isLed4On, "LED4");
}
void loop() {
  wifiConfig.run();

  if(WiFi.status()==WL_CONNECTED){
    if(blynkConnect==0){
      Serial.println("Connecting to blynk cloud...!");
      if(Blynk.connect(5000)){ 
        Serial.println("Connected to blynk cloud!");
        blynkConnect=1;
      }else{
        Serial.println("Connection failed. Try again later.");
      }
    }
    if (!Blynk.connected()) blynkConnect=0;
    Blynk.run();
  }

  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    // Điều chỉnh tọa độ cảm ứng
    int x = map(p.x, 3900, 350, 0, tft.width());
    int y = map(p.y, 3900, 350, 0, tft.height());
    if (ts.tirqTouched()) {
      if (ts.touched()) {
        TS_Point p = ts.getPoint();
        Serial.print("Pressure = ");
        Serial.print(p.z);
        Serial.print(", x = ");
        Serial.print(p.x);
        Serial.print(", y = ");
        Serial.print(p.y);
        delay(30);
        Serial.println();
      }
    }
    // Kiểm tra nếu chạm vào switch 1
    if (x > switch1X && x < (switch1X + switchWidth) && y > switch1Y && y < (switch1Y + switchHeight)) {
      isLed1On = !isLed1On; // Đổi trạng thái bật/tắt
      digitalWrite(led1, isLed1On ? HIGH : LOW); // Bật/tắt đèn LED 1
      drawSwitch(switch1X, switch1Y, isLed1On, "LED1"); // Cập nhật giao diện switch 1
      Blynk.virtualWrite(V1,isLed1On);
      delay(200); // Tránh nhận nhiều lần chạm
    }

    // Kiểm tra nếu chạm vào switch 2
    if (x > switch2X && x < (switch2X + switchWidth) && y > switch2Y && y < (switch2Y + switchHeight)) {
      isLed2On = !isLed2On; // Đổi trạng thái bật/tắt
      digitalWrite(led2, isLed2On ? HIGH : LOW); // Bật/tắt đèn LED 2
      drawSwitch(switch2X, switch2Y, isLed2On, "LED2"); // Cập nhật giao diện switch 2
      Blynk.virtualWrite(V2,isLed2On);
      delay(200); // Tránh nhận nhiều lần chạm
    }
    // Kiểm tra nếu chạm vào switch 3
    if (x > switch3X && x < (switch3X + switchWidth) && y > switch3Y && y < (switch3Y + switchHeight)) {
      isLed3On = !isLed3On; // Đổi trạng thái bật/tắt
      digitalWrite(led3, isLed3On ? HIGH : LOW); // Bật/tắt đèn LED 3
      drawSwitch(switch3X, switch3Y, isLed3On, "LED3"); // Cập nhật giao diện switch 3
      Blynk.virtualWrite(V3,isLed3On);
      delay(200); // Tránh nhận nhiều lần chạm
    }
    // Kiểm tra nếu chạm vào switch 2
    if (x > switch4X && x < (switch4X + switchWidth) && y > switch4Y && y < (switch4Y + switchHeight)) {
      isLed4On = !isLed4On; // Đổi trạng thái bật/tắt
      digitalWrite(led4, isLed4On ? HIGH : LOW); // Bật/tắt đèn LED 4
      drawSwitch(switch4X, switch4Y, isLed4On, "LED4"); // Cập nhật giao diện switch 4
      Blynk.virtualWrite(V4,isLed4On);
      delay(200); // Tránh nhận nhiều lần chạm
    }
  }
}