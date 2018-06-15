#pragma once

#include <cstddef>
#include <cstdarg>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS
#include "../nuklear.h"

#include <GLES2/gl2.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

struct backend_device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
    void *vertices;
    void *elements;
    GLsizei vs;
    size_t vp, vt, vc;
};

struct nk_glfw_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

struct nk_image icon_load(const char *filename);
struct nk_image icon_load_from_memory(unsigned char *fileData, int fileSize);


void backend_init(struct backend_device *dev);
void backend_upload_atlas(struct backend_device *dev, const void *image, int width, int height);
void backend_device_draw(struct backend_device *dev, struct nk_context *ctx, int width, int height, enum nk_anti_aliasing AA);
void backdend_shutdown(struct backend_device *dev);
