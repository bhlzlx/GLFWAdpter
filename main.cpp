#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "event/MouseEvent.h"
#include "app/AppDelegate.h"

typedef struct
{
    GLFWwindow * window;
} WindowHandle;

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

int main ()
{
    // start GL context and O/S window using the GLFW helper library
    appDelegate.AppWillStart();

    InitOpenGL();

    appDelegate.AppDidStart();

    while (!glfwWindowShouldClose (mainWnd.window))
    {
        // wipe the drawing surface clear
        OnRender();
        // update other events like input handling
        glfwPollEvents ();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (mainWnd.window);
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
