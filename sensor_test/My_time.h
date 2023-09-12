

#ifndef MY_TIME_H
#define MY_TIME_H
#include "Arduino.h"

//定时器变量声明
extern hw_timer_t *my_timer; //创建硬件定时器
extern bool flag; //创建中断标志位

//函数声明
void IRAM_ATTR timer_flag();//定时中断回调函数    作用：设置中断标志位
void Init_timeset();

#endif
