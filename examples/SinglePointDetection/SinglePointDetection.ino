/*!
 *  file SinglePointDetection.ino
 * 
 *  There are five buttons on the multi-touch pad: 0, 1, 2, 3, 4.
 *
 *  This example presents: Single point printing, which button is pressed to print the corresponding content
 *  
 *  Create DFRobot_MultiTouchPad object, take the analog pin as a parameter and tramsmit into the function when starting initialization.
 *  When the initialization is finished, call read function,view the print content.
 * 
 *  Copyright   [DFRobot](http://www.dfrobot.com), 2018
 *  Copyright   GNU Lesser General Public License
 * 
 *  version V1.0
 *  date 2018-11-2
 */

#include "dfrobot_multi_touch_pad.h"
DFRobot_MultiTouchPad multiTouchPad;

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
}

// the loop routine runs over and over again forever:
void loop() 
{
  int touchNum = multiTouchPad.read();
  switch (touchNum){
    case DFMT_BUTTON_0:
      Serial.println("button 0 touched"); 
      break;
    case DFMT_BUTTON_1:
      Serial.println("button 1 touched");  
      break;
    case DFMT_BUTTON_2:
      Serial.println("button 2 touched");  
      break;
    case DFMT_BUTTON_3:
      Serial.println("button 3 touched"); 
      break; 
    case DFMT_BUTTON_4:
      Serial.println("button 4 touched"); 
      break;
    default:
      Serial.println("other");  
      break;
  }
  delay(50);       
}