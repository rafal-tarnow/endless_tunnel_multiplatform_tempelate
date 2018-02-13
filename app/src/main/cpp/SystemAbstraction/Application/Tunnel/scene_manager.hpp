/*
 * Copyright (C) Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef endlesstunnel_scene_manager_h
#define endlesstunnel_scene_manager_h

#include "our_key_codes.hpp"

class Scene;

/* Scene manager (singleton). The scene manager is responsible for managing the
 * currently active scene (class Scene) and delivering events to it. */
class SceneManager {
    private:
        Scene* mCurScene;
        int mScreenWidth, mScreenHeight;
        bool mHasGraphics;
        Scene *mSceneToInstall;
        void InstallScene(Scene *newScene);

    public:
        SceneManager();
        void SetScreenSize(int width, int height);
        void KillGraphics();
        void StartGraphics(int width, int height);

        // Returns screen width in pixels
        int GetScreenWidth() { return mScreenWidth; }

        // Returns screen height in pixels
        int GetScreenHeight() { return mScreenHeight; }

        // Returns screen's aspect ratio
        float GetScreenAspect() {
            return (float)mScreenWidth/mScreenHeight;
        }

        // Returns whether or not we have graphics (i.e. OpenGL context etc)
        bool HasGraphics() {
            return mHasGraphics;
        }

        // Returns current scene
        Scene* GetScene();

        // Renders current scene
        void DoFrame();

        // Reports that a pointer (e.g. touchscreen, touchpad, etc) went down
        void OnPointerDown(int pointerId, const struct PointerCoords *coords);

        // Reports that a pointer (e.g. touchscreen, touchpad, etc) went up
        void OnPointerUp(int pointerId, const struct PointerCoords *coords);

        // Reports that a pointer (e.g. touchscreen, touchpad, etc) moved
        void OnPointerMove(int pointerId, const struct PointerCoords *coords);


        void OnScroll(double yoffset);

        // Reports that the back key was pressed
        bool OnBackKeyPressed();

        // Reports that the given key went down
        void OnKeyDown(int ourKeycode);
        
        // Reports that the given key went up
        void OnKeyUp(int ourKeycode);

        // Reports a joystick update
        void UpdateJoy(float joyX, float joyY);

        // Reports that the game was paused (e.g. Activity got an onPause())
        void OnPause();

        // Reports that the game was resumed (e.g. Activity got an onResume())
        void OnResume();

        // Requests that a new scene be installed, replacing the currently active
        // scene. The new scene will be installed on the next DoFrame() call.
        void RequestNewScene(Scene *newScene);

        // Returns the (singleton) instance of SceneManager.
        static SceneManager* GetInstance();
};

#endif

