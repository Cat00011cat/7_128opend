#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 4
#define RST_PIN 5

//进行了一些优化，包括简化UID比较，增加延时以降低轮询频率，并添加了一些错误处理。提高系统的稳定性。

Servo myservo;
MFRC522 mfrc522(SDA_PIN, RST_PIN);

byte allowedUID1[] = {0xC3, 0x85, 0x66, 0x09};
byte allowedUID2[] = {0x1D, 0xC8, 0x6A, 0xD5, 0x04, 0x10, 0x80};

void setup() {
  myservo.attach(2);
  myservo.write(0);  
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
}

void loop() {
  Serial.println("Start Access!!!");
  delay(50);

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Check UID
    if (isUIDAllowed()) {
      grantAccess();
    } else {
      denyAccess();
    }

    // Halt and stop crypto
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

bool isUIDAllowed() {
  return memcmp(mfrc522.uid.uidByte, allowedUID1, sizeof(allowedUID1)) == 0 ||
         memcmp(mfrc522.uid.uidByte, allowedUID2, sizeof(allowedUID2)) == 0;
}

void grantAccess() {
  Serial.println("Access granted");
  myservo.write(180);
  delay(1500);
  myservo.write(0);
}

void denyAccess() {
  Serial.println("Access denied");
}
