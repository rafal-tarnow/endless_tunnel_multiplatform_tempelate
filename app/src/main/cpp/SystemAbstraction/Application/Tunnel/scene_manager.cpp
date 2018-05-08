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

#include "../../system_log.hpp"
#include "common.hpp"
#include "scene.hpp"
#include "scene_manager.hpp"

static SceneManager _sceneManager;

SceneManager::SceneManager() {
    mCurScene = NULL;

    // start with non-bogus (though not accurate) values
    mScreenWidth = 320;
    mScreenHeight = 240;

    mSceneToInstall = NULL;

    mHasGraphics = false;
}

void SceneManager::RequestNewScene(Scene *newScene) {
    LOGD("SceneManager: requesting new scene %p", newScene);
    mSceneToInstall = newScene;
}

void SceneManager::InstallScene(Scene *newScene) {
    LOGD("SceneManager: installing scene %p.", newScene);

    // kill graphics, if we have them.
    bool hadGraphics = mHasGraphics;
    if (mHasGraphics) {
        KillGraphics();
    }

    // If we have an existing scene, uninstall it.
    if (mCurScene) {
        mCurScene->OnUninstall();
        delete mCurScene;
        mCurScene = NULL;
    }

    // install the new scene
    mCurScene = newScene;
    if (mCurScene) {
        mCurScene->OnInstall();
    }

    // if we had graphics before, start them again.
    if (hadGraphics) {
        StartGraphics(mScreenWidth, mScreenHeight);
    }
}

Scene* SceneManager::GetScene() {
    return mCurScene;
}

void SceneManager::DoFrame() {
    if (mSceneToInstall) {
        InstallScene(mSceneToInstall);
        mSceneToInstall = NULL;
    }

    if (mHasGraphics && mCurScene) {
        mCurScene->DoFrame();
    }
}

void SceneManager::KillGraphics() {
    if (mHasGraphics) {
        LOGD("SceneManager: killing graphics.");
        mHasGraphics = false;
        if (mCurScene) {
            mCurScene->OnKillGraphics();
        }
    }
}

void SceneManager::StartGraphics(int width, int height) {
    this->SetScreenSize(width,height);

    if (!mHasGraphics) {
        LOGD("SceneManager: starting graphics.");
        mHasGraphics = true;
        if (mCurScene) {
            LOGD("SceneManager: calling mCurScene->OnStartGraphics.");
            mCurScene->OnStartGraphics(mScreenWidth, mScreenHeight);
        }
    }
}


void SceneManager::SetScreenSize(int width, int height) {
    if (mScreenWidth != width || mScreenHeight != height) {
        mScreenWidth = width;
        mScreenHeight = height;

        if (mCurScene && mHasGraphics) {
            mCurScene->OnFramebufferResized(width, height);
        }
    }
}

SceneManager* SceneManager::GetInstance() {
    return &_sceneManager;
}

void SceneManager::OnPointerDown(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerDown(pointerId, coords);
    }
}

void SceneManager::OnPointerUp(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerUp(pointerId, coords);
    }
}

void SceneManager::OnMouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y)
{
    if (mHasGraphics && mCurScene) {
        mCurScene->OnMouseButton(mouseButton, event, x, y);
    }
}

void SceneManager::OnMouseMove(int x, int y)
{
    if (mHasGraphics && mCurScene) {
        mCurScene->OnMouseMove(x, y);
    }
}

void SceneManager::OnScroll(double yoffset)
{
    if (mHasGraphics && mCurScene) {
        mCurScene->OnScroll(yoffset);
    }
}

void SceneManager::OnPointerMove(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerMove(pointerId, coords);
    }
}

bool SceneManager::OnBackKeyPressed() {
    if (mHasGraphics && mCurScene) {
        return mCurScene->OnBackKeyPressed();
    }
    return false;
}

void  SceneManager::OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y){
    if (mHasGraphics && mCurScene) {
        mCurScene->OnKey(event, key,  mods, x, y);
    }
}

void SceneManager::OnChar(unsigned int codepoint)
{
    if (mHasGraphics && mCurScene)
    {
        mCurScene->OnChar(codepoint);
    }
}

void SceneManager::UpdateJoy(float joyX, float joyY) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnJoy(joyX, joyY);
    }
}

void SceneManager::OnPause() {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPause();
    }
}

void SceneManager::OnResume() {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnResume();
    }
}



