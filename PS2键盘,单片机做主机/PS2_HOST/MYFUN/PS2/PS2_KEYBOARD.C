#include "PS2_KEYBOARD.H"
#include "MYGPIO.H"
#include "IT.H"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "key_map.h"

void   	PS2_KEYBOARD_Init(PS2_KEYBOARD_str* PS2_KEYBOARD);               
void    PS2_KEYBOARD_Host_Read(PS2_KEYBOARD_str* PS2_KEYBOARD);            	//主机读数据,根据返回值操作
u16    	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 cmd);    	//主机写命令
void 	PS2_KeyBoard_Reset(PS2_KEYBOARD_str* PS2_KEYBOARD);					//键盘复位
void 	PS2_KEYBOARD_FunInExti(PS2_KEYBOARD_str *PS2_KEYBOARD);				//PS2_KEYBOARD放在外部中断中的函数，从键盘读数据
void 	PS2_KEYBOARD_FunInTimer(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period);	//PS2_KEYBOARD放在定时器中断中的函数
void 	PS2_KEYBOARD_SetLED(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 led,u8 state);//设定PS2_KEYBOARD键盘LED
void 	PS2_InMain(PS2_KEYBOARD_str *PS2_KEYBOARD);							//主循环中的函数
void 	PS2_InTimeMain(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period);

/************************************************************************************
*-函数名称	：PS2_KEYBOARD初始化
*-参数	：
*返回值	：
*-函数功能	：时钟配置成下拉中断输入
*-创建者	：蓬莱市博硕电子科技
*/

void  PS2_KEYBOARD_Init(PS2_KEYBOARD_str* PS2_KEYBOARD)
{
	My_GPIO_Init(PS2_KEYBOARD_DAT_PIN,PS2_KEYBOARD_PIN_MODE);  //上拉输入 
	My_GPIO_Init(PS2_KEYBOARD_CLK_PIN,PS2_KEYBOARD_PIN_MODE);  //上拉输入 
	SET_CLK_1;
	SET_DAT_1;
	
	EXTI_Struct exti_stru;
	exti_stru.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_stru.NVIC_IRQChannelPreemptionPriority = 0;
	exti_stru.NVIC_IRQChannelSubPriority = 0;
	My_EXTI_Init(PS2_KEYBOARD_CLK_PIN,PS2_KEYBOARD_PIN_MODE,&exti_stru);           //配置时钟为下拉中断

	TIM_Struct structTIM;
	structTIM.TIMx = TIM2;
	structTIM.NVIC_IRQChannelPreemptionPriority = 2;
	structTIM.NVIC_IRQChannelSubPriority = 3;     
	structTIM.Prescaler = 71;
	structTIM.Period = PS2_KEYBOARD_TIMER; //根据采样频率设置周期
	TIM_Init(&structTIM);
	
	PS2_KeyBoard_Reset(PS2_KEYBOARD);

	
}


//键盘复位
void PS2_KeyBoard_Reset(PS2_KEYBOARD_str* PS2_KEYBOARD)
{
	u8 i = 0;
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,PS2_KEYBOARD_CMD_RESET);
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,0);
	PS2_KEYBOARD_SetLED(PS2_KEYBOARD,LED_Num_Lock|LED_Caps_Lock|LED_Scroll_Lock,1);
	PS2_KEYBOARD->flag_30ms = 0;
	PS2_KEYBOARD->flag_15ms = 0;
	PS2_KEYBOARD->flag_2ms = 0;
	PS2_KEYBOARD->key_val = 0;
	PS2_KEYBOARD->count_dat = 0;
	PS2_KEYBOARD->counter_15ms = 0;
	PS2_KEYBOARD->counter_2ms = 0;
	PS2_KEYBOARD->counter_30ms = 0;
	PS2_KEYBOARD->f0 = 0;
	for(i=0;i<13;i++)
	{
		PS2_KEYBOARD->keystate[i] = 0;
		PS2_KEYBOARD->edge[i] = 0;
	}
	delay_ms(500);
	PS2_KEYBOARD_SetLED(PS2_KEYBOARD,LED_Caps_Lock,LED_INITCAPS);
	PS2_KEYBOARD_SetLED(PS2_KEYBOARD,LED_Scroll_Lock,LED_INITSCROLL);
	PS2_KEYBOARD_SetLED(PS2_KEYBOARD,LED_Num_Lock,LED_INITNUM);
}

