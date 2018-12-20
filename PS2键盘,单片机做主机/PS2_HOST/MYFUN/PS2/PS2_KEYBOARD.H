#ifndef  __PS2_KEYBOARD_H   
#define  __PS2_KEYBOARD_H  

#include "UHEAD.H"
/*
使用方法：
-	创建一个PS2_KEYBOARD_str对象，并初始化他PS2_KEYBOARD_Init(&PS2_KEYBOARD); 
-	将PS2_KEYBOARD_FunInExti(PS2_KEYBOARD_str *PS2_KEYBOARD);放入时钟引脚的外部中断中
-	将PS2_KEYBOARD_FunInTimer(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period);放入定时器中断中
-	使用 PS2_KEYBORAD_GetState(PS2_KEYBOARD_str *PS2_KEYBOARD,TYPE_KEY key,EdgeMod edgemod);
	获得按键及设定的边沿MOD；0为假，1为真
*/

/********************************键盘灯初始状态*******************************************/

#define	LED_INITNUM 			1
#define	LED_INITCAPS			0
#define	LED_INITSCROLL		0

/********************************调试开关*******************************************/
#define     PS2_LOG_LEVEL   0     //DEBUG开关   

#if (PS2_LOG_LEVEL > 0)
#include "stdio.h"
#define  PS2_Log(...)         printf(__VA_ARGS__);
#else
#define PS2_Log(...)   
#endif 

/********************************引脚及定时器中断配置********************************************************/
#define     PS2_KEYBOARD_DAT_PIN       GPIOB,GPIO_Pin_13             //PS2_KEYBOARD数据引脚
#define     PS2_KEYBOARD_CLK_PIN       GPIOB,GPIO_Pin_15             //PS2_KEYBOARD时钟引脚
#define     PS2_KEYBOARD_PIN_MODE      GPIO_Mode_Out_OD              //集电极开路

#define     PS2_KEYBOARD_TIMER         1000                          //单位us,进入中断的间隔时间
#define			PS2_TIMER_MAIN						 1														 //单位ms,处理PS扫描的主函数周期	
#define     PS2_KEYBOARD_EXTI_LINE     EXTI_Line15                   //时钟的中断线

#define     SET_CLK_1         GPIO_SetBits(PS2_KEYBOARD_CLK_PIN)
#define     SET_DAT_1         GPIO_SetBits(PS2_KEYBOARD_DAT_PIN)

#define     SET_CLK_0         GPIO_ResetBits(PS2_KEYBOARD_CLK_PIN)
#define     SET_DAT_0         GPIO_ResetBits(PS2_KEYBOARD_DAT_PIN)

#define     READ_CLK_IN       GPIO_ReadInputDataBit(PS2_KEYBOARD_CLK_PIN)
#define     READ_DAT_IN       GPIO_ReadInputDataBit(PS2_KEYBOARD_DAT_PIN)


/*******************************数据类型*********************************************/

typedef     u16   	TYPE_KEYVAL;         		//定义返回的键值的数据类型
typedef			u8			TYPE_KEY;								//键值数据类型
typedef			u16			TYPE_PS2;								//PS2值的数据类型

//边沿模式
typedef	enum{
	EdgeMod_Non,							//不进行边沿触发，机打允许
	EdgeMod_PULL,							//上升沿状态，既释放状态
	EdgeMod_DOWN							//下降沿状态，按下状态
}EdgeMod;

//PS2_KEYBOARD的数据类型
typedef struct{
	
	TYPE_KEYVAL   	key_val;			//键值
	u8    					flag_15ms;    //15ms计时
	u8    					flag_2ms;     //2MS计时
	u8    					flag_30ms;    //30MS计时
	u32 						counter_15ms;
	u32 						counter_2ms;  
	u32 						counter_30ms;
	u8 							count_dat;		//记录接收到个数
	u8							ledState;			//LED状态
	u8							id;						//id号
	volatile u8			keystate[13];	//104键对应的状态
	volatile u8			edge[13];			//边沿触发标志
	
	volatile u8 		f0;						//接收到0xf0为抬起按键标志
	volatile u8 		e0;						//接收到0xe0标志
	volatile u8 		shift;				//接受到0x12标志的次数
}PS2_KEYBOARD_str;

#define			GETSIZE(x)		(sizeof(x)/sizeof(x[0]))			//获得数组大小
	
/**********************************不需要配置和修改的宏定义*****************************************************/
//接收状态

#define     PS2_KEYBOARD_OK                  0x8000                  //接收数据完成,发送成功
#define     STOP_BIT          				 			 0x200
#define     CRC_BIT           				 			 0x100
#define     PS2_KEYBOARD_ERROR               0x4000                  //接收数据错误,请求重传,发送超时
#define     PS2_KEYBOARD_NULL                0                       //还没有接收完成


//命令
#define     PS2_KEYBOARD_CMD_RESET           0XFF              //引起键盘进入 Reset 模式
#define     PS2_KEYBOARD_CMD_RESEND          0xFE              //重发
#define     PS2_KEYBOARD_CMD_SETALLA         0xFA              //所有键的通码 断码和机打重复都使能
#define     PS2_KEYBOARD_CMD_TONGMA          0xF9              //所有键都只发送通码 断码和机打重复被禁止
#define     PS2_KEYBOARD_CMD_SETDEFAULT      0xF6              //缺省值,载入缺省的机打速率/延时\
																																	10.9cps/500ms 按键类型(所有按键都使能机打/通\
																																	码/断码) 以及第二套扫描码集
#define     PS2_KEYBOARD_CMD_READID          0XF2              //读键盘ID
#define     PS2_KEYBOARD_CMD_EE              0XEE              //键盘的回应
#define     PS2_KEYBOARD_CMD_LED             0xED              //主机在本命令后跟随一个参数字节 用于指示键盘上  Caps Lock,Num Lock,\
                                                      and Scroll Lock LED 的状态   0X07


  
//LED灯
#define		LED_Num_Lock					0x02	//小键盘数字LED指示
#define		LED_Caps_Lock					0x04	//大小写LED指示
#define		LED_Scroll_Lock				0x01	//功能LED指示



/*函数*/
void   	PS2_KEYBOARD_Init(PS2_KEYBOARD_str* PS2_KEYBOARD);               
u16    	PS2_KEYBOARD_Host_Write(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 cmd);    	//主机写命令
void 		PS2_KEYBOARD_FunInExti(PS2_KEYBOARD_str *PS2_KEYBOARD);							//PS2_KEYBOARD放在外部中断中的函数，从键盘读数据
void 		PS2_KEYBOARD_FunInTimer(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period);	//PS2_KEYBOARD放在定时器中断中的函数
void 		PS2_KEYBOARD_SetLED(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 led,u8 state);//设定PS2_KEYBOARD键盘LED

u8 			PS2_KEYBORAD_GetState(PS2_KEYBOARD_str *PS2_KEYBOARD,TYPE_KEY key,EdgeMod edgemod);
void 		PS2_KEYBOARD_ReversalLED(PS2_KEYBOARD_str *PS2_KEYBOARD,u8 led);		//取反键盘LED

#endif
