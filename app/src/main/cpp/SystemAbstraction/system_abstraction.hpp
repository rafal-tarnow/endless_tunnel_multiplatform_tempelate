#pragma once

struct PointerCoords {
    float x, y;

    // was this generated by a touch on the screen? If not, this touch was
    // generated in a touch device that's not associated to the screen.
    bool isScreen;

    // motion range:
    float minX, minY;
    float maxX, maxY;
};

class SystemAbstraction{
public:
    typedef enum{
        MOUSE_LEFT_BUTTON,
        MOUSE_MIDDLE_BUTTON,
        MOUSE_RIGHT_BUTTON,

        MOUSE_LEFT,
        MOUSE_ENTERED
    }MouseButton;

    typedef enum{
        EVENT_DOWN,
        EVENT_UP,
    }ButtonEvent;

    static void onInit(unsigned int width, unsigned int height);
    static void onPause();
    static void onResume();
    static void onResize(unsigned int width, unsigned int height);
    static void onRenderFirstFrame();
    static void onRenderFrame();
    static void onScroll(double yoffset);
    static void onMouseButton(MouseButton mouseButton, ButtonEvent event, int x, int y);
    static void onKeyboard(ButtonEvent, int key, int x, int y );
    static bool onBackKeyPressed();
    static void onJoyUpdate(float joyX, float joyY);
    static void onPointerDown(int pointerId, const struct PointerCoords *coords);
    static void onPointerUp(int pointerId, const struct PointerCoords *coords);
    static void onPointerMove(int pointerId, const struct PointerCoords *coords);
    static void onTimerTick();
    static void onUninit();
};
