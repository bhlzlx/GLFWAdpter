//
// Created by phantom on 2015/5/18.
//

#include "AppDelegate.h"
#include <gl/glew.h>
//#include "../models/models.h"

#include <stdio.h>

#include <glm/glm.hpp>

void AppDelegate::AppWillStart()
{
    m_pViewController = new OpenGLViewController();
}

void AppDelegate::AppMouseAction(MouseEvent * event)
{
    //event->Trace();
    m_pViewController->OnMouseEvent(event);
}

void AppDelegate::AppDidStart()
{
    printf("OpenGL Application Started~\n");
    this->m_pViewController->OnInit();
}

void AppDelegate::AppWillTerminate()
{
    m_pViewController->Release();
}

void AppDelegate::AppWindowDidResize(uint32 width,uint32 height)
{
    m_pViewController->OnResize(width,height);
}

void AppDelegate::AppKeyPressed(unsigned char key, int x, int y)
{
    m_pViewController->OnKeyPressed(key,x,y);
}

void AppDelegate::AppOnUpdate()
{
    m_pViewController->OnUpdate();
}

void AppDelegate::AppOnTimer()
{
    m_pViewController->OnTimer();
}
