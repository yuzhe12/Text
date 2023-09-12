#include "My_button2.h"

bool power = 1;                    // 关机 开机标志位  若开机状态则power = 1 低功耗（关机）状态power = 0
int inter = 0;                     // 网络连接指示位
Button2 button;                    //创建一个按键变量button

// 长按按键一秒时的处理函数   关机
void handleTap(Button2& b) {
    // check for really long clicks
  if (b.wasPressedFor() > 1000) {  // 设置长按的时长为1000ms（1s）
    if(power != 0){

      Serial.println("22");      // 用于测试
      digitalWrite(LED, LOW);    // 指示灯灭 表示关机
      power = 0;                 // 状态为关机状态
      
      //Go to sleep now
      Serial.println("Going to sleep now");   // 用于测试
      esp_deep_sleep_start();                 // 进入低功耗模式（关机）
    }
  }
}

// 短按按键的处理函数  开机
void handleTap_2(Button2& b) {
  // check for really long clicks
  if (b.wasPressed()) {
    Serial.println("11");        // 用于测试
    digitalWrite(LED, HIGH);     // 状态指示灯亮
    power = 1;                   // 状态位置1表示开机
  }
}

// 双击按键的处理函数 进入配网模式
void handleTap_3(Button2& b) {
  // check for really long clicks
  if(power == 1){
  
    if (b.getNumberOfClicks() == 2) {   // 设置次数 2 双击
      if (inter == 1){
        inter = 0;
        return;
      }else{
        inter = inter + 1;

        // 设置状态指示灯的闪烁状态 表示进入配网中
        Serial.println("33");
        digitalWrite(LED, LOW);
        delay(100);
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(100);
        digitalWrite(LED, HIGH);

        Serial.println("正在清空网络连保存接信息.");
        restoreWiFi();              // 删除保存的wifi信息
        ESP.restart();              // 重启复位esp32
        power = 1;                  // 多余？
        Serial.println("已重启设备.");   //  多余？
      }
       
    }
  }
}  
//打印唤醒原因
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

// 按键检测初始化设置
void Init_KeyDetectionSet()
{
  button.begin(BUTTON_PIN);   //启动按键检测
  button.setTapHandler(handleTap);  // 长按，进入低功耗模式（待机）
  button.setClickHandler(handleTap_2);  // 短按，打印11，状态标志位power = 1
  button.setDoubleClickHandler(handleTap_3);  // 双击，清空网络信息，重新配网
}
       
  

