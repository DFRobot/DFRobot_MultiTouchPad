#ifndef DF_MULTI_TOUCH_PAD_H
#define DF_MULTI_TOUCH_PAD_H

#include "Arduino.h"
/*
 * kout0->key0->1
 * kout1->key1->2
 * kout2->key2->3 
 * kout3->key3->0
 * kout4->key4->4
 * button0 touch 00010000
 * button1 touch 00001000
 * button2 touch 00000100
 * button3 touch 00000010
 * button4 touch 00000001
 */
#define DFMT_BUTTON_0 0x10
#define DFMT_BUTTON_1 0x08
#define DFMT_BUTTON_2 0x04
#define DFMT_BUTTON_3 0x02
#define DFMT_BUTTON_4 0x01

 
#define KEY0 2.4194 
#define KEY1 1.94
#define KEY2 1.5
#define KEY3 1.0714
#define KEY4 0.4648

#define KEY0_KEY1 1.6802
#define KEY0_KEY2 1.3393
#define KEY0_KEY3 0.9868
#define KEY0_KEY4 0.4481
#define KEY1_KEY2 1.1786
#define KEY1_KEY3 0.8967
#define KEY1_KEY4 0.4286
#define KEY2_KEY3 0.7895
#define KEY2_KEY4 0.4024
#define KEY3_KEY4 0.3634

#define KEY0_KEY1_KEY2 1.0770
#define KEY0_KEY1_KEY3 0.8367
#define KEY0_KEY1_KEY4 0.4147
#define KEY0_KEY2_KEY3 0.7437
#define KEY0_KEY2_KEY4 0.3902
#define KEY0_KEY3_KEY4 0.3534
#define KEY1_KEY2_KEY3 0.6904
#define KEY1_KEY2_KEY4 0.3750
#define KEY1_KEY3_KEY4 0.3409
#define KEY2_KEY3_KEY4 0.3234

#define KEY0_KEY1_KEY2_KEY3 0.6542
#define KEY0_KEY1_KEY2_KEY4 0.3641
#define KEY0_KEY1_KEY3_KEY4 0.3319
#define KEY0_KEY2_KEY3_KEY4 0.3160
#define KEY1_KEY2_KEY3_KEY4 0.3061

#define KEY0_KEY1_KEY2_KEY3_KEY4 0.2988

#define VOLTAGE 3;  //voltage=3V

typedef void (*buttonCombinationCallback_t)(void);

/* !Save the button number and corresponding callback function when the button is pressed */
struct touch_fun
{
  uint8_t touchNum;
  buttonCombinationCallback_t fun;
};

class DFRobot_MultiTouchPad
{
public:
  DFRobot_MultiTouchPad();
  /* ! Get the base analog value in one second*/
  int init(int analogPin); 
  /* ! Read the number of the pressed button*/
  uint8_t read(void);
  /* ! Set the time of the identification switch*/
  int setOpenedKeyTime(int time);
  /* ! Set the number of the switch*/
  int setOpenedKey(int key);
  /* ! Set the callback function of button combination*/
  int setButtonCombinationCallback(int buttonCombination, buttonCombinationCallback_t callback_handler);

private:
  int readOnce();
  int noSetOpenedKeyTimeRead();
  int hasSetOpenedKeyTimeRead();
  int whichTouched(int sensorValue);
  bool isOpenedKeyTouched(int sensorValue);
  void runTouchedCallback(int touchedNum);
  int getCorrespondingNum(int buttonNum);

private:
  float baseValue;
  uint8_t readAnalogPin;

  bool isOpenedKeyOpened;
  uint8_t openedKey;
  uint8_t openedKeyTime;
  bool openedKeyThouchedFlag;

  uint8_t touchFuncLen;
};
#endif
