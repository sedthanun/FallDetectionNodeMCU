#include <TridentTD_LineNotify.h>
#include "MPU6050.h"
#include "I2Cdev.h"
#include "Wire.h"

#define BUTTON D6
#define LED D0
#define BUZZER_PIN D7

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx, valy, valz;
int stat = 0;
//------------------Line Notify
#define SSID "Sedthanun's IPhone"
#define PASSWORD "Bigsc2000"
#define LINE_TOKEN "my4riKK6aqeHJuVETp6GFfam01SgrphHtOJfx4YlrQc"

//------------------Line Notify

void setup() {
  //---------------------
  Wire.begin();
  Serial.begin(38400);
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER_PIN,OUTPUT);

  //------------------------ Line
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());


  // ทดสอบ Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("ไลน์นอติไฟล์ทำงาน");
}

void loop() {
  // ปรับค่าของ MPU-6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("Stat =");
  Serial.print(stat);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);
 
  if (valx >= 0 && valx <= 179 && valy >= 0 && valy <= 179) {
    if (digitalRead(BUTTON) == LOW) {
      stat = 0;
      Serial.print("Stat =");
      Serial.print(stat);
      digitalWrite(LED, LOW);
      digitalWrite(BUZZER_PIN,LOW);    
    }
    if (stat == 0) {
      Serial.print("axis x = ");
      Serial.print(valx);
      Serial.print(" axis y = ");
      Serial.print(valy);
      Serial.print(" axis z = ");
      Serial.println(valz);
      if (valx < 20) {  //ระยะล้มไปด้านหน้า
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER_PIN,HIGH);    
        Serial.println("ล้มไปด้านหน้า");
        LINE.notify("ล้มไปด้านหน้า");
        stat = 1;
      } else if (valx > 160) {  //ระยะล้มไปด้านหลัง
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER_PIN,HIGH);    
        Serial.println("ล้มไปด่้านหลัง");
        LINE.notify("ล้มไปด้านหลัง");
        stat = 1;
      }

      if (valy < 20) {  //ระยะล้มไปด้านซ้าย
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER_PIN,HIGH);    
        Serial.println("ล้มไปด้านซ้าย");
        LINE.notify("ล้มไปด้านซ้าย");
        stat = 1;
      } else if (valy > 160) {  //ระยะล้มไปด้านขวา
        digitalWrite(LED, HIGH);
        digitalWrite(BUZZER_PIN,HIGH);    
        Serial.println("ล้มไปด้านขวา");
        LINE.notify("ล้มไปด้านขวา");
        stat = 1;
      }
    }
  }
  delay(100);
}