/************************************************************************************
*-函数名称	：主机读数据
*-参数	：u16 dat_receive:低8位代表数据位 9位-校验位 10位-停止位,
*返回值	：如果完全接收到数据返回DATA_OK,处理完后将该位清零,继续接收
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
  
void PS2_KEYBOARD_Host_Read(PS2_KEYBOARD_str* PS2_KEYBOARD)
{ 
      static u8 crc_q = 0;                			//奇校验
      static TYPE_KEYVAL PS2_KEYBOARD_dat = 0;        	//接收到的数据
      PS2_KEYBOARD->key_val = PS2_KEYBOARD_NULL;
	
      //在下降沿读数据
      if(READ_CLK_IN==0)
      {
		    PS2_KEYBOARD->counter_30ms = 0;
            if(PS2_KEYBOARD->count_dat == 0)      	//第一次接收到数据
            {       					
                  if(READ_DAT_IN == 0)
                  {            
					    PS2_KEYBOARD_dat = 0;
						PS2_KEYBOARD->flag_30ms	= 1;//启动计时器30ms,没收到数据就将包丢弃			  
                        PS2_KEYBOARD->count_dat ++; //接收到数据自增
						
                  }
            }else //接收数据
            {
                  PS2_KEYBOARD_dat >>= 1;
                  if(READ_DAT_IN)
                  {
                        PS2_KEYBOARD_dat |= 0X200;
                        crc_q ++;                              
                  }                        
                  PS2_KEYBOARD->count_dat ++;           //接收到数据自增  
				  
            }
            
            if(PS2_KEYBOARD->count_dat >= 11)     		//接收完11位数据
            {
                  PS2_KEYBOARD->count_dat = 0 ;                                 
                  crc_q = 0;
                  if((PS2_KEYBOARD_dat&STOP_BIT)==0 ||(crc_q&0x01) != 0) //停止位不为1或校验错误,则重新传数据   
                  {                                         /*奇校验数据前9位是奇数,加停止位的1肯定是偶数*/ 
                                                              
                        PS2_KEYBOARD_dat |= PS2_KEYBOARD_ERROR;
							
                  }else
                  {                              
                        PS2_KEYBOARD_dat |= PS2_KEYBOARD_OK;     
                  }
                  PS2_KEYBOARD->key_val = PS2_KEYBOARD_dat;
                  PS2_KEYBOARD_dat = 0;
				  PS2_KEYBOARD->flag_30ms	= 0;	
            }
      }            
        
}


//从键盘读数据
void PS2_KEYBOARD_FunInExti(PS2_KEYBOARD_str *PS2_KEYBOARD)
{
      PS2_KEYBOARD_Host_Read(PS2_KEYBOARD);
}

