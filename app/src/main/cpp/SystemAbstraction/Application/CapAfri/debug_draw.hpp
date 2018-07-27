#ifndef RENDER_H
#define RENDER_H



#include <Box2D/Box2D.h>
#include <library_opengles_2/RectangleRenderer/Primitive_Renderer.hpp>

struct b2AABB;

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class GLESDebugDraw : public b2Draw
{
public:
    GLESDebugDraw();
    ~GLESDebugDraw();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...);

    void DrawAABB(b2AABB* aabb, const b2Color& color);

    void setProjectionView(glm::mat4 projection, glm::mat4 view);
private:
    PR_LineStrip lineStrip;
    glm::mat4 mProjection;
    glm::mat4 mView;
};


#endif
