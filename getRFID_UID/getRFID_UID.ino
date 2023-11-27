#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 4
#define RST_PIN 5

MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);   // 设置波特率
  SPI.begin();     // 初始化SPI通信
  mfrc522.PCD_Init(); // 初始化MFRC522
}

void loop() {
  // 检测卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // 读取卡片序列号
    Serial.print("uid:");
    for (byte i = 0; i < mfrc522.uid.size; ++i) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    
    // 停止卡片
    mfrc522.PICC_HaltA();
    // 等待下一张卡片
    delay(1000);
  }
}
