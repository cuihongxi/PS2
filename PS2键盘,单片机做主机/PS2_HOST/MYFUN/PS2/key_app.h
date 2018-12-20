#ifndef	__key_app_H
#define	__key_app_H

#include "PS2_KEYBOARD.H"


#define	PS2_OUT(...) printf(__VA_ARGS__)	//对外输出内容


void PS2_KeyApp(PS2_KEYBOARD_str *ps2_key);

#endif


