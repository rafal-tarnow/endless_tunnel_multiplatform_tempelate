//
#ifndef PostProcessor_hpp
#define PostProcessor_hpp

#include <system_opengl_include.hpp>
#include <glm/glm.hpp>


#include "Shader.hpp"

// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or
// Shake boolean.
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
    // State
    LShader PostProcessingShader;
    GLuint Width, Height;
    // Options
    GLboolean Confuse, Chaos, Shake;
    // Constructor
    PostProcessor(GLuint width, GLuint height);
    // Prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // Should be called after rendering the game, so it stores all the rendered data into a texture object
    void EndRender();
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render(GLfloat time);
private:
    // Render state
    GLuint /*MSFBO,*/ FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    /*GLuint RBO;*/ // RBO is used for multisampled color buffer
    //GLuint VAO;
    GLuint VBO;

    GLuint ID;
    // Texture image dimensions
    GLuint TWidth, THeight; // Width and height of loaded image in pixels
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
};

#endif /* PostProcessor_hpp */
