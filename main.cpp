#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "event/MouseEvent.h"
#include "app/AppDelegate.h"

typedef struct
{
    GLFWwindow * window;
} WindowHandle;

typedef void (*FPS_UPDATE_CALLBACK)(unsigned int );

class FPS_Helper
{
private:
    double          m_uFixedFPSInterval;

    unsigned int    m_uFPS;
    unsigned int    m_uFPSCounter;

    double          m_iFPSBegin;
    double          m_iTimeUpdate;
    //
    FPS_UPDATE_CALLBACK m_onFpsUpdate;
public:
    FPS_Helper();
    void SetFixedFPS(unsigned int fps);
    bool Tick();
    unsigned int GetFPS();
    void SetFpsCallback(FPS_UPDATE_CALLBACK callback);
};

WindowHandle        mainWnd;
MouseEvent          mouseEvent;
AppDelegate         appDelegate;

unsigned int shader_programme;



int InitOpenGL();

static unsigned int counter = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        mouseEvent.eventType = eMouseDown;
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseEvent.button = eMouseButtonLeft;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseEvent.button = eMouseButtonRight;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseEvent.button = eMouseButtonMid;
            break;
        }
        appDelegate.AppMouseAction(&mouseEvent);
    }
    else
    {
        mouseEvent.eventType = eMouseUp;
        appDelegate.AppMouseAction(&mouseEvent);
        mouseEvent.button = eMouseButtonInvalid;
    }

}

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if(mouseEvent.button == eMouseButtonInvalid)
    {
        mouseEvent.lastX = x;
        mouseEvent.lastY = y;
        return;
    }
    else
    {
        mouseEvent.currX = x;
        mouseEvent.currY = y;
        mouseEvent.eventType = eMouseMove;
        appDelegate.AppMouseAction(&mouseEvent);

        mouseEvent.lastX = x;
        mouseEvent.lastY = y;
    }
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    appDelegate.AppWindowDidResize(width,height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    appDelegate.AppKeyPressed(key,mouseEvent.lastX,mouseEvent.lastY);
}

static void OnRender()
{
    appDelegate.AppOnUpdate();
}

static void OnTimer()
{
    appDelegate.AppOnTimer();
}

int main ()
{
    // start GL context and O/S window using the GLFW helper library
    appDelegate.AppWillStart();
    FPS_Helper fpsHelper;
    fpsHelper.SetFixedFPS(30);
    InitOpenGL();

    appDelegate.AppDidStart();

    while (!glfwWindowShouldClose (mainWnd.window))
    {
        // wipe the drawing surface clear
        static double timer_last = 0;
        double curr_time = glfwGetTime();
        if(curr_time - timer_last > 0.2)
        {
            timer_last = timer_last + 0.02;
            OnTimer();
        }

        if(fpsHelper.Tick())
        {
            OnRender();
            // update other events like input handling
            glfwPollEvents ();
            // put the stuff we've been drawing onto the display
            glfwSwapBuffers (mainWnd.window);
        }
    }
    /* OTHER STUFF GOES HERE NEXT */

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}


int InitOpenGL()
{
    if (!glfwInit ())
    {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    mainWnd.window = glfwCreateWindow (640, 480, "Graphics", NULL, NULL);

    if (!mainWnd.window)
    {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwSetWindowUserPointer(mainWnd.window,&mainWnd);
    glfwSetMouseButtonCallback(mainWnd.window, mouse_button_callback);
    glfwSetCursorPosCallback(mainWnd.window, cursor_position_callback);
    glfwSetWindowSizeCallback(mainWnd.window, window_size_callback);
    glfwSetKeyCallback(mainWnd.window,key_callback);

    glfwMakeContextCurrent (mainWnd.window);

    glewExperimental = GL_TRUE;
    glewInit ();

    if (glewIsSupported("GL_VERSION_4_1"))
    {
        printf("Ready for OpenGL 4.1\n");
    }
    else
    {
        printf("OpenGL 4.1 not supported\n");
        exit(1);
    }

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LEQUAL); // depth-testing interprets a smaller value as "closer"

    return 0;
}

FPS_Helper::FPS_Helper()
{
    this->m_uFixedFPSInterval = 0.032;
    this->m_iFPSBegin = 0;
    this->m_iTimeUpdate = 0;
    this->m_uFPS = 0;
    this->m_uFPSCounter = 0;
    m_onFpsUpdate = NULL;
}

void FPS_Helper::SetFixedFPS(unsigned int fps)
{
    this->m_uFixedFPSInterval = 1.0f/(float)fps;
}

bool FPS_Helper::Tick()
{
    static double msec_curr = 0;
    msec_curr = glfwGetTime();
    double msec_diff = msec_curr - this->m_iTimeUpdate;
    double msec_fps_diff = msec_curr - this->m_iFPSBegin;

    if(msec_fps_diff >= 1.0)
    {
        //printf("fps : %d \n",fps);
        this->m_uFPS = this->m_uFPSCounter;
        this->m_uFPSCounter = 0;
        this->m_iFPSBegin = msec_curr;
        // fps更新回调
        if(m_onFpsUpdate)
            m_onFpsUpdate(m_uFPS);
    }

    if(msec_diff > this->m_uFixedFPSInterval)
    {
        // 如果期间有2倍以上间隔应该立即刷新，刷新时间就是当前时间
        // 如果没有2倍以上，则计算出对应的时间
        if(msec_diff < this->m_uFixedFPSInterval * 2)
        {
            m_iTimeUpdate = m_iTimeUpdate + m_uFixedFPSInterval;
        }
        else
        {
            m_iTimeUpdate = msec_curr;
        }
        m_uFPSCounter++;
        return true;
    }
    else
    {
        return false;
    }
}

void FPS_Helper::SetFpsCallback(FPS_UPDATE_CALLBACK callback)
{
    this->m_onFpsUpdate = callback;
}
