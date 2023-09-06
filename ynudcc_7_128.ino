#define BLINKER_WIFI
/*小爱同学语言控制*/
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>
#include <Servo.h>

/*
   滇院智能门锁
   启动宿舍128专属
   @Cat00011cat
   @http://thecat.top
*/

Servo myservo;  /*定义mg996s大扭力伺服舵机对象*/

char auth[] = "d2daf753e335"; /*设备ID密钥*/
char ssid[] = "tttt"; /*WIFI 名称*/
char pswd[] = "1234567890"; /*WIFI 密码*/
BlinkerButton Button1("test");  /*按钮对象*/
int counter = 0;

/*电源类操作*/
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    myservo.write(200); /*收到“on”的指令 舵机旋转100度数*/
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();  /*将状态推送到app*/
    delay(1000);
    myservo.write(90);  /*舵机恢复出厂*/
  }
  else if (state == BLINKER_CMD_OFF) {
    myservo.write(70);  /*舵机偏转60度*/
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    delay(1000);
    myservo.write(90);
  }
}

void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if (state == "on")
    /*开门*/
  {
    myservo.write(180);
    delay(1000);
    myservo.write(90);


  }
  else if (state == "press" || state == "tap")
    /*关门*/
  {
    myservo.write(70);
    delay(1000);
    myservo.write(90);
  }
}
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
  counter++;
}

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
}
void loop() {
  Blinker.run();
}
