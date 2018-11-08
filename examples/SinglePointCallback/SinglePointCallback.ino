/*!
 *  file SinglePointCallback.ino
 * 
 *  There are five buttons on the multi-touch pad: 0, 1, 2, 3, 4.
 *
 *  This example presents: Single point callback function, the user realizes his own function by pressing the button
 *  
 *  Create DFRobot_MultiTouchPad object, take the analog pin as a parameter and tramsmit into the function when starting initialization.
 *  Set the callback function of the corresponding button, function implemented in callback function.
 * 
 *  Copyright   [DFRobot](http://www.dfrobot.com), 2018
 *  Copyright   GNU Lesser General Public License
 * 
 *  version V1.1
 *  date 2018-11-8
 */

#include "DFRobot_MultiTouchPad.h"
DFRobot_MultiTouchPad multiTouchPad;

void button_0_touch()
{
  Serial.println("button 0 touched");  
}

void button_1_touch()
{
  Serial.println("button 1 touched");  
}

void button_2_touch()
{
  Serial.println("button 2 touched");  
}

void button_3_touch()
{
  Serial.println("button 3 touched");  
}

void button_4_touch()
{
  Serial.println("button 4 touched");  
}

// the setup routine runs once when you press reset:
void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Multi touch pad init.");
  while( !multiTouchPad.init(A0)){ //multiTouchPad.init 0:fail 1:sucess
    Serial.println("Please connect multi touch pad or choose real analog Pin or reset the board.");
    delay(1000);  
  }
  Serial.println("Multi touch pad init OK.");
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_0, button_0_touch);
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_1, button_1_touch);
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_2, button_2_touch);
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_3, button_3_touch);
  multiTouchPad.setButtonCombinationCallback(DFMT_BUTTON_4, button_4_touch);
}

// the loop routine runs over and over again forever:
void loop() 
{
  multiTouchPad.read();
  delay(50);        
}
