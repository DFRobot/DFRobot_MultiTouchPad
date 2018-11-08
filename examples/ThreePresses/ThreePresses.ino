/*!
 *  file ThreePresses.ino
 * 
 *  There are five buttons on the multi-touch pad: 0, 1, 2, 3, 4.
 *
 *  This example presents: press three buttons at the same time to make the program run the function the user customized
 *  
 *  Create DFRobot_MultiTouchPad object, take the analog pin as a parameter and tramsmit into the function when starting initialization.
 *  When the initialization is finished, call setTrebleFun function,set the numbers of the three buttons (the numbers should be different)\
 *  and the related callback function. Please note that the parameters of the buttons must be within 0~4. 
 * 
 *  Copyright   [DFRobot](http://www.dfrobot.com), 2018
 *  Copyright   GNU Lesser General Public License
 * 
 *  version V1.1
 *  date 2018-11-8
 */

#include "DFRobot_MultiTouchPad.h"
DFRobot_MultiTouchPad multiTouchPad;

void threeble_touch1()
{
  Serial.println("threeble_touch1");  
}

void threeble_touch2()
{
  Serial.println("threeble_touch2");  
}

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Multi touch pad init.");
  while( !multiTouchPad.init(A0)){ //multiTouchPad.init 0:fail 1:sucess
    Serial.println("Please connect multi touch pad or choose real analog Pin or reset the pad.");
    delay(1000);  
  }
  Serial.println("Multi touch pad init OK.");

  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_0|DFMT_BUTTON_3|DFMT_BUTTON_4,threeble_touch1);
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_0|DFMT_BUTTON_1|DFMT_BUTTON_2,threeble_touch2);
}

void loop() 
{
  multiTouchPad.read();
  delay(50);       
}
