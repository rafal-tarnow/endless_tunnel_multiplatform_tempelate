//#include <iostream>
//using namespace std;

//#include "../app/src/main/cpp/SystemAbstraction/Application/data/design_graffiti_agentorange_www_myfontfree_com.ttf.hpp"
//#include "../app/src/main/cpp/SystemAbstraction/system_log.hpp"

//#include <ft2build.h>
//#include FT_FREETYPE_H

//#include <bitset>

//void drawGlyphToConsole(FT_Face &face){
//    for(unsigned int i = 0; i < face->glyph->bitmap.rows; i++){
//        for(unsigned int j = 0; j < face->glyph->bitmap.width; j++){

//            int alpha_value =   (int)(face->glyph->bitmap.buffer[i*face->glyph->bitmap.width + j]) / 26;

//            if(alpha_value > 0){
//                LOGD("%d", alpha_value);
//            }else{
//                LOGD(" ", alpha_value);
//            }


//        }
//        LOGD("\n");
//    }
//}

//int main()
//{
//    FT_Library ft;

//    if (FT_Init_FreeType(&ft))
//        LOGD("ERROR::FREETYPE: Could not init FreeType Library\n");

//    FT_Face face;
//    if (FT_New_Memory_Face(ft,design_graffiti_agentorange_www_myfontfree_com_ttf, size_of_design_graffiti_agentorange_www_myfontfree_com_ttf , 0, &face))
//        LOGD("ERROR::FREETYPE: Failed to load font\n");

//    FT_Set_Pixel_Sizes(face, 0, 32);

//    if (FT_Load_Char(face, 'W', FT_LOAD_RENDER /*| FT_LOAD_TARGET_MONO*/))
//        LOGD("ERROR::FREETYTPE: Failed to load Glyph\n");

//    LOGD("width = %d\n", face->glyph->metrics.width);
//    LOGD("height = %d\n", face->glyph->metrics.height);

//    LOGD("bitmap.width = %d\n", face->glyph->bitmap.width);
//    LOGD("bitmap.rows = %d\n", face->glyph->bitmap.rows);
//    LOGD("bitmap.pitch = %d\n", face->glyph->bitmap.pitch);

//    drawGlyphToConsole(face);

//    FT_Done_FreeType(ft);
//    return 0;
//}


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

static  GLuint CURRENT_WINDOW_WIDTH = 320;
static  GLuint CURRENT_WINDOW_HEIGHT = 240;

GLFWwindow* window;


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

void initialization()
{
    string configPath = getenv("HOME");
    configPath += "/.config";
    systemInput_initConfigPath(configPath);
    SystemAbstraction::onInit(CURRENT_WINDOW_WIDTH, CURRENT_WINDOW_HEIGHT);
}


/* reshaped window */
void reshape(GLFWwindow * , int width, int height) {
    CURRENT_WINDOW_HEIGHT = height;
    CURRENT_WINDOW_WIDTH = width;

    SystemAbstraction::onResize(CURRENT_WINDOW_WIDTH, CURRENT_WINDOW_HEIGHT);

}

static bool isFirstFrame = true;

void renderFunction()
{
    if(isFirstFrame == true)
    {
        SystemAbstraction::onRenderFirstFrame();
        isFirstFrame = false;
    }else{
        SystemAbstraction::onRenderFrame();
    }
}

void scroll_callback(GLFWwindow*,double xoffset,double yoffset){
    cout << "scroll_callback xoffset = " << xoffset << " yoffset = " << yoffset << endl;
    SystemAbstraction::onScroll(yoffset);
}

void cursor_position_callbask(GLFWwindow*,double,double)
{
    //cout << "cursor_position_callbask(GLFWwindow*,double,double)" << endl;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = xpos;
    coords.y = ypos;
    coords.minX = 0;
    coords.maxX = 0;
    coords.minY = 0;
    coords.maxY = 0;
    coords.isScreen = true;

    SystemAbstraction::onPointerMove(0, &coords);

}

void mouse_button_callback(GLFWwindow* , int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_LEFT_BUTTON, SystemAbstraction::EVENT_DOWN, xpos, ypos);
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        SystemAbstraction::onMouseButton(SystemAbstraction::MOUSE_RIGHT_BUTTON, SystemAbstraction::EVENT_DOWN, xpos, ypos);
    }


    PointerCoords coords;
    memset(&coords, 0, sizeof(coords));
    coords.x = xpos;
    coords.y = ypos;
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

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{

    if(action == GLFW_PRESS){
        SystemAbstraction::onKeyboard(SystemAbstraction::EVENT_DOWN, key, 0 , 0);
    }else if(action == GLFW_RELEASE){
        SystemAbstraction::onKeyboard(SystemAbstraction::EVENT_UP, key, 0 , 0);
    }

    if(action == GLFW_RELEASE && (key == 'q' || key == 'Q'))
    {
        SystemAbstraction::onBackKeyPressed();
    }
}

void timerForGlut(int value){
    SystemAbstraction::onTimerTick();
}



int main(int , char** )
{
    InitGLFWWindow();
    printOpenGLVersion(); //this function must be invoked after Create Window
    enableMultisample(0);


    while (!glfwWindowShouldClose(window)) {
        //cout << "FPS = "  << calculateFPS() << endl;
        glfwPollEvents();
        {
            renderFunction();
            static int time = 0;
            timerForGlut(time++);
        }
        glfwSwapBuffers(window);
    }

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

void InitGLFWWindow(){
    glfwInit();

    if(1){ // OPEN GL ES
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    }else{
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }

    window = glfwCreateWindow(CURRENT_WINDOW_WIDTH, CURRENT_WINDOW_HEIGHT, __FILE__, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, reshape);
    glfwSetMouseButtonCallback(window, mouse_button_callback) ;
    glfwSetCursorPosCallback(window, cursor_position_callbask);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    initialization();
    reshape(window,CURRENT_WINDOW_WIDTH,CURRENT_WINDOW_HEIGHT);

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

    initFPS();
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
