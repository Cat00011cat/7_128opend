/**
   ==========================================
*  滇院智能门锁
*  启动宿舍128专属
*  实现功能：远程网络控制、nfc刷卡
*  @Cat00011cat
*  @http://thecat.top
*  ==========================================
*/

/**
   初始化参数
*/
#define BLINKER_WIFI  /**wifi通信*/
#define BLINKER_MIOT_LIGHT  /**小爱IOT*/

#define SDA_PIN 4 /**数据线端口*/
#define RST_PIN 5 /**重置端口*/
#define LED_PIN "D1" /**led端口*/

/**
   Blin库 用于硬件接入云端
*/
#include <Blinker.h>
/**
   舵机库
*/
#include <Servo.h>
/**
   SPI通信库
*/
#include <SPI.h>
/**
   MFRC522库 用于RFID模块通信
*/
#include <MFRC522.h>


Servo myservo;  /*定义mg996s大扭力伺服舵机对象*/

char auth[] = "d2daf753e335"; /*设备ID密钥*/
char ssid[] = "tttt"; /*WIFI 名称*/
char pswd[] = "1234567890"; /*WIFI 密码*/
BlinkerButton Button1("test");  /*按钮对象*/
int counter = 0;

/**
   spi协议与rc522 初始化参数
*/
MFRC522 mfrc522(SDA_PIN, RST_PIN); /**创建MFRC522*/


/**
   电源类操作
*/
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    myservo.write(180); /*收到“on”的指令 舵机旋转100度数*/
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();  /*将状态推送到app*/
    delay(1000);
    myservo.write(90);  /*舵机恢复出厂*/
  }
  else if (state == BLINKER_CMD_OFF) {

    //    myservo.write(70);  /*舵机偏转60度*/
    //    BlinkerMIOT.powerState("off");
    //    BlinkerMIOT.print();
    //    delay(1000);
    //    myservo.write(90);
  }
}

void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if (state == "on")
    /**
       开门  控制舵机转向角度
    */
  {
    myservo.write(180);
    delay(1000);
    myservo.write(90);


  }
  else if (state == "press" || state == "tap")
    /**
       关门
    */
  {
    /*
      myservo.write(70);
      delay(1000);
      myservo.write(90);
    */
  }
}
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
  counter++;
}

/**
   硬件初始化
*/
void setup()
{
  Serial.begin(115200);   /*串口波特率*/
  BLINKER_DEBUG.stream(Serial);
  myservo.attach(2);  /*舵机GPIO2口驱动，nodemcu的D4引脚，上拉电平。*/
  myservo.write(90);  /*上电时舵机初始化状态设置*/
  BlinkerMIOT.attachPowerState(miotPowerState);
  Blinker.begin(auth, ssid, pswd);  /*Blinker初始化*/
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);

  SPI.begin();      /**初始化SPI通信*/
  mfrc522.PCD_Init(); /** 初始化MFRC522*/
  delay(4); /**等待传感器稳定下来*/
  Serial.println("Scan your RFID card to access");  /**串口回显信息*/
}

/**
* RC522 LED 控制
*/
void Rc522_Led(){
  
 }


/**
   程序入口
*/
void loop() {
  /**
     接入服务器
  */
  Blinker.run();
  /**
    检测卡片
  */
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    /** 读取卡片序列号*/
    Serial.print("uid:");
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
      /**
       * DEBUG 用
       */
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
    }

    /**
       UID 可以直接借助第三方工具获取
       或者 直接通过串口打印到电脑上。
    */
    Serial.println("cardUID:");Serial.print(cardUID);
    Serial.println("Start Access!!!");
    /**RFID UID，可以添加多张卡 */
    if (cardUID == "c3856609" || cardUID == "1dc86ad5041080") {
      /**身份认证成功 灯闪烁2次*/
      Rc522_Led();
      Rc522_Led();
      /** 运行控制舵机*/
      myservo.write(180);
      delay(1000);
      myservo.write(90);
      Serial.println("Access SuccessFull");
    }
    else {
      /** 认证失败，蜂鸣器嘀嘀嘀 报警*/
      Serial.println("Access denied");
    }

    /** 停止卡片*/
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}
