#ifndef __MYBUTTON2__
#define __MYBUTTON2__

#include "Button2.h"
#include "WiFiUser.h"

//变量声明
#define LED 16                   // 状态指示灯（led双色灯珠）连接的io口为16          
#define BUTTON_PIN 33             // 按键连接的io口为io0                 
extern Button2 button;           // 创建一个按键功能的对象  详情参考 Button2的官方文档
extern bool power;               // 关机 开机标志位  若开机状态则power = 1 低功耗（关机）状态power = 0
extern int inter;                // 网络连接指示位
//函数声明
void Init_KeyDetectionSet();     // 按键检测初始化设置
void print_wakeup_reason();      // 打印esp32唤醒原因

#endif