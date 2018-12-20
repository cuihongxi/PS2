使用方法：
-	创建一个PS2_KEYBOARD_str对象，并初始化他PS2_KEYBOARD_Init(&PS2_KEYBOARD); 
-	将PS2_KEYBOARD_FunInExti(PS2_KEYBOARD_str *PS2_KEYBOARD);放入时钟引脚的外部中断中
-	将PS2_KEYBOARD_FunInTimer(PS2_KEYBOARD_str *PS2_KEYBOARD,u32 period);放入定时器中断中
-	使用 PS2_KEYBORAD_GetState(PS2_KEYBOARD_str *PS2_KEYBOARD,TYPE_KEY key,EdgeMod edgemod);
	获得按键及设定的边沿MOD；0为假，1为真

QQ: 108287205