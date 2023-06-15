#include<reg52.h>
typedef unsigned int u16;
typedef unsigned char u8;

//定义数码管的端口
sbit D1 = P2^0;
sbit D2 = P2^1;
sbit D3 = P2^2;
sbit D4 = P2^3;
//定义按键的端口
sbit K3 = P3^2;
sbit K2 = P3^3;
//0~9的段码
u8 code table[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
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
	  TMOD =0X01; //定时器0，工作方式1，16位定时器
	  ET0=1;     
	  EA=1;      
	  second=0;
      TH0=0xFC;  
      TL0=0x18;
   	  P3=0xff;
	  P0=0x00;
}

void Display(u8 s) //数码管显示
{
	D1=0;D2=1;
	P0=table[s/10];
   	delay1ms(5);
	D1=1;D2=0;
	P0=table[s%10];
	delay1ms(5);
}


void Keyscan()
{
    if (TR0 == 0) //定时器0关闭
    {
          if(K3==0)
	        { 
	        	delay1ms(10); 
	        	if(K3==0)
	        	{  
                    TR0=1;
	        	}
	        }
			if (K2==0)
			{
				second = 0;
			}
			
    }
	delay1ms(100); 
	if (TR0 == 1)
    {
          if(K3==0)
	        { 
	        	delay1ms(10); 
	        	if(K3==0)
	        	{  
                    TR0=0;
	        	}
	        }
    }
    
}

void main()
{
	ExitInit();  //定时器初始化           
  while(1)
	{
		Display(second); //显示秒数
		Keyscan();     //按键扫描
	}
}

void Timer0() interrupt 1 //定时器0中断函数
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
        // 当second=100时，second=0，即从0开始计数
		if(second==100)  
			second=0;	
	}	
}

// Path: C\按键计时.c