//主机写命令
u16 PS2_KEYBOARD_Host_Write(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 cmd)
{
      u8 i = 0;
      u8 crc_bit = 0;   
      SET_CLK_1;
      SET_DAT_1;
      Disable_ExtiLine(PS2_KEYBOARD_EXTI_LINE);
      SET_CLK_0;//下拉时钟至少100us  
      PS2_KEYBOARD->flag_15ms = 1;
      PS2_KEYBOARD->flag_2ms = 0;      
      delay_us(120);
      SET_DAT_0;//下拉数据线
      delay_us(10);
      SET_CLK_1;  //释放时钟
      
      while(READ_CLK_IN == 1 && PS2_KEYBOARD->flag_15ms);//等待时钟线被拉低
      if(PS2_KEYBOARD->flag_15ms == 0)     
      {
            Enable_ExtiLine(PS2_KEYBOARD_EXTI_LINE);
            return PS2_KEYBOARD_ERROR;//超时错误
      } 
      PS2_KEYBOARD->flag_15ms = 0;
      PS2_KEYBOARD->flag_2ms = 1;
      if(cmd&0x01) 
      {
            SET_DAT_1;
            crc_bit ++;
      }else SET_DAT_0;
      while(READ_CLK_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉高
      for(i=0;i<7;i++)
      {
            cmd >>= 1;
            while(READ_CLK_IN == 1 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉低
            if(cmd&0x01) 
            {
                  SET_DAT_1;
                  crc_bit ++;
            }else SET_DAT_0;
            while(READ_CLK_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉高  
            
      }
      //发送校验位
      while(READ_CLK_IN == 1 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉低
      if(crc_bit&0x01)
      {
            SET_DAT_0;
      }else SET_DAT_1;
      while(READ_CLK_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉高 
      
			while(READ_CLK_IN == 1 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉低	  
      SET_DAT_1;              							//释放数据线,发送停止位
			while(READ_CLK_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉高 
	  
      while(READ_DAT_IN == 1 && PS2_KEYBOARD->flag_2ms);//等待数据线被拉低，等待应答位 
      while(READ_CLK_IN == 1 && PS2_KEYBOARD->flag_2ms);//等待时钟线被拉低    
      while(READ_CLK_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待时钟线被释放 
      while(READ_DAT_IN == 0 && PS2_KEYBOARD->flag_2ms);//等待数据线被释放
      
      if(PS2_KEYBOARD->flag_2ms == 0)   
      {
            Enable_ExtiLine(PS2_KEYBOARD_EXTI_LINE);
            return PS2_KEYBOARD_ERROR;//超时错误
      }  
      Enable_ExtiLine(PS2_KEYBOARD_EXTI_LINE);
	  PS2_KEYBOARD->flag_2ms = 0;
	  PS2_KEYBOARD->flag_15ms = 0;
      return PS2_KEYBOARD_OK;
      
} 

void PS2_KEYBOARD_FunInTimer(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period)
{
    PS2_InTimeMain(PS2_KEYBOARD,period);		//键值主处理函数
	//计时在主机最初把书记现拉低后 设备开始产生时钟脉冲的时间 必须不大于 15ms
	if(PS2_KEYBOARD->flag_15ms)
	{
		PS2_KEYBOARD->counter_15ms ++;
		if(PS2_KEYBOARD->counter_15ms > 15*(1000/period)) PS2_KEYBOARD->flag_15ms = 0;
	}else PS2_KEYBOARD->counter_15ms = 0;

	//数据包被发送的时间必须不大于 2ms
	if(PS2_KEYBOARD->flag_2ms)
	{
		PS2_KEYBOARD->counter_2ms ++;
		if(PS2_KEYBOARD->counter_2ms > 2*(1000/period))PS2_KEYBOARD->flag_2ms = 0;
	}else PS2_KEYBOARD->counter_2ms = 0;

	//计时30ms超时
	if(PS2_KEYBOARD->flag_30ms)
	{
		PS2_KEYBOARD->counter_30ms ++;
		if(PS2_KEYBOARD->counter_30ms > 30*(1000/period))
		{
			PS2_KEYBOARD->flag_30ms = 0;
			PS2_KEYBOARD->count_dat = 0;
		}
	}else PS2_KEYBOARD->counter_30ms = 0;
	
}

//设定PS2_KEYBOARD键盘LED
void PS2_KEYBOARD_SetLED(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 led,u8 state)
{
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,PS2_KEYBOARD_CMD_LED);
	if(state)	
	{
		PS2_KEYBOARD->ledState |= led;
	}else
		PS2_KEYBOARD->ledState &= ~led;
	
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,PS2_KEYBOARD->ledState);

}

//取反键盘LED
void PS2_KEYBOARD_ReversalLED(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 led)
{
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,PS2_KEYBOARD_CMD_LED);
	PS2_KEYBOARD->ledState ^= led;
	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD,PS2_KEYBOARD->ledState);

}

//array数组
//key按键别名
//status状态
void PS2_SetPress(PS2_KEYBOARD_str *PS2_KEYBOARD,volatile u8* array,u8 key)
{
	u8 arraySide = key/8;			//数组下标	
	u8 byteSide = 0x01<<(key&7);	//字节中的位置
	u8 i = array[arraySide];
	if(PS2_KEYBOARD->f0 == 0)	//按下
	{
		array[arraySide] |= byteSide;
	}
	else		//抬起
	{
		array[arraySide] &= ~byteSide;
		PS2_KEYBOARD->f0 = 0;
	}
	
	if(array[arraySide] != i) 
	{
		
		PS2_KEYBOARD->edge[arraySide] |= byteSide;	//记录边沿触发
		PS2_Log("PS2_KEYBOARD->edge[%d] = 0x%x\r\n",arraySide,PS2_KEYBOARD->edge[arraySide]);
	}
	//else PS2_KEYBOARD->edge = 0;
}

//找到对应的数,0xff 没找到
u8 FineCorres(u16 dat)
{
	u8 i;
	for(i = 0;i<GETSIZE(keyMap);i++)
	{
		if(dat == keyMap[i].PS2) 
		{
			return keyMap[i].key;
		}
	}
	return 0xff;
}

//处理数据
void DisposeDate(PS2_KEYBOARD_str *PS2_KEYBOARD)
{
	u8 dat;
	if((u8)PS2_KEYBOARD->key_val == 0xf0) PS2_KEYBOARD->f0 = 0xf0;
	else if((u8)PS2_KEYBOARD->key_val == 0xe0 || (u8)PS2_KEYBOARD->key_val == 0xe1)
	{
		
		PS2_KEYBOARD->e0 = 0xe0;
//		PS2_Log("PS2_KEYBOARD->e0 = 0x%x\r\n",PS2_KEYBOARD->e0);
	}		
	else if((u8)PS2_KEYBOARD->key_val == 0x12 && PS2_KEYBOARD->e0 == 0xe0) 								//决定shift状态
	{
		if(PS2_KEYBOARD->f0 == 0)PS2_KEYBOARD->shift ++;
		else 
		{
			PS2_KEYBOARD->shift --;
			PS2_KEYBOARD->f0 = 0;
			PS2_KEYBOARD->e0 = 0;
		}	
		PS2_Log("PS2_KEYBOARD->shift = %d\r\n",PS2_KEYBOARD->shift);
		
	}
	else
	{
		//找到对应的数		
		if(PS2_KEYBOARD->e0 == 0xe0)
		{
			if(PS2_KEYBOARD->shift != 0)			
				dat = FineCorres((u16)(0x1200|(u8)PS2_KEYBOARD->key_val));			
		}
		else
			dat = FineCorres((u16)(((u16)PS2_KEYBOARD->e0<<8)|(u8)PS2_KEYBOARD->key_val));
		if(dat != 0xff)
		PS2_SetPress(PS2_KEYBOARD,PS2_KEYBOARD->keystate,dat);
		PS2_Log("dat = %d\r\n",dat);
		PS2_KEYBOARD->key_val = 0;
		PS2_KEYBOARD->e0 = 0;
		PS2_KEYBOARD->f0 = 0;
	}
	
}

//主循环中的函数
void PS2_InTimeMain(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period)
{
	static u32 counter = 0;
	counter ++;
	if(counter > (PS2_TIMER_MAIN *(1000/period)))
	{
		counter = 0;
		if(PS2_KEYBOARD->key_val != PS2_KEYBOARD_NULL)                               //有键值
		{
			if(PS2_KEYBOARD->key_val & PS2_KEYBOARD_OK)                              //成功
			{	
				PS2_KEYBOARD->key_val = (u8)PS2_KEYBOARD->key_val;
				PS2_Log("0x%x\r\n",PS2_KEYBOARD->key_val);
				DisposeDate(PS2_KEYBOARD);											//处理数据	
			}
			
			PS2_KEYBOARD->key_val = PS2_KEYBOARD_NULL;             
		}		
	}

}

//获取按键状态
//返回按键状态：0为假，1为真
//mod 可以为上升沿，下降沿

u8 PS2_KEYBORAD_GetState(PS2_KEYBOARD_str *PS2_KEYBOARD,TYPE_KEY key,EdgeMod edgemod)
{
	u8 arraySide; 						//数组下标	
	u8 byteSide;						//字节中的位置

	if(PS2_KEYBOARD->key_val == PS2_KEYBOARD_NULL)
	{
		arraySide = key/8;
		byteSide = (0x01<<(key&7));
		if((PS2_KEYBOARD->edge[arraySide] & byteSide)==0)	return 0;		//获得边沿

		if(edgemod == EdgeMod_DOWN)		//检测按下沿
		{
			 if(PS2_KEYBOARD->keystate[arraySide]&byteSide)
			 {
				PS2_KEYBOARD->edge[arraySide] &= ~byteSide;
				 return 1;
			 }				 
		}
			
		else if(edgemod == EdgeMod_PULL)//检测释放沿	
		{
			if((PS2_KEYBOARD->keystate[arraySide]&byteSide) == 0)
			{	
				PS2_KEYBOARD->edge[arraySide] &= ~byteSide;
				return 1;	
			}					
		}	
	
	}
	return 0;
}
