#ifndef	__KEY_MAP_H
#define	__KEY_MAP_H

#include "PS2_KEYBOARD.H"


//在索引表中的位置 ：0没有被按下，1被按下

typedef struct{
	TYPE_KEY 	key;
	TYPE_PS2  	PS2;
}keyVal_Str;

//宏定义PS2值和KEY值
#define KEY_R_ALT		0
#define KEY_L_ALT		1
#define KEY_R_GUI		2
#define KEY_L_GUI		3

#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29

#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39

#define KEY_ENTER				40
#define KEY_ESC					41
#define KEY_BACKSPACE		42
#define KEY_TAB					43
#define KEY_SPACE				44
#define KEY_MINUS				45
#define KEY_EQUAL				46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
#define KEY_FN				50
#define KEY_SEMICOLON	51
#define KEY_QUOTE			52
#define KEY_TILDE			53
#define KEY_COMMA			54
#define KEY_PERIOD		55
#define KEY_SLASH			56
#define KEY_CAPSLOCK	57

#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69

#define KEY_PRINT_SCREEN		70
#define KEY_SCROLL					71
#define KEY_PAUSE						72

#define KEY_INSERT			73
#define KEY_HOME				74
#define KEY_PAGE_UP			75
#define KEY_DELETE			76
#define KEY_END					77
#define KEY_PAGE_DOWN		78

#define KEY_RIGHT				79
#define KEY_LEFT				80
#define KEY_DOWN				81
#define KEY_UP					82

#define KEY_NUM_LOCK		83
#define KEYPAD_SLASH		84
#define KEYPAD_ASTERIX	85
#define KEYPAD_MINUS		86
#define KEYPAD_PLUS			87
#define KEYPAD_ENTER		88
#define KEYPAD_1				89
#define KEYPAD_2				90
#define KEYPAD_3				91
#define KEYPAD_4				92
#define KEYPAD_5				93
#define KEYPAD_6				94
#define KEYPAD_7				95
#define KEYPAD_8				96
#define KEYPAD_9				97
#define KEYPAD_0				98
#define KEYPAD_PERIOD		99

#define KEY_R_SHFT		100
#define KEY_L_SHFT		101
#define KEY_R_CTRL		102
#define KEY_L_CTRL		103



#define PS2_A				0x1C
#define PS2_B				0x32
#define PS2_C				0x21
#define PS2_D				0x23
#define PS2_E				0x24
#define PS2_F				0x2B
#define PS2_G				0x34
#define PS2_H				0x33
#define PS2_I				0x43
#define PS2_J				0x3B
#define PS2_K				0x42
#define PS2_L				0x4B
#define PS2_M				0x3A
#define PS2_N				0x31
#define PS2_O				0x44
#define PS2_P				0x4D
#define PS2_Q				0x15
#define PS2_R				0x2D
#define PS2_S				0x1B
#define PS2_T				0x2C
#define PS2_U				0x3C
#define PS2_V				0x2A
#define PS2_W				0x1D
#define PS2_X				0x22
#define PS2_Y				0x35
#define PS2_Z				0x1A
#define PS2_0				0x45
#define PS2_1				0x16
#define PS2_2				0x1E
#define PS2_3				0x26
#define PS2_4				0x25
#define PS2_5				0x2E
#define PS2_6				0x36
#define PS2_7				0x3D
#define PS2_8				0x3E
#define PS2_9				0x46

#define PS2_BACKSPACE   0x66	//退格
#define PS2_TILDE	    	0x0E	//"`"
#define PS2_MINUS				0x4E	//-
#define PS2_EQUAL				0x55	//=
#define PS2_BACKSLASH		0x5D	//“\”
#define PS2_SPACE				0x29
#define PS2_TAB					0x0D
#define PS2_CAPSLOCK		0x58
#define PS2_L_SHFT			0x12
#define PS2_L_CTRL			0x14
#define PS2_R_CTRL 			0XE014
#define PS2_R_GUI				0XE027
#define PS2_L_GUI				0XE01f 
#define PS2_L_ALT				0x11
#define PS2_R_ALT				0XE011
#define PS2_R_SHFT			0x59

#define PS2_ENTER		0x5A
#define PS2_ESC			0x76
#define PS2_F1			0x05
#define PS2_F2			0x06
#define PS2_F3			0x04
#define PS2_F4			0x0C
#define PS2_F5			0x03
#define PS2_F6			0x0B
#define PS2_F7			0x83
#define PS2_F8			0x0A
#define PS2_F9			0x01
#define PS2_F10			0x09
#define PS2_F11			0x78
#define PS2_F12			0x07

#define	PS2_FN						0XE02F
#define PS2_PRINT_SCREEN	0x127c
#define PS2_SCROLL				0x7E
#define PS2_PAUSE					0x1477	//e1 14 77 e1 f0 14 f0 77

#define PS2_NUM_LOCK			0x77
#define PS2_PAD_SLASH			0xE04A	// /
#define PS2_PAD_ASTERIX		0x7C	//*
#define PS2_PAD_MINUS			0x7B	//-	
#define PS2_PAD_PLUS			0x79	//+
#define PS2_PAD_ENTER			0xE05A
#define PS2_PAD_PERIOD		0x71	//.
#define PS2_PAD_0					0x70
#define PS2_PAD_1					0x69
#define PS2_PAD_2					0x72
#define PS2_PAD_3					0x7A
#define PS2_PAD_4					0x6B
#define PS2_PAD_5					0x73
#define PS2_PAD_6					0x74
#define PS2_PAD_7					0x6C
#define PS2_PAD_8					0x75
#define PS2_PAD_9					0x7D

#define PS2_SLASH					0x4A		// /
#define PS2_LEFT_BRACE		0x54		//[
#define PS2_RIGHT_BRACE		0x5B		//]
#define PS2_SEMICOLON			0x4C		//; 
#define PS2_COMMA					0x41		// ,
#define PS2_QUOTE		    	0x52		// '

#define PS2_PERIOD				0x49		//。


#define PS2_INSERT				0x1270
#define PS2_HOME					0x126C
#define PS2_PAGE_UP				0x127D
#define PS2_DELETE				0x1271
#define PS2_END						0x1269
#define PS2_PAGE_DOWN			0x127a

#define	PS2_UP						0X1275
#define	PS2_DOWN					0X1272
#define	PS2_LEFT					0X126B
#define	PS2_RIGHT					0X1274

extern const keyVal_Str keyMap[104];

#endif
