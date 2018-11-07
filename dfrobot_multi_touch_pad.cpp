#include "dfrobot_multi_touch_pad.h"

struct touch_fun * touch_func_list[30];

DFRobot_MultiTouchPad::DFRobot_MultiTouchPad()
{  
  readAnalogPin = 0;
  openedKeyTime = 0;
  isOpenedKeyOpened = false;
  touchFuncLen = 0;
  openedKey = 0x10; //默认为按键0
  pinMode(13,OUTPUT);
}

int DFRobot_MultiTouchPad::setOpenedKeyTime(int time)
{
  if(time > 0){
    openedKeyTime = time;
    isOpenedKeyOpened = false;//等0按键按了3秒后，再打开开关，为true
    return 1;
  }else{
    Serial.println("set time must greater than 0.");
    return 0;
  }
}

int DFRobot_MultiTouchPad::setOpenedKey(int key)
{
  if((key < 0) || (key > 4)){
    Serial.println("error key number.");
    return 0;  
  }
  openedKey = getCorrespondingNum(key);
  return 1;
}

int DFRobot_MultiTouchPad::getCorrespondingNum(int buttonNum)
{
  if(buttonNum == 0){
    return 0x10;  
  }else if(buttonNum == 1){
    return 0x08;
  }else if(buttonNum == 2){  
    return 0x04;
  }else if(buttonNum == 3){
    return 0x02;
  }else{
    return 0x01;
  }
}

int DFRobot_MultiTouchPad::setButtonCombinationCallback(int buttonCombination, buttonCombinationCallback_t callback_handler)
{
  if((buttonCombination > 0x1F)||(buttonCombination < 1)||(callback_handler == NULL)){
    Serial.println("error button number.");
    return 0;  
  }
  struct touch_fun *myfun = (struct touch_fun *)malloc(sizeof(struct touch_fun));
  myfun->touchNum = buttonCombination;
  myfun->fun = callback_handler;
  touch_func_list[touchFuncLen] = myfun;
  touchFuncLen += 1;
  return 1;
}

int DFRobot_MultiTouchPad::init(int analogPin)
{
  readAnalogPin = analogPin;
  baseValue = 0;
  int sensorValue = 0;
  int resetValue = 0;
  for(int i = 0 ; i < 1000 ; i++){
    sensorValue = analogRead(readAnalogPin);
    //Serial.println(sensorValue);
    if((sensorValue == 1023) || (sensorValue == 0)){
      return 0;  
    }else if((sensorValue < 590) || (sensorValue > 630)){
      i -= 1;
      resetValue += 1;
      if(resetValue >= 10){
        return 0;  
      }
      delay(1);
      continue;
    }else if(i < 500){
      delay(1);
      continue;  
    }
    baseValue += sensorValue;
    delay(1);
  }
  Serial.println("get base value OK.");
  baseValue = baseValue/(3 * 500);
  Serial.print("baseValue=");
  Serial.println(baseValue,2);

  return 1;
}

uint8_t DFRobot_MultiTouchPad::read(void)
{
  if(openedKeyTime > 0){
    return hasSetOpenedKeyTimeRead();  
  }else{
    return noSetOpenedKeyTimeRead();
  }
}

int DFRobot_MultiTouchPad::hasSetOpenedKeyTimeRead()
{
  if(isOpenedKeyOpened == true){
    int sensorValue = readOnce();
    if(isOpenedKeyTouched(sensorValue)){
      int notouchNum = 0;
      int allnotouchNum = 0;
      int alltouchNum=openedKeyTime*1000/10;
      for(int i = 0; i < alltouchNum ; i++){
        sensorValue = readOnce();
        if(whichTouched(sensorValue) == openedKey){
          notouchNum = 0;
          continue;
        }else if(whichTouched(sensorValue) == 0xFF){
          notouchNum += 1;
          allnotouchNum += 1;
          if(notouchNum >= 10){
            allnotouchNum = 0;
            runTouchedCallback(openedKey);
            return openedKey;  
          }
          continue;
        }else{
          int touchedNum = whichTouched(sensorValue);
          runTouchedCallback(touchedNum);
          return touchedNum;
        }
      }
      if(allnotouchNum/alltouchNum < 0.2){
        Serial.println("openKey close");
        isOpenedKeyOpened = false;
        digitalWrite(13, LOW); 
        return openedKey;
      }else{
        return -1;  
      }
    }else{
      int touchedNum = whichTouched(sensorValue);
      runTouchedCallback(touchedNum);
      return touchedNum;
    }
  }else{
    int sensorValue = readOnce();
    if (isOpenedKeyTouched(sensorValue)){
      int notouchNum = 0;
      int allnotouchNum = 0;
      int alltouchNum=openedKeyTime*1000/10;
      for(int i = 0; i < alltouchNum ; i++){
        sensorValue = readOnce();
        if(whichTouched(sensorValue) == openedKey){
          notouchNum = 0;
          continue;
        }else if(whichTouched(sensorValue) == 0xFF){
          notouchNum += 1;
          allnotouchNum += 1;
          if(notouchNum >= 10){
            allnotouchNum = 0;
            Serial.println("Please touch more time.");
            return 0xFF;  
          }
          continue;
        }else{
          return -1; 
        }
      }
      if(allnotouchNum/alltouchNum < 0.2){
        Serial.println("openKey open");
        isOpenedKeyOpened = true;
        digitalWrite(13, HIGH); 
        return openedKey;
      }else{
        return -1;  
      }
    }else{
      return -1;  
    }
  }
}

