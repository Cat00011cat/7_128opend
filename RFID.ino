#define SDA_PIN 4
#define RST_PIN 5

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo myservo;
MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup()
{
  myservo.attach(2);  /*舵机GPIO2口驱动，nodemcu的D4引脚，上拉电平。*/
  myservo.write(90);  /*上电时舵机初始化状态设置*/
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
}

void loop() {
  Serial.println("Start Access!!!");
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println(cardUID);
    if (cardUID == "c3856609" || cardUID == "1dc86ad5041080") {
      myservo.write(180);
      delay(1500);
      myservo.write(90);
    }
    else {
      Serial.println("Access denied");
    }
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}
