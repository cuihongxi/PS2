#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "IT.H"
#include "PS2_KEYBOARD.H"
#include "key_map.h"
#include "key_app.h"


u8 ledstate = 0;
u8 capstate = 0;
PS2_KEYBOARD_str PS2_KEYBOARD;

int main(void)
{	

	delay_init();
	uart_init(115200); 
	printf("start:\r\n");
	PS2_KEYBOARD_Init(&PS2_KEYBOARD);  
		
	while(1)
	{

		PS2_KeyApp(&PS2_KEYBOARD);
		

		
	}
}
 
//定时中断,作用是给信号计时
 void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		PS2_KEYBOARD_FunInTimer(&PS2_KEYBOARD,PS2_KEYBOARD_TIMER);

		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update); //清除标志位	  
	}

    
}

 //根据CLK的中断线调用响应的中断函数
void EXTI15_10_IRQHandler()
{  
	if(EXTI_GetFlagStatus(PS2_KEYBOARD_EXTI_LINE))
	{
		PS2_KEYBOARD_FunInExti(&PS2_KEYBOARD);
		EXTI_ClearITPendingBit(PS2_KEYBOARD_EXTI_LINE);	
	}

}