int DFRobot_MultiTouchPad::noSetOpenedKeyTimeRead()
{
  int sensorValue = readOnce();
  int touchedNum = whichTouched(sensorValue);
  runTouchedCallback(touchedNum);
  return touchedNum;
}

int DFRobot_MultiTouchPad::readOnce()
{
  int sensorValue;
  int sensorLast = 0;
  for(int i = 0; i < 10 ; i++){
    sensorValue = analogRead(readAnalogPin);
    sensorLast += sensorValue;
    delay(1);
  }
  sensorLast = sensorLast/10;

  //Serial.println(sensorLast);
  //Serial.println((KEY0*baseValue));
  return sensorLast;
}

bool DFRobot_MultiTouchPad::isOpenedKeyTouched(int sensorValue)
{
  if (openedKey == whichTouched(sensorValue)){
    return true;  
  }else{
    return false;  
  }
}

void DFRobot_MultiTouchPad::runTouchedCallback(int touchedNum)
{
  for(int i=0;i<touchFuncLen;i++){
    if(touch_func_list[i]->touchNum == touchedNum){
      touch_func_list[i]->fun();  
    }
  }
}

int DFRobot_MultiTouchPad::whichTouched(int sensorValue)
{
  if ((sensorValue >= ((int)(KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY3 * baseValue) + 1))){
  //}else if ((sensorValue >= ((int)(KEY0_KEY1_KEY2 * baseValue)-1))&&(sensorValue <= ((int)(KEY0_KEY1_KEY2 * baseValue)+1))){
    //Serial.println("KEY3");
    return 0x10;   //00010000  button0
  }else if ((sensorValue >= ((int)(KEY0 * baseValue) - 3)) && (sensorValue <= ((int)(KEY0 * baseValue) + 2))){
    //Serial.println("KEY0");
    return 0x08;   //00001000  button1
  }else if ((sensorValue >= ((int)(KEY1 * baseValue) - 2)) && (sensorValue <= ((int)(KEY1 * baseValue) + 1))){
    //Serial.println("KEY1");
    return 0x04;   //00000100  button2
  }else if ((sensorValue >= ((int)(KEY2 * baseValue) - 3)) && (sensorValue <= ((int)(KEY2 * baseValue) + 2))){
    //Serial.println("KEY2");
    return 0x02;   //00000010  button3
  }else if ((sensorValue >= ((int)(KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY4 * baseValue) + 1))){
    //Serial.println("KEY4");
    return 0x01;   //00000001  button4
  }else if ((sensorValue >= ((int)(KEY0_KEY1 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY1 * baseValue) + 1))){
    //Serial.println("KEY0_KEY1");
    return 0x0C;   //00001100   button1 button2
  }else if ((sensorValue >= ((int)(KEY0_KEY2 * baseValue)- 2 )) && (sensorValue <= ((int)(KEY0_KEY2 * baseValue) + 1))){
    //Serial.println("KEY0_KEY2");
    return 0x0A;   //00001010   button1 button3
  }else if ((sensorValue >= ((int)(KEY0_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY3 * baseValue) + 1))){
    //Serial.println("KEY0_KEY3");
    return 0x18;   //00011000   button1 button0
  }else if ((sensorValue >= ((int)(KEY0_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY0_KEY4 * baseValue) + 1))){
    //Serial.println("KEY0_KEY4");
    return 0x09;   //00001001   button1 button4
  }else if ((sensorValue >= ((int)(KEY1_KEY2 * baseValue) - 2)) && (sensorValue <= ((int)(KEY1_KEY2 * baseValue) + 1))){
    //Serial.println("KEY1_KEY2");
    return 0x06;   //00000110   button2 button3
  }else if ((sensorValue >= ((int)(KEY1_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY1_KEY3 * baseValue) + 1))){
    //Serial.println("KEY1_KEY3");
    return 0x14;   //00010100   button2 button0
  }else if ((sensorValue >= ((int)(KEY1_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY1_KEY4 * baseValue) + 1))){
    //Serial.println("KEY1_KEY4");
    return 0x05;   //00000101   button2 button4
  }else if ((sensorValue >= ((int)(KEY2_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY2_KEY3 * baseValue) + 1))){
    //Serial.println("KEY2_KEY3");
    return 0x12;   //00010010   button3 button0
  }else if ((sensorValue >= ((int)(KEY2_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY2_KEY4 * baseValue)))){
    //Serial.println("KEY2_KEY4");
    return 0x03;   //00000011   button3 button4
  }else if ((sensorValue >= ((int)(KEY3_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY3_KEY4 * baseValue)))){
  //}else if ((sensorValue >= ((int)(KEY0_KEY1_KEY2_KEY4 * baseValue)-2))&&(sensorValue <= ((int)(KEY0_KEY1_KEY2_KEY4 * baseValue)))){
    //Serial.println("KEY3_KEY4");
    return 0x11;   //00010001   button0 button4
  }else if ((sensorValue >= ((int)(KEY0_KEY1_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY1_KEY3 * baseValue)))){
    //Serial.println("KEY0_KEY1_KEY3");
    return 0x1C;   //00011100   button1 button2 button0
  }else if ((sensorValue >= ((int)(KEY0_KEY1_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY0_KEY1_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY1_KEY4");
    return 0x0D;   //00001101   button1 button2 button0
  }else if ((sensorValue >= ((int)(KEY0_KEY2_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY2_KEY3 * baseValue)))){
    //Serial.println("KEY0_KEY2_KEY3");
    return 0x1A;   //00011010   button1 button3 button0
  }else if ((sensorValue >= ((int)(KEY0_KEY2_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY0_KEY2_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY2_KEY4");
    return 0x0B;   //00001011   button1 button3 button4
  }else if ((sensorValue >= ((int)(KEY0_KEY3_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY3_KEY4");
    return 0x19;   //00011001   button1 button0 button4
  }else if ((sensorValue >= ((int)(KEY1_KEY2_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY1_KEY2_KEY3 * baseValue)))){
    //Serial.println("KEY1_KEY2_KEY3");
    return 0x16;   //00010110   button2 button3 button0
  }else if ((sensorValue >= ((int)(KEY1_KEY2_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY1_KEY2_KEY4 * baseValue)))){
    //Serial.println("KEY1_KEY2_KEY4");
    return 0x07;   //00000111   button2 button3 button4
  }else if ((sensorValue >= ((int)(KEY1_KEY3_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY1_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY1_KEY3_KEY4");
    return 0x15;   //00010101   button2 button0 button4
  }else if ((sensorValue >= ((int)(KEY2_KEY3_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY2_KEY3_KEY4 * baseValue) - 1))){
    //Serial.println("KEY2_KEY3_KEY4");
    return 0x13;   //00010011   button3 button0 button4
  }else if ((sensorValue >= ((int)(KEY0_KEY1_KEY2_KEY3 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY1_KEY2_KEY3 * baseValue)))){
    //Serial.println("KEY0_KEY1_KEY2_KEY3");
    return 0x1E;   //00011110   button1 button2 button3 button0
  }else if ((sensorValue >= ((int)(KEY0_KEY1_KEY3_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY0_KEY1_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY1_KEY3_KEY4");
    return 0x1D;   //00011101   button1 button2 button0 button4
  }else if ((sensorValue >= ((int)(KEY0_KEY2_KEY3_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY0_KEY2_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY2_KEY3_KEY4");
    return 0x1B;   //00011011   button1 button3 button0 button4
  }else if ((sensorValue >= ((int)(KEY1_KEY2_KEY3_KEY4 * baseValue) - 1)) && (sensorValue <= ((int)(KEY1_KEY2_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY1_KEY2_KEY3_KEY4");
    return 0x17;   //00010111   button2 button3 button0 button4
  }else if ((sensorValue >= ((int)(KEY0_KEY1_KEY2_KEY3_KEY4 * baseValue) - 2)) && (sensorValue <= ((int)(KEY0_KEY1_KEY2_KEY3_KEY4 * baseValue)))){
    //Serial.println("KEY0_KEY1_KEY2_KEY3_KEY4");
    return 0x1F;   //00011111   button1 button2 button3 button0 button4
  }else{
    return 0xFF;  
  }
}

