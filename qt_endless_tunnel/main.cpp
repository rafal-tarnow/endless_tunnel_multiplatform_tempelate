#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include "opengl_includes.hpp"
#include "../app/src/main/cpp/SystemAbstraction/system_abstraction.hpp"
#include <system_paths.hpp>

using namespace std;

void InitGLFWWindow();
void initFPS();
double calculateFPS();
void  printOpenGLVersion();

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

GLFWwindow *win;
int window_width = 0;
int window_height = 0;
int framebuffer_width=0;
int framebuffer_height=0;
float scale_x;
float scale_y;


void enableMultisample(int msaa)
{
    //    if (msaa)
    //    {
    //        glEnable(GL_MULTISAMPLE);
    //        glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    //        // detect current settings
    //        GLint iMultiSample = 0;
    //        GLint iNumSamples = 0;
    //        glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    //        glGetIntegerv(GL_SAMPLES, &iNumSamples);
    //        printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
    //    }
    //    else
    //    {
    //        glDisable(GL_MULTISAMPLE);
    //        printf("MSAA off\n");
    //    }
}








//****************************************************************
//**************** GLFW CALLBACK *********************************
//****************************************************************

void errorCallback(int e, const char *d)
{
    printf("Error %d: %s\n", e, d);
}

void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods)
{
    double win_cursor_x, win_cursor_y;
    glfwGetCursorPos(win, &win_cursor_x, &win_cursor_y);

    int fb_cursor_x = win_cursor_x*scale_x;
    int fb_cursor_y = win_cursor_y*scale_y;

    if(action == GLFW_PRESS)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_DOWN, fb_cursor_x, fb_cursor_y);
        }
    }
    else if(action == GLFW_RELEASE)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_MIDDLE_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
        else if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_UP, fb_cursor_x, fb_cursor_y);
        }
    }

    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = fb_cursor_x;
    coords.y = fb_cursor_y;
    coords.minX = 0;
    coords.maxX = 0;
    coords.minY = 0;
    coords.maxY = 0;
    coords.isScreen = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        SystemAbstraction::onPointerDown(0, &coords);
    }else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        SystemAbstraction::onPointerUp(0, &coords);
    }
}

void cursorPosCallback(GLFWwindow* win,double xpos, double ypos)
{
    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = xpos*scale_x;
    coords.y = ypos*scale_y;
    coords.minX = 0;
    coords.maxX = 0;
    coords.minY = 0;
    coords.maxY = 0;
    coords.isScreen = true;
    SystemAbstraction::onPointerMove(0, &coords);
}

void charCallback(GLFWwindow *win, unsigned int codepoint)
{
    SystemAbstraction::onChar(codepoint);
}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    SystemAbstraction::onKey((SystemAbstraction::ButtonEvent)action, (SystemAbstraction::Key)key, (SystemAbstraction::Mods)mods, 0, 0);

    if(action == GLFW_RELEASE && (key == 'q' || key == 'Q'))
    {
        SystemAbstraction::onBackKeyPressed();
    }
}

void scrollCallback(GLFWwindow*,double xoffset,double yoff)
{
    yoff = yoff*scale_y;
    SystemAbstraction::onScroll(yoff);
}

void framebufferSizeCallback(GLFWwindow * , int width, int height)
{
    int fb_width = width*scale_x;
    int fb_height = height*scale_y;
    SystemAbstraction::onFramebufferResize(fb_width, fb_height);
}

//****************************************************************
//**************** END CALLBACK **********************************
//****************************************************************


void timerForGlut(int value){
    SystemAbstraction::onTimerTick();
}


int main(int , char** )
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(EXIT_FAILURE);
    }
    // OPEN GL ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, __FILE__, NULL, NULL);
    glfwMakeContextCurrent(win);

    //GLFW CALLBACK
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetCursorPosCallback(win, cursorPosCallback);
    glfwSetCharCallback(win, charCallback);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetScrollCallback(win, scrollCallback);
    glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    //SCALE FAMEBUFFER TO WINDOW SIZE
    glfwGetWindowSize(win, &window_width, &window_height);
    glfwGetFramebufferSize(win, &framebuffer_width, &framebuffer_height);
    scale_x = (float)framebuffer_width/(float)window_width;
    scale_y = (float)framebuffer_height/(float)window_height;


    //SYSTEM ABSTRACTION INIT
    string homePath = getenv("HOME");
    string configPath = homePath + "/.config";
    systemInput_initConfigPath(configPath);
    SystemAbstraction::onInit(WINDOW_WIDTH, WINDOW_HEIGHT);


    framebufferSizeCallback(win,WINDOW_WIDTH,WINDOW_HEIGHT);

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    initFPS();

    printOpenGLVersion(); //this function must be invoked after Create Window
    enableMultisample(0);


    while (!glfwWindowShouldClose(win))
    {
        //cout << "FPS = "  << calculateFPS() << endl;

        glfwPollEvents();

        static bool isFirstFrame = true;
        if(isFirstFrame == true)
        {
            SystemAbstraction::onRenderFirstFrame();
            isFirstFrame = false;
        }else{
            SystemAbstraction::onRenderFrame();
        }

        static int time = 0;
        timerForGlut(time++);

        glfwSwapBuffers(win);
    }

    //DELETE
    SystemAbstraction::onUninit();
    glfwTerminate();
    return EXIT_SUCCESS;
}



void printOpenGLVersion(){

    GLenum err;
    err = glGetError(); //w celu ignorowania błędu 1282 INVALID ENUM
    GL_CHECK_ERRORS

            //wyprowadzanie informacji na ekran
            cout<<"\tProducent: "<<glGetString (GL_VENDOR)<<endl;
    cout<<"\tRenderer: "<<glGetString (GL_RENDERER)<<endl;
    cout<<"\tWersja OpenGL: "<<glGetString (GL_VERSION)<<endl;
    cout<<"\tGLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
}





const int FPS_BUFF_SIZE =  200;
static double tablica[FPS_BUFF_SIZE];
static int fps_index = 0;
static double suma = 0.0;

void initFPS()
{
    for(int i = 0; i < FPS_BUFF_SIZE; i++){
        tablica[i] = 0.0;
    }
}

double calculateFPS()
{
    static double current_time = 0;
    static double previous_time = 0;
    static double delta_time = 0;
    static double fps = 0;


    current_time = glfwGetTime();
    delta_time = current_time - previous_time;
    previous_time = current_time;

    fps = 1.0/delta_time;

    double tmp = tablica[fps_index];

    tablica[fps_index] = fps;
    fps_index++;

    if(fps_index == FPS_BUFF_SIZE)
        fps_index = 0;

    suma = suma + fps - tmp;
    fps  = suma/FPS_BUFF_SIZE;

    return fps;
}
