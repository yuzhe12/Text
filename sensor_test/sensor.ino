#include "Arduino.h"
#include "WiFiUser.h"
#include "My_time.h"
#include "My_button2.h"
#include "AsyncUDP.h"
#include "ArduinoJson.h"

int port;                                  // 设置默认的udp传输发送端口

//RTC_DATA_ATTR int bootCount = 0;        // 用于记录关机开机的次数 存放在rtc-data-attr中
//String name;
 AsyncUDP udpClient; 

void UDPSendData(String message, String ip, int port)
{
    char charBuffer[1024];
    message.toCharArray(charBuffer, 1024);  // 转成数组

    if (udpClient.connected()){
      udpClient.broadcastTo(charBuffer, port);
    } else {
      Serial.println ("Server Not Connected");
      IPAddress ipAddress = IPAddress();
      ipAddress.fromString(ip);
      udpClient.connect(ipAddress, port);   
      Serial.println ("Server Connected");
    }
}

String GetJSONString(String sensorName, float x, float y ,float z,float w, float euler, float acc, int system, float power) 
{
    StaticJsonDocument<1024> staticJsonDocument;//创建静态的JSON文档对象staticJsonDocument，缓冲区大小为1024
    staticJsonDocument["n"] = sensorName;       //设置传感器名称

    DynamicJsonDocument orientation(768);       //创建动态json文档对象orientation，缓冲区大小为768

    orientation["x"] = String(x, 4);            // 四元数 x 的值
    orientation["y"] = String(y, 4);            // 四元数 y 的值
    orientation["z"] = String(z, 4);            // 四元数 z 的值
    orientation["w"] = String(w, 4);            // 四元数 w 的值
    orientation["euler"] = String(euler, 4);    // 欧拉角
    orientation["acc"] = String(acc, 4);        // 加速度
    orientation["s"] = String(system, 2);       // 校准状态指数
    orientation["p"] = String(power, 3);        //功耗
    staticJsonDocument["o"] = orientation;      //将orientation对象存储到staticJsonDocument对象的"o"键中
    char docBuf[210];                           //创建一个字符数组docBuf作为序列化后的JSON字符串的缓冲区。
    serializeJson(staticJsonDocument, docBuf);  //将staticJsonDocument对象序列化为JSON格式，并将结果存储在docBuf缓冲区中。

    return String(docBuf);                      //将缓冲区中的JSON字符串转换为String类型，并将其返回。
}


void setup() {
    
    Serial.begin(115200);  // 初始化串口通信 设置波特率为115200  
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);  // 设置唤醒设置关机->开机 （从低功耗进入正常工作模式）；唤醒引脚为io33 ；当1 = High或 0 = Low 唤醒
    Init_timeset();  // 初始化定时器设置
    pinMode(LED, OUTPUT);  // ledio设置为输出

    Init_KeyDetectionSet();  // 按键检测初始化设置

    digitalWrite(LED, HIGH);  // 开机 开机指示灯亮 
    power = 1;  // 状态位置1
    inter = 0;
   
}

void loop() {
  // 启动按钮状态检测
  button.loop();
  // 处于开机状态
  if (power == 1)
  {
    digitalWrite(LED, HIGH);            // 状态指示灯常亮
    checkDNS_HTTP();                    // 检测客户端DNS&HTTP请求，也就是检查配网页面那部分
    checkConnect(true);                 // 检测网络连接状态，参数true表示如果断开重新连接
    // name = getSensorName();

    if(flag)//判断定时器中断标志位是否触发
  {
    // if(name == "") name = "sensor"; 
    if(port == 0) port = 8080;          // 若没输入 则默认为8080
    String json_data = GetJSONString ("name", 1, 1, 1, 1, 1, 1, 1, 0);
    UDPSendData(json_data, "127.0.0.1", port);
    flag = false;//复原标志位
  }
  }

}






