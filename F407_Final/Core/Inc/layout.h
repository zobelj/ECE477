/*
 * layout.h
 *
 *  Created on: Oct 28, 2022
 *      Author: joe
 */

#ifndef INC_LAYOUT_H_
#define INC_LAYOUT_H_

#define nRows1_const 7
#define nCols1_const 6

#define nRows2_const 7
#define nCols2_const 6

/* Primary Half */
int nRows1 = 7;
int nCols1 = 6;

const uint16_t layout1[nRows1_const][nCols1_const] = {{KEY_ESC, KEY_TAB, KEY_CAPSLOCK, KEY_LSHIFT, KEY_LCTRL, KEY_SPACE},
													  {KEY_1, KEY_Q, KEY_A, KEY_Z, KEY_LGUI, KEY_BACKSPACE},
													  {KEY_2, KEY_W, KEY_S, KEY_X, KEY_LALT, KEY_ENTER},
													  {KEY_3, KEY_E, KEY_D, KEY_C, KEY_F1, 0},
													  {KEY_4, KEY_R, KEY_F, KEY_V, KEY_F2, 0},
													  {KEY_5, KEY_T, KEY_G, KEY_B, KEY_F3, 0},
													  {KEY_TILDE, KEY_F4, KEY_LBRACKET, KEY_HOME, KEY_END, 0}
};

char keypresses1[nRows1_const][nCols1_const] = {{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0}};
/* END Primary Half */


/* Secondary Half */
int nRows2 = 7;
int nCols2 = 6;

const uint16_t layout2[nRows2_const][nCols2_const] = {{KEY_DELETE, KEY_INSERT, KEY_RBRACKET, KEY_PAGEUP, KEY_PAGEDOWN, KEY_ENTER},
													  {KEY_6, KEY_Y, KEY_H, KEY_N, KEY_BSLASH, KEY_BACKSPACE},
													  {KEY_7, KEY_U, KEY_J, KEY_M, KEY_RCTRL, KEY_SPACE},
													  {KEY_8, KEY_I, KEY_K, KEY_COMMA, KEY_LEFT, 0},
													  {KEY_9, KEY_O, KEY_L, KEY_PERIOD, KEY_UP, 0},
													  {KEY_0, KEY_P, KEY_COLON, KEY_FSLASH, KEY_DOWN, 0},
													  {KEY_MINUS, KEY_PLUS, KEY_QUOTE, KEY_RSHIFT, KEY_RIGHT, 0}
};

char keypresses2[nRows2_const][nCols2_const] = {{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0}};
/* END Secondary Half */


/* Primary Rotary Encoder */
const uint16_t rotary_keys1[3] = {KEY_SPACE, KEY_LEFT, KEY_RIGHT};

char rotary_keypresses1[3] = {0, 0, 0};
/* END Primary Rotary Encoder */


/* Secondary Rotary Encoder */
const uint16_t rotary_keys2[3] = {KEY_MUTE, KEY_PAGEDOWN, KEY_PAGEUP};

char rotary_keypresses2[3] = {0, 0, 0};
/* END Secondary Rotary Encoder */

#endif /* INC_LAYOUT_H_ */
