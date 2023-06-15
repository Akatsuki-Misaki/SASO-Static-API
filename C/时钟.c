/*

  く__,.ヘヽ.        /  ,ー､ 〉
           ＼ ', !-─‐-i  /  /´
           ／｀ｰ'       L/／｀ヽ､
         /   ／,   /|   ,   ,       ',
       ｲ   / /-‐/  ｉ  L_ ﾊ ヽ!   i
        ﾚ ﾍ 7ｲ｀ﾄ   ﾚ'ｧ-ﾄ､!ハ|   |
          !,/7 '0'     ´0iソ|    |
          |.从"    _     ,,,, / |./    |
          ﾚ'| i＞.､,,__  _,.イ /   .i   |
            ﾚ'| | / k_７_/ﾚ'ヽ,  ﾊ.  |
              | |/i 〈|/   i  ,.ﾍ |  i  |
             .|/ /  ｉ：    ﾍ!    ＼  |
              kヽ>､ﾊ    _,.ﾍ､    /､!
              !'〈//｀Ｔ´', ＼ ｀'7'ｰr'
              ﾚ'ヽL__|___i,___,ンﾚ|ノ
                  ﾄ-,/  |___./
                  'ｰ'    !_,.:

四位数码管STC89C52RC单片机实现简易时钟外加闹钟功能
Four digital tube STC89C52RC microcontroller to achieve a simple clock plus alarm clock function
创建于2023/6/15 14:13
Created on 2023/6/15 14:13
*/
#include<reg52.h>
typedef unsigned int u16;
typedef unsigned char u8;

//定义数码管的端口
sbit D1 = P2^0;
sbit D2 = P2^1;
sbit D3 = P2^2;
sbit D4 = P2^3;
//如果D1 = 0则选中
//如果D1 = 1则不选中


//定义按键的端口
sbit K3 = P3^5; //切换显示时间和设置时间和设置闹钟的时间
sbit K4 = P3^4; //切换小时和分钟的时间设置
sbit K5 = P3^3; //当设置时钟的时间、设置闹钟的时间，用来增加时间、可减少小时和分钟通过K4切换
sbit K6 = P3^2; //当设置时钟的时间、设置闹钟的时间，用来减少时间、可减少小时和分钟通过K4切换

//判断按钮是否按下
int K3UPdown;
int K4UPdown;
int K5UPdown;
int K6UPdown;

// 设置默认时间
//12:00:00
u8 hour = 12;
u8 minute = 0;

// 设置默认闹钟时间
//12:01:00
u8 bellhour = 12;
u8 bellminute = 1;
u8 bellsecond = 0;

//默认显示时钟
u8 timekey = 0;

//小时和分分钟的切换状态
u8 Statukey = 0; //Statukey = 0 小时设置、Statukey = 1 分钟设置
//定义蜂鸣器的端口
sbit beep = P3^6;
//0~9的段码
u8 code table[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; //0~9的段码 共阳极数码管
u8 code tablecode[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0e};//0~9的段码 共阳极数码管带小数点
u8 second;
u8 key=0;

// 1ms*1000=1s
void delay1ms(u16 t)
{
	u16 i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<120;j++);	
	}
}

void ExitInit() //定时器初始化
{
	  TMOD =0X01;
	  ET0=1;     
	  EA=1;      
	  second=0; //秒数计时
      TH0=0xFC;  
      TL0=0x18;
   	  P3=0xff;
	  P0=0x00;
}

