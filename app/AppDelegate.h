//
// Created by phantom on 2015/5/18.
//

#ifndef GRAPHICSLIBRARY_APPDELEGATE_H
#define GRAPHICSLIBRARY_APPDELEGATE_H

#include "OpenGLViewController.h"
#include "../event/MouseEvent.h"

struct IAppDelegate
{
    virtual void AppWillStart() = 0;
    virtual void AppDidStart() = 0;
    virtual void AppWillTerminate() = 0;
    virtual void AppWindowDidResize(uint32 width,uint32 height) = 0;
    virtual void AppOnUpdate() = 0;
    virtual void AppOnTimer() = 0;

    virtual void AppMouseAction(MouseEvent * event) = 0;
    virtual void AppKeyPressed(unsigned char key, int x, int y) = 0;
};

struct AppDelegate:public IAppDelegate
{
    OpenGLViewController *    m_pViewController;
    virtual void AppWillStart();
    virtual void AppDidStart();
    virtual void AppWillTerminate();
    virtual void AppWindowDidResize(uint32 width,uint32 height);
    virtual void AppOnUpdate();
    virtual void AppOnTimer();
    virtual void AppMouseAction(MouseEvent * event);
    virtual void AppKeyPressed(unsigned char key, int x, int y);
};


#endif //GRAPHICSLIBRARY_APPDELEGATE_H
