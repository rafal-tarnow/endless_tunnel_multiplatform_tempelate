#include "PostProcessor.hpp"
#include <iostream>
#include <system_log.hpp>

const GLchar *vShaderCode = "#version 100                                               \n"
                            "                                                           \n"
                            "precision mediump float;                                   \n"
                            "                                                           \n"
                            "attribute vec4 vertex;     // <vec2 position, vec2 texCoords>      \n"
                            "                                                                   \n"
                            "varying vec2 TexCoords;                                            \n"
                            "                                                                   \n"
                            "uniform bool  black;                                               \n"
                            "uniform bool  background;                                          \n"
                            "uniform bool  chaos;                                               \n"
                            "uniform bool  confuse;                                             \n"
                            "uniform bool  shake;                                               \n"
                            "uniform bool  spin;                                                \n"
                            "uniform float time;                                                \n"
                            "                                                                   \n"
                            "void main()                                                        \n"
                            "{                                                                  \n"
                            "    gl_Position = vec4(vertex.xy, 0.0, 1.0);                       \n"
                            "    vec2 texture = vertex.zw;                                      \n"
                            "                                                                   \n"
                            "    if(chaos)                                                      \n"
                            "    {                                                              \n"
                            "        float strength = 0.3;                                      \n"
                            "        vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);   \n"
                            "        TexCoords = pos;                                                                       \n"
                            "    }                                                              \n"
                            "    else if(confuse)                                               \n"
                            "    {                                                              \n"
                            "        TexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);        \n"
                            "    }                                                              \n"
                            "    else if(spin || black || background)                           \n"
                            "    {                                                              \n"
                            "        TexCoords = texture;                                       \n"
                            "    }                                                              \n"
                            "    else                                                           \n"
                            "    {                                                              \n"
                            "        TexCoords = texture;                                       \n"
                            "    }                                                              \n"
                            "                                                                   \n"
                            "    if (shake)                                                     \n"
                            "    {                                                              \n"
                            "        float strength = 0.01;                                     \n"
                            "        gl_Position.x += cos(time * 10.0) * strength;              \n"
                            "        gl_Position.y += cos(time * 15.0) * strength;              \n"
                            "    }                                                              \n"
                            "}                                                                  \n";



const GLchar *fShaderCode = "#version 100                                                                   \n"
                            "                                                                               \n"
                            "precision mediump float;                                                       \n"
                            "                                                                               \n"
                            "varying  vec2  TexCoords;                                                      \n"
                            "vec4  color;                                                                   \n"
                            "                                                                               \n"
                            "uniform sampler2D textureMap;                                                  \n"
                            "uniform vec2      offsets[9];                                                  \n"
                            "uniform int       edge_kernel[9];                                              \n"
                            "uniform float     blur_kernel[9];                                              \n"
                            "                                                                               \n"
                            "uniform bool  black;                                                           \n"
                            "uniform bool  background;                                                      \n"
                            "uniform bool chaos;                                                            \n"
                            "uniform bool confuse;                                                          \n"
                            "uniform bool spin;                                                             \n"
                            "uniform bool shake;                                                            \n"
                            "uniform float time;                                                            \n"
                            "uniform float opaque;                                                            \n"
                            "uniform float spin_inversion_param;                                            \n"
                            "                                                                               \n"
                            "void main()                                                                    \n"
                            "{                                                                              \n"
                            "    color = vec4(0.0);                                                         \n"
                            "    vec3 sample[9];                                                            \n"
                            "    // sample from texture offsets if using convolution matrix                 \n"
                            "    if(chaos || shake || background)                                                         \n"
                            "        for(int i = 0; i < 9; i++)                                             \n"
                            "            sample[i] = vec3(texture2D(textureMap, TexCoords.st + offsets[i]));\n"
                            "                                                                               \n"
                            "    // process effects                                                         \n"
                            "    if(chaos)                                                                  \n"
                            "    {                                                                          \n"
                            "        for(int i = 0; i < 9; i++)                                             \n"
                            "            color += vec4(sample[i] * float(edge_kernel[i]), 0.0);             \n"
                            "        color.a = 1.0;                                                         \n"
                            "    }                                                                          \n"
                            "    else if(confuse)                                                           \n"
                            "    {                                                                          \n"
                            "        color = vec4(1.0 - texture2D(textureMap, TexCoords).rgb, 1.0);                 \n"
                            "    }                                                                                  \n"
                            "    else if(black)                                                                     \n"
                            "    {                                                                                  \n"
                            "       color = texture2D(textureMap, TexCoords);                                       \n"
                            "       float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;         \n"
                            "       color = vec4(average, average, average, 1.0);                                   \n"
                            "    }                                                                                  \n"
                            "    else if(background)                                                                \n"
                            "    {                                                                                  \n"
                            "        //for(int i = 0; i < 9; i++)                                                     \n"
                            "        //    color += vec4(sample[i] * blur_kernel[i], 0.0);                            \n"
                            "        //color.a = 1.0;                                                                 \n"
                            "                                                                                       \n"
                            "       //color = texture2D(textureMap, TexCoords);                                       \n"
                            "       //float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;         \n"
                            "       //color = vec4(average, average, average, opaque);                                \n"
                            "                                                                                       \n"
                            "       color = texture2D(textureMap, TexCoords);                                       \n"
                            "       color.a = opaque;                                                                  \n"
                            "    }                                                                                  \n"
                            "    else if(shake)                                                                     \n"
                            "    {                                                                                  \n"
                            "        for(int i = 0; i < 9; i++)                                                     \n"
                            "            color += vec4(sample[i] * blur_kernel[i], 0.0);                            \n"
                            "        color.a = 1.0;                                                                 \n"
                            "    }                                                                                  \n"
                            "    else if(spin)                                                                      \n"
                            "    {                                                                                  \n"
                            "       //przesuniecie poczętku wspolrzednych tekstury na srodek obrazu                 \n"
                            "       vec2 uv = TexCoords-0.5;                                                        \n"
                            "                                                                                       \n"
                            "       //obliczanie kota na podstawie przesunietych wspolrzednych kartezjanskich       \n"
                            "       float angle = atan(uv.y, uv.x);                                                 \n"
                            "                                                                                       \n"
                            "       //obliczanie promieniana podstawie przesunietych wspolrzednych kartezjanskich   \n"
                            "       float radius = length(uv);                                                      \n"
                            "                                                                                       \n"
                            "       //zwiekszanie kata o iloczym promienia i mocy filtra                            \n"
                            "       angle+= radius*2.0*cos(time/2.0);//*twirl_amount;                               \n"
                            "                                                                                       \n"
                            "       //powrot do wspolrzednych kartezjanskich                                        \n"
                            "       vec2 shifted = radius* vec2(cos(angle), sin(angle)) + 0.5;                      \n"
                            "                                                                                       \n"
                            "       //przesuniecie poczatku wspolrzednych tekstury do polozenia pierwotnego         \n"
                            "       if((shifted.x < 1.0) && (shifted.x > 0.0) && (shifted.y < 1.0) && (shifted.y > 0.0))    \n"
                            "       {                                                                                                                                   \n"
                            "                                                                                                                                           \n"
                            "           //color = vec4((texture2D(textureMap, TexCoords).rgb)*(1.0 - 2.0*spin_inversion_param) + spin_inversion_param, 1.0);            \n"
                            "           color = vec4(/*1.0 - */texture2D(textureMap, shifted).rgb, 1.0);                            \n"
                            "       }                                                                                           \n"
                            "       else                                                                                        \n"
                            "       {                                                                                           \n"
                            "           color = vec4(0.0, 0.0, 0.0, 1.0);                                                       \n"
                            "       }                                                                                           \n"
                            "    }                                                                          \n"
                            "    else                                                                       \n"
                            "    {                                                                          \n"
                            "        color =  texture2D(textureMap, TexCoords);                                  \n"
                            "    }                                                                          \n"
                            "                                                                               \n"
                            "    gl_FragColor = color;                                                      \n"
                            "}                                                                              \n";

