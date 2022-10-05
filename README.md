# Split USB Keyboard
ECE477 Team 4 github

## Reset source code to work as keyboard
1. Make changes to .ioc file and save it
2. Build the code
3. Delete the Middlewares folder
4. Unzip Middlewares KEYBOARD SETUP.zip and rename the folder to Middlewares (Keep the ziped file here for future use)
5. Build the code (there will likely be errors, if so proceed to step 6)
6. Navigate to Drivers>STM32F4xx_HAL_DRIVER>Src in this directory there will be duplicate files that look like this `...cortex.c` and `...cortext 2.c` or `...cortex 3.c` to fix this delete all of the files that have a number at the end of the file name. This can easily be done in the console/terminal with `rm *2.c` or `rm *3.c`
