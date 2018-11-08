# MULTI TOUCH PAD

Multi touch pad has five buttons on the multi-touch pad: 0, 1, 2, 3, 4.<br>
The screen has 29 button combinations. Users can set callback for the single button,<br>
double buttons and triple buttons combinations to realize different functions.<br>
The switch  mode, time and button can be set by users.<br>


## DFRobot_MultiTouchPad library for Arduino
-------------------------------------------------------------
Provide a library faciltates operations in the multi touch pad modules.

## Table of Contents

* [Summary](#summary)
* [Feature](#feature)
* [Installation](#installation)
* [Methods](#methods)

* [Compatibility](#compatibility)
* [Credits](#credits)
<snippet>
<content>

## Summary
Read the nubmer of the pressed butoon combination from multi-touch pad.

## Feature

1. Provide 29 button combinations<br>
2. Set callback for the single button, double buttons and triple buttons combinations<br>
3. Switch mode can be set<br>
4. Switch time can be set<br>
5. Switch button can be set<br>

## Installation

Download the library ZIP file and unzip it to the Arduino folder of the library.<br>

## Methods

```C++

#include "DFrobot_multi_touch_pad.h"

/*
 * @brief DFRobot_MultiTouchPad object
 */
DFRobot_MultiTouchPad();

/*
 * @brief Get the initial value of the multi-touch pad
 * 
 * @param analogPin  Analog port number
 * @return 0    init false
 *         1    init success
 */
int init(int analogPin); 

/*
 * @brief Get the initial value of the multi-touch pad
 * 
 * @param time  Set the switch time in seconds
 * @return -1    When the switch mode is on, the switch fails to open or close.
 *         0xFF    When the switch mode is on, the switch is pressed for less than the set switch recognition time.
 *         touched number    In any mode, return the number of the button combination
 */
uint8_t read(void);

/*
 * @brief Set the switch mode switch button
 * 
 * @param key  Set the switch mode switch button
 * @return 0   Set open button failed
 *         1   Set open button success
 */
int setOpenedKey(int key);

/*
 * @brief Set the switching time of the switch mode
 *
 * @param time Switch mode switching time in seconds
 * @return 0   failed
 *         1   success
 */
int setOpenedKeyTime(int time);

/*
 * @brief Set the callback function of button combination
 *
 * @param  buttonCombination    The number of the button combination
 *         callback_handler The function name of the callback function
 * @return 0    failed
 *         1    success
 */
int setButtonCombinationCallback(int buttonCombination, buttonCombinationCallback_t callback_handler);
```



## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno |       √      |             |            | 

## Credits

Written by DFRobot_JH, 2018. (Welcome to our [website](https://www.dfrobot.com/))