PostProcessor::PostProcessor(GLuint width, GLuint height)
    : Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Spin(GL_FALSE), Black(GL_FALSE), Background(GL_FALSE), Shake(GL_FALSE), TWidth(0), THeight(0), Internal_Format(GL_RGB), Image_Format(GL_RGB),
      Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    LOGD("PostProcessor::PostProcessor(1)");
    PostProcessingShader.Compile(vShaderCode, fShaderCode);
    LOGD("PostProcessor::PostProcessor(2)");

    glGenTextures(1, &this->ID);

    // this->Texture.Generate(width, height, NULL);
    this->TWidth = width;
    this->THeight = height;
    // Create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, NULL);
    // Set Texture wrap and filter modes
    // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    // glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Initialize renderbuffer/framebuffer object

    glGenFramebuffers(1, &this->FBO);

    // Also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ID, 0); // Attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Initialize render data and uniforms
    this->initRenderData();
    this->PostProcessingShader.SetInteger("textureMap", 0, GL_TRUE);
    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
        {-offset, offset},  // top-left
        {0.0f, offset},     // top-center
        {offset, offset},   // top-right
        {-offset, 0.0f},    // center-left
        {0.0f, 0.0f},       // center-center
        {offset, 0.0f},     // center - right
        {-offset, -offset}, // bottom-left
        {0.0f, -offset},    // bottom-center
        {offset, -offset}   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat *)offsets);
    GLint edge_kernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    GLfloat blur_kernel[9] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16};
    glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);

    this->PostProcessingShader.SetFloat("spin_inversion_param", 0.0);
}

void PostProcessor::BeginRender(glm::vec4 clearColour)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::EndRender()
{
    // Now resolve multisampled color-buffer into intermediate FBO to store to texture
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    // glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::Render(GLfloat time)
{
    // Set uniforms/options
    this->PostProcessingShader.Use();
    this->PostProcessingShader.SetFloat("time", time);
    this->PostProcessingShader.SetInteger("confuse", this->Confuse);
    this->PostProcessingShader.SetInteger("chaos", this->Chaos);
    this->PostProcessingShader.SetInteger("spin", this->Spin);
    this->PostProcessingShader.SetInteger("black", this->Black);
    this->PostProcessingShader.SetInteger("background", this->Background);
    this->PostProcessingShader.SetInteger("shake", this->Shake);
    this->PostProcessingShader.SetFloat("opaque", this->opaque);
    // Render textured quad
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->ID);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    {
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PostProcessor::initRenderData()
{
    // Configure VAO/VBO

    GLfloat vertices[] = {// Pos        // TexCoord
                          -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f};
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
