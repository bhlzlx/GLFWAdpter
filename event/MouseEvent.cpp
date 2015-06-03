//
// Created by phantom on 2015/5/21.
//

#include "MouseEvent.h"
#include <cstdio>

const char eventName[][32] =
{
  "move","down","up","nil"
};

const char buttonName[][32]=
{
  "left","mid","right","nil"
};

MouseEvent::MouseEvent()
{
    this->currX = this->currY = this->lastX = this->lastY = 0;
    this->button = eMouseButtonInvalid;
    this->eventType = eMouseEventInvalid;
}

void MouseEvent::Trace()
{
    printf("Button : %s\n Event : %s \nCoord: %04d x %04d \n %04d x %04d \n",buttonName[button],eventName[eventType],lastX,lastY,currX,currY);
}
