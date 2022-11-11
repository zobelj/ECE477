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

const uint16_t layout1[nRows1_const][nCols1_const] = {{KEY_ESC, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5},
													  {KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T},
													  {0, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G},
													  {KEY_LSHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B},
													  {0, KEY_LGUI, 0, 0, 0, 0},
													  {0, 0, 0, 0, 0, 0},
													  {0, 0, 0, 0, 0, 0}
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

const uint16_t layout2[nRows2_const][nCols2_const] = {{KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_PLUS},
													  {KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, 0},
													  {KEY_H, KEY_J, KEY_K, KEY_L, KEY_COLON, KEY_QUOTE},
													  {KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_FSLASH, KEY_RSHIFT},
													  {0, 0, 0, 0, 0, 0 },
													  {0, 0, 0, 0, 0, 0 },
													  {0, 0, 0, 0, 0, 0 }};

char keypresses2[nRows2_const][nCols2_const] = {{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0},
												{0, 0, 0, 0, 0, 0}};



#endif /* INC_LAYOUT_H_ */
