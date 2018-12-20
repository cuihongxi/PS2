#include "key_app.h"
#include "key_map.h"
#include "stdio.h"

void PS2_KeyApp(PS2_KEYBOARD_str *ps2_key)
{
		u8 i;
/***********************************键盘灯*************************************************/

		if(PS2_KEYBORAD_GetState(ps2_key,KEY_NUM_LOCK,EdgeMod_DOWN))	//NUM led
		{
				PS2_KEYBOARD_ReversalLED(ps2_key,LED_Num_Lock);
		}
		if(PS2_KEYBORAD_GetState(ps2_key,KEY_CAPSLOCK,EdgeMod_DOWN))	//CAPSLOCK led
		{
				PS2_KEYBOARD_ReversalLED(ps2_key,LED_Caps_Lock);
		}
		
/***********************************键盘按下功能*************************************************/		
		//A~Z
		for(i = 4; i<30;i++)
		{
			if(PS2_KEYBORAD_GetState(ps2_key,i,EdgeMod_DOWN))	
			{
					PS2_OUT("%c",i+61);
			}
		}
		//0~9
		for(i = 30; i<39;i++)
		{
			if(PS2_KEYBORAD_GetState(ps2_key,i,EdgeMod_DOWN))	
			{
					PS2_OUT("%c",i+61);
			}
		}		
	

		
		
		//if(PS2_KEYBORAD_GetState(ps2_key,KEY_DOWN,EdgeMod_DOWN))	;
		//if(PS2_KEYBORAD_GetState(ps2_key,KEY_DOWN,EdgeMod_PULL))	;
			
/***********************************键盘弹起功能*************************************************/
		if(PS2_KEYBORAD_GetState(ps2_key,KEY_UP,EdgeMod_DOWN))	;		
		if(PS2_KEYBORAD_GetState(ps2_key,KEY_UP,EdgeMod_PULL))	;		
		
}

