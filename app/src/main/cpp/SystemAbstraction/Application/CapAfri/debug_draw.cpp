#include "debug_draw.hpp"

#include <GLES2/gl2.h>
#include <cstdio>
#include <cstdarg>

#include <cstring>
#include <iostream>

using namespace std;


GLESDebugDraw::GLESDebugDraw()
{
    glm::vec3 verticles[4];
    PR_init(&lineStrip, verticles, 4, glm::vec4(1.0, 0.0, 0.0, 1.0), GL_LINE_LOOP, GL_DYNAMIC_DRAW);
}

GLESDebugDraw::~GLESDebugDraw()
{
    PR_delete(&lineStrip);
}

void GLESDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    cout << "GLESDebugDraw::DrawPolygon()" << endl;
    vector<glm::vec3> vert;
    for(int32 i = 0; i < vertexCount; i++)
    {
        vert.push_back(glm::vec3(vertices[i].x, vertices[i].y, 2.0f));
    }
    PR_setMode(&lineStrip, GL_LINE_LOOP);
    PR_setColour(&lineStrip, glm::vec4(color.r, color.g, color.b, color.a));
    PR_updateData(&lineStrip, vert.data(), vert.size());
    lineStrip.projection = mProjection;
    lineStrip.view = mView;
    PR_draw(&lineStrip, 1.0);
}

void GLESDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    cout << "GLESDebugDraw::DrawSolidPolygon()" << endl;

    vector<glm::vec3> vert;
    for(int32 i = 0; i < vertexCount; i++)
    {
        vert.push_back(glm::vec3(vertices[i].x, vertices[i].y, 2.0f));
    }
    PR_setMode(&lineStrip, GL_TRIANGLE_FAN);
    PR_setColour(&lineStrip, glm::vec4(color.r, color.g, color.b, color.a));
    PR_updateData(&lineStrip, vert.data(), vert.size());
    lineStrip.projection = mProjection;
    lineStrip.view = mView;
    PR_draw(&lineStrip, 1.0);

}

void GLESDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    cout << "GLESDebugDraw::DrawCircle()" << endl;

    const float32 k_segments = 16.0f;
    int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;

    glm::vec3 glVertices[vertexCount];
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertices[i] = glm::vec3(v.x, v.y, 2.0);
        theta += k_increment;
    }

    PR_setMode(&lineStrip, GL_LINE_LOOP);
    PR_setColour(&lineStrip, glm::vec4(color.r, color.g, color.b, color.a));
    PR_updateData(&lineStrip, glVertices, vertexCount);
    lineStrip.projection = mProjection;
    lineStrip.view = mView;
    PR_draw(&lineStrip, 1.0);
}

void GLESDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    const float32 k_segments = 16.0f;
    int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;

    glm::vec3 glVertices[vertexCount];
    for (int32 i = 0; i < k_segments; ++i)
    {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        glVertices[i] = glm::vec3(v.x, v.y, 2.0);
        theta += k_increment;
    }

    PR_setMode(&lineStrip, GL_TRIANGLE_FAN);
    PR_setColour(&lineStrip, glm::vec4(color.r, color.g, color.b, color.a));
    PR_updateData(&lineStrip, glVertices, vertexCount);
    lineStrip.projection = mProjection;
    lineStrip.view = mView;
    PR_draw(&lineStrip, 1.0);

    // Draw the axis line
    DrawSegment(center,center+radius*axis,color);
}

void GLESDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    //glColor4f(color.r, color.g, color.b,1);
    GLfloat				glVertices[] = {
        p1.x,p1.y,p2.x,p2.y
    };
    //glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glDrawArrays(GL_LINES, 0, 2);
}

void GLESDebugDraw::DrawTransform(const b2Transform& xf)
{
    //b2Vec2 p1 = xf.position, p2;
    const float32 k_axisScale = 0.4f;

    //p2 = p1 + k_axisScale * xf.R.col1;
    //DrawSegment(p1,p2,b2Color(1,0,0));

    //p2 = p1 + k_axisScale * xf.R.col2;
    //DrawSegment(p1,p2,b2Color(0,1,0));
}

void GLESDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    cout << "GLESDebugDraw::DrawPoint()" << endl;
    vector<glm::vec3> vert;

    vert.push_back(glm::vec3(p.x, p.y, 2.0f));

    PR_setPointSize(&lineStrip, size);
    PR_setMode(&lineStrip, GL_POINTS);
    PR_setColour(&lineStrip, glm::vec4(color.r, color.g, color.b, color.a));
    PR_updateData(&lineStrip, vert.data(), vert.size());
    lineStrip.projection = mProjection;
    lineStrip.view = mView;
    PR_draw(&lineStrip, 1.0);
}

void GLESDebugDraw::DrawString(int x, int y, const char *string, ...)
{

    /* Unsupported as yet. Could replace with bitmap font renderer at a later date */
}

void GLESDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{

    //glColor4f(c.r, c.g, c.b,1);

    GLfloat				glVertices[] = {
        aabb->lowerBound.x, aabb->lowerBound.y,
        aabb->upperBound.x, aabb->lowerBound.y,
        aabb->upperBound.x, aabb->upperBound.y,
        aabb->lowerBound.x, aabb->upperBound.y
    };
    //glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glDrawArrays(GL_LINE_LOOP, 0, 8);

}

void GLESDebugDraw::setProjectionView(glm::mat4 projection, glm::mat4 view)
{
    mProjection = projection;
    mView = view;
}
