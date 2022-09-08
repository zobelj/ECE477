# Software Outline

This file is meant to serve as a general outline of the software running on the microcontrollers.

In its current state, only an overview of the keyboard input is included.


## Keyboard Input

---

### Preprocessing Overview
Some global variables will be set up before the keyboard is ready to use.

Tasks:
- set `rows` to be outputs, initialize all to low (eg. PA0-PA3)
- set `cols` to be inputs (eg. PA4-PA7)
- setup timer interrupt
- setup pins
- ...

Global variables:
- `queue` : array that holds the keypresses to be sent 
- `queue_index` : the index of the next available space in the queue
- ...

---

### Timer Interrupt Overview
- A timer `TIMx` will be set to interrupt the process at a  frequency of 10kHz (?)
- `TIMx_IRQHandler()` will do the following:
  - Acknowledge the interrupt
  - Check for columns that are activated (`get_cols()`)
  - Update the `queue` variable with any keys pressed between `send_data()` calls
  - Change the row currently set (`set_rows()`)
- `send_data()` sends the queue to the host PC at a frequency of 1000Hz (polling rate)
  - resets `queue`, prepares for next packet
- 


---

### Functions to Implement
- `get_key()`
  - waits for the interrupt to update the queue
  - converts the row-column value in queue to a key (switch case)
  - returns the key being pressed
  - Is this useful for our keyboard? I don't think we should have a `get_key()` function
  - This funcitonality should probably exist in the `send_data()` function or in `main()`
- `TIMx_IRQHandler()`
  - runs `get_cols()`
  - figures out what key is being pressed
  - runs `set_rows()` to change active row for the next interrupt
- `TIMx_setup()`
  - initializes the timer frequency
  - enables the timer
- `setup_pins()`
  - initialize pins for rows and columns
  - maybe initialize other necessary pins (LCDs, LEDs, etc.)
- `get_cols()`
  - returns the values of the column inputs as a 4-bit number
  - eg. `0010` indicates that columns 0, 2, and 3 are low and column 1 is high
- `set_rows()`
  - one active row at a time
  - changes the active row to the next row
  - eg. `0001` -> `0010` indicates the active row is changed from row 0 to row 1
- `update_queue`
  - add new keypresses to the end of the queue
  - these keypresses will be included in the next packet sent to the PC
- `send_data()`
  - send the queue to the host PC
  - clear the queue, reset the queue index
