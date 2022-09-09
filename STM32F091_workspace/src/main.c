/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"

char offset;
char history[16];
char queue[2];
int  qin;
int  qout;			

void enable_ports(void) {
	// Turn on GPIOA and GPIOB clocks
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

  // Configure PB0-PB3 to be outputs
  GPIOB->MODER &= ~0xff;
  GPIOB->MODER |= 0x55;

  // configure PA0-PA3 to be outputs
  GPIOA->MODER &= ~0xff;
  GPIOA->MODER |= 0x55;

  // configure PA4-PA7 to be inputs
  GPIOA->MODER &= ~0xff00;

  // configure internal pull down resistors for PA4-PA7
  GPIOA->PUPDR &= ~0xff00;
  GPIOA->PUPDR |= 0xaa00;

}

//===========================================================================
// insert_queue()    (Autotest #7)
// Insert the key index number into the two-entry queue.
// Parameters: n: the key index number
//===========================================================================
void insert_queue(int n)
{
    n |= 0x80;
    queue[qin] = n;
    qin ^= 1;
}

//===========================================================================
// update_hist()    (Autotest #8)
// Check the columns for a row of the keypad and update history values.
// If a history entry is updated to 0x01, insert it into the queue.
// Parameters: none
//===========================================================================
void update_hist(int cols)
{
    int row = offset & 3;
    for(int i=0; i < 4; i++) {
        history[4*row+i] = (history[4*row+i]<<1) + ((cols>>i)&1);
        if(history[4*row+i] == 0x1)
            insert_queue(4*row+i);
    }
}

//===========================================================================
// set_row()    ( #5)
// Set the row active on the keypad matrix.
// Parameters: none
//===========================================================================
void set_row()
{
    char lower2_offset = offset & 3;
    GPIOA->ODR |= 1<<lower2_offset;
    GPIOA->ODR &= 0xfff0 + (1<<lower2_offset);
}

//===========================================================================
// get_cols()    ( #6)
// Read the column pins of the keypad matrix.
// Parameters: none
// Return value: The 4-bit value read from PC[7:4].
//===========================================================================
int get_cols()
{
    int cols = GPIOA->IDR;
    cols = cols>>4;
    cols &= 15;
    return cols;
}

//===========================================================================
// Timer 7 ISR()    ( #9)
// This is the Timer 7 ISR
// Parameters: none
// (Write the entire subroutine below.)
//===========================================================================
void TIM7_IRQHandler()
{
    // acknowledge interrupt
    TIM7->SR &= ~1;

    // call functions
    update_hist(get_cols());

    // update offset
    offset = (offset + 1) & 0x7;
    set_row();
}

//===========================================================================
// setup_tim7()    ( #10)
// Configure timer 7
// Parameters: none
//===========================================================================
void setup_tim7()
{
    // enable RCC clock for TIM7
    RCC->APB1ENR |= 1<<5;

    // set PSC and ARR to interrupt once per millisecond
    TIM7->PSC = 120-1;
    TIM7->ARR = 400-1;

    // enable UIE bit in the DIER
    TIM7->DIER |= TIM_DIER_UIE;

    // enable tim7 in NVIC ISER
    NVIC->ISER[0] = 1<<TIM7_IRQn;

    // set the CEN bit in TIM7_CR1
    TIM7->CR1 |= TIM_CR1_CEN;
}

// function to scan the keypad
// char scanKeypad(void) {
//   int row, col;

//   // Change the state of one row at a time
//   for (row = 0; row < 4; row++) {
//     // Set all rows to 0
//     GPIOA->ODR &= ~(0xF << 4);

//     // Set the current row to 1
//     GPIOA->ODR |= (1 << (row + 4));

//     // Check if any column is 1
//     if ((GPIOA->IDR & 0xF) != 0xF) {
//       // Check which column is 1
//       for (col = 0; col < 4; col++) {
//         if ((GPIOA->IDR & (1 << col)) == 0) {
//           return key_array[row][col];
//         }
//       }
//     }
//   }

//   return '0';
// }

//===========================================================================
// show_digit()    ( #4)
// Output a single digit on the seven-segmnt LED array.
// Parameters: none
//===========================================================================
void show_digit(char key)
{
  // set PB0-3 to low
  GPIOB->ODR &= ~(0xF);

  // Light up PB0 if key is 1, 3, 5, 7, 9, B, D, or #
  if (key == '1' || key == '3' || key == '5' || key == '7' || key == '9' || key == 'B' || key == 'D' || key == '#') {
    GPIOB->ODR |= (1 << 0);
  } else {
    GPIOB->ODR &= ~(1 << 0);
  }

  // Light up PB1 if key is 2, 3, 6, 7, A, B, *, or #
  if (key == '2' || key == '3' || key == '6' || key == '7' || key == 'A' || key == 'B' || key == '*' || key == '#') {
    GPIOB->ODR |= (1 << 1);
  } else {
    GPIOB->ODR &= ~(1 << 1);
  }

  // Light up PB2 if key is 4, 5, 6, 7, C, D, *, or #
  if (key == '4' || key == '5' || key == '6' || key == '7' || key == 'C' || key == 'D' || key == '*' || key == '#') {
    GPIOB->ODR |= (1 << 2);
  } else {
    GPIOB->ODR &= ~(1 << 2);
  }
  
  //Light up PB3 if key is 8, 9, A, B, C, D, *, or #
  if (key == '8' || key == '9' || key == 'A' || key == 'B' || key == 'C' || key == 'D' || key == '*' || key == '#') {
    GPIOB->ODR |= (1 << 3);
  } else {
    GPIOB->ODR &= ~(1 << 3);
  }
}

//===========================================================================
// getkey()    (Autotest #11)
// Wait for an entry in the queue.  Translate it to ASCII.  Return it.
// Parameters: none
// Return value: The ASCII value of the button pressed.
//===========================================================================
int getkey()
{
    // wait for initial interrupt
    asm volatile("wfi");
    // keep waiting while queue[qout] is 0
    while(queue[qout] == 0)
        asm volatile("wfi");

    // save copy, set to 0, toggle qout
    char queue_copy = queue[qout];
    queue[qout] = 0;
    qout ^= 1;

    // AND copy with 0x7f
    queue_copy &= 0x7f;

    if(queue_copy == 0)
        return '1';
    if(queue_copy == 1)
        return '4';
    if(queue_copy == 2)
        return '7';
    if(queue_copy == 3)
        return '*';
    if(queue_copy == 4)
        return '2';
    if(queue_copy == 5)
        return '5';
    if(queue_copy == 6)
        return '8';
    if(queue_copy == 7)
        return '0';
    if(queue_copy == 8)
        return '3';
    if(queue_copy == 9)
        return '6';
    if(queue_copy == 10)
        return '9';
    if(queue_copy == 11)
        return '#';
    if(queue_copy == 12)
        return 'A';
    if(queue_copy == 13)
        return 'B';
    if(queue_copy == 14)
        return 'C';
    if(queue_copy == 15)
        return 'D';
}


int main(void)
{
  // Reset peripherals
	HAL_Init();

  // Setup
  enable_ports();
  setup_tim7();

  while(1) {
    // Scan the keypad
    char key = getkey();

    // Show the digit in binary on the LED array
    show_digit(key);
  }
  
	for(;;);
}
