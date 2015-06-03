//
// Created by phantom on 2015/5/21.
//

#ifndef GRAPHICSLIBRARY_MOUSEEVENT_H
#define GRAPHICSLIBRARY_MOUSEEVENT_H

enum MouseEventType
{
    eMouseMove,
    eMouseDown,
    eMouseUp,
    eMouseEventInvalid
};

enum MouseButton
{
    eMouseButtonLeft,
    eMouseButtonMid,
    eMouseButtonRight,
    eMouseButtonInvalid
};

struct MouseEvent
{
    int     lastX;
    int     lastY;
    int     currX;
    int     currY;
    MouseEventType eventType;
    MouseButton button;

    MouseEvent();

    void Trace();
};


#endif //GRAPHICSLIBRARY_MOUSEEVENT_H
