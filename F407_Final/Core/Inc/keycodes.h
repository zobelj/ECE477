/*
 * keycodes.h
 *
 *  Created on: Sep 28, 2022
 *      Author: joe
 */

#ifndef INC_KEYCODES_H_
#define INC_KEYCODES_H_

#define KEY_A       	0x04
#define KEY_B       	0x05
#define KEY_C      		0x06
#define KEY_D       	0x07
#define KEY_E			0x08
#define KEY_F			0x09
#define KEY_G			0x0A
#define KEY_H			0x0B
#define KEY_I			0x0C
#define KEY_J			0x0D
#define KEY_K			0x0E
#define KEY_L			0x0F
#define KEY_M			0x10
#define KEY_N			0x11
#define KEY_O			0x12
#define KEY_P			0x13
#define KEY_Q			0x14
#define KEY_R			0x15
#define KEY_S			0x16
#define KEY_T			0x17
#define KEY_U			0x18
#define KEY_V			0x19
#define KEY_W			0x1A
#define KEY_X			0x1B
#define KEY_Y			0x1C
#define KEY_Z			0x1D

#define KEY_1    		0x1E
#define KEY_2     		0x1F
#define KEY_3     		0x20
#define KEY_4     		0x21
#define KEY_5     		0x22
#define KEY_6     		0x23
#define KEY_7    		0x24
#define KEY_8     		0x25
#define KEY_9     		0x26
#define KEY_0     		0x27

#define KEY_ENTER		0x28
#define KEY_ESC         0x29
#define KEY_BACKSPACE 	0x2A
#define KEY_TAB		    0x2B
#define KEY_SPACE       0x2C
#define KEY_MINUS       0x2D
#define KEY_PLUS        0x2E
#define KEY_LBRACKET    0x2F
#define KEY_RBRACKET    0x30
#define KEY_BSLASH		0x31
#define KEY_COLON       0x33
#define KEY_QUOTE       0x34
#define KEY_TILDE		0x35
#define KEY_COMMA       0x36
#define KEY_PERIOD      0x37
#define KEY_FSLASH      0x38
#define KEY_CAPSLOCK	0x39

#define KEY_F1			0x3A
#define KEY_F2			0x3B
#define KEY_F3			0x3C
#define KEY_F4			0x3D

#define KEY_PRINTSCREEN 0x46
#define KEY_INSERT		0x49
#define KEY_HOME		0x4A
#define KEY_PAGEUP		0x4B
#define KEY_DELETE      0x4C
#define KEY_END			0x4D
#define KEY_PAGEDOWN	0x4E
#define KEY_RIGHT 		0x4F
#define KEY_LEFT  		0x50
#define KEY_DOWN  		0x51
#define KEY_UP    		0x52

#define KEY_CUT			0x7B
#define KEY_COPY		0x7C
#define KEY_PASTE		0x7D

#define KEY_MUTE    	0x7F
#define KEY_VOLUP   	0x80
#define KEY_VOLDOWN 	0x81

// modifiers. FF prefix to differentiate
#define KEY_LCTRL   	0xFF00
#define KEY_LSHIFT  	0xFF01
#define KEY_LALT    	0xFF02
#define KEY_LGUI    	0xFF03
#define KEY_RCTRL   	0xFF04
#define KEY_RSHIFT  	0xFF05
#define KEY_RALT    	0xFF06
#define KEY_RGUI    	0xFF07


#define MEDIA_PLAYPAUSE 0xE8
#define MEDIA_PREVSONG	0xE9
#define MEDIA_NEXTSONG	0xEA
#define MEDIA_VOLUP		0xED
#define MEDIA_VOLDOWN	0xEE
#define MEDIA_MUTE		0xEF


#endif /* INC_KEYCODES_H_ */