void Display() //数码管显示
{
    //切换显示时间设置或者闹钟设置
    //如果key = 0则显示时间设置，key = 1则显示闹钟设置
    if(timekey == 0 || timekey == 1){ //切换显示时间或者时间设置
        //显示时间设置
        D1 = 0; D2 = 1; D3 = 1; D4 = 1; //选中第一个数码管
	    P0 = table[hour / 10]; //显示s的十位
	    delay1ms(3); 
	    D1 = 1; D2 = 0; D3 = 1; D4 = 1; 
	    P0 = tablecode[hour % 10]; 
	    delay1ms(3); 
	    D1 = 1; D2 = 1; D3 = 0; D4 = 1; 
	    P0 = table[minute / 10]; //显示s的十位
	    delay1ms(3); 
	    D1 = 1; D2 = 1; D3 = 1; D4 = 0; 
	    P0 = table[minute % 10]; 
	    delay1ms(3); 
    }
    
    // 闹钟设置
    if (timekey==2) //闹钟设置
    {
        D1 = 0; D2 = 1; D3 = 1; D4 = 1; //选中第一个数码管
        P0 = table[bellhour / 10]; //显示s的十位
        delay1ms(3); 
        D1 = 1; D2 = 0; D3 = 1; D4 = 1; 
        P0 = tablecode[bellhour % 10]; 
        delay1ms(3); 
        D1 = 1; D2 = 1; D3 = 0; D4 = 1; 
        P0 = table[bellminute / 10]; //显示s的十位
        delay1ms(3); 
        D1 = 1; D2 = 1; D3 = 1; D4 = 0; 
        P0 = table[bellminute % 10]; 
        delay1ms(3); 
    }
    
    
}
void Timer0() interrupt 1 //定时器0中断函数 检查是否到达闹钟时间
{
	static u16 timer;
	TH0=0XFC;	        
	TL0=0X18;
	timer++;
    //1ms*1000=1s
	if(timer==1000)
    {
        timer=0;
        second++;
        if(second==60)
        {
            second=0;
            minute++;
            if(minute==60)
            {
                minute=0;
                hour++;
                if(hour==24)
                {
                    hour=0;
                }
            }
        }
    }
}
void main()
{
    int timex;
    ExitInit();
    TR0 = 1;
    while(1)
	{   
        if(timekey == 0)//时钟模式才响铃
        {
            if (hour == bellhour && minute == bellminute && second == bellsecond)
            {
                for(timex=0;timex<3;timex++)
                {
                    beep = 0;
                    delay1ms(1);
                    beep = 1;
                    delay1ms(1);
                }
            }
        }
        // 如果计时器开启则进行以下判断
        if(timekey==0)
        {
            TR0 = 1;
        }
        if(timekey==1 || timekey==2)
        {
            // 停止计时
            TR0 = 0;
            //归零
            second = 0;
        }

		Display(); //数码管显示

        //切换显示时间或者时间设置或者闹钟设置
        //timekey = 0 显示时间 timekey = 1 时间设置 timekey = 2 闹钟设置
        //当进入时间设置或者闹钟设置时，timekey = 1或者timekey = 2 则归零秒数计时并暂停
        if (K3 == 0) //切换显示时间和设置时间和设置闹钟的时间
        {
            delay1ms(5);
            if (K3 == 0)
            {
                if (K3UPdown == 0)
                {
                    key++;
                    K3UPdown = 1;
                    if (key == 3)
                    {
                        key = 0;
                    }
                    if (key == 0)
                    {
                        timekey = 0;
                    }
                    if (key == 1)
                    {
                        timekey = 1;
                    }
                    if (key == 2)
                    {
                        timekey = 2;
                    }
                }
            }
        }
        if(K3UPdown == 1)//恢复按键状态
        {
            if (K3 == 1) 
            {
                delay1ms(5);
                if (K3 == 1)
                {
                    K3UPdown = 0;
            	}
            }
        }

        
        //切换小时和分钟的时间设置
        if (K4 == 0) 
        {
            delay1ms(5);
            if (K4 == 0)
            {
                if (K4UPdown == 0)
                {
                    Statukey++;
                    K4UPdown = 1;
                    if (Statukey == 2)
                    {
                        Statukey = 0;
                    }
                }
            }
	    }
        if(K4UPdown == 1) //恢复按键状态
        {
            if (K4 == 1)
            {
                delay1ms(5);
                if (K4 == 1)
                {
                    K4UPdown = 0;
            	}
            }
        }
        //K5用来增加时间、小时和分钟通过K4切换
        if(K5 == 0)
        {
            delay1ms(5);
            if (K5 == 0)
            {
                if (K5UPdown == 0)
                {
                    K5UPdown = 1;
                    if (timekey == 1)
                    {
                        if (Statukey == 0)
                        {
                            hour++;
                            if (hour == 24)
                            {
                                hour = 0;
                            }
                        }
                        if (Statukey == 1)
                        {
                            minute++;
                            if (minute == 60)
                            {
                                minute = 0;
                            }
                        }
                    }
                    if(timekey == 2)
                    {
                        if (Statukey == 0)
                        {
                            bellhour++;
                            if (bellhour == 24)
                            {
                                bellhour = 0;
                            }
                        }
                        if (Statukey == 1)
                        {
                            bellminute++;
                            if (bellminute == 60)
                            {
                                bellminute = 0;
                            }
                        }
                    }
                }
                    //当设置时钟的时间、设置闹钟的时间，用来增加时间、可减少小时和分钟通过K4切换

            }
        }
        if(K5UPdown == 1) //恢复按键状态
        {
            if (K5 == 1)
            {
                delay1ms(5);
                if (K5 == 1)
                {
                    K5UPdown = 0;
            	}
            }
        }
        if(K6 == 0)
        {
            delay1ms(5);
            if(K6 == 0)
            {
                if (K6UPdown == 0)
                {
                    K6UPdown = 1;
                    //当设置时钟的时间、设置闹钟的时间，用来减少时间、可减少小时和分钟通过K4切换
                    if (timekey == 1)
                    {
                        if (Statukey == 0)
                        {
                            hour--;
                            Display(); //数码管显示
                            if (hour == 255)
                            {
                                hour = 23;
                            }
                        }
                        if (Statukey == 1)
                        {
                            minute--;
                            if (minute == 255)
                            {
                                minute = 59;
                            }
                        }
                    }
                    if(timekey == 2)
                    {
                        if (Statukey == 0)
                        {
                            bellhour--;
                            if (bellhour == 255)
                            {
                                bellhour = 23;
                            }
                        }
                        if (Statukey == 1)
                        {
                            bellminute--;
                            if (bellminute == 255)
                            {
                                bellminute = 59;
                            }
                        }
                    }
                }
            }
        }
        if(K6UPdown == 1) //恢复按键状态
        {
            if (K6 == 1)
            {
                delay1ms(5);
                if (K6 == 1)
                {
                    K6UPdown = 0;
            	}
            }
        }
    }
}
//Path: SASO-STATIC-API/C/时钟.c
// Compare this snippet from C/按键计时.c:
// Author: Syayuri
// E-mail: syayuri@outlook.jp