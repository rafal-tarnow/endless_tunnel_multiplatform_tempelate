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
#include <cstdio>
#include "../../system_log.hpp"

#include "anim.hpp"
#include "ascii_to_geom.hpp"
#include "our_shader.hpp"
#include "play_capafri_scene.hpp"
#include "welcome_scene.hpp"
#include "./data/cube_geom.inl"
#include "./data/tunnel_geom.inl"
#include "./data/strings.inl"

// colors for menus
static const float MENUITEM_SEL_COLOR[] = { 1.0f, 1.0f, 0.0f };
static const float MENUITEM_COLOR[] = { 1.0f, 1.0f, 1.0f };

PlayCapAfriScene::PlayCapAfriScene() : Scene() {
    mmTrivialShader = NULL;
    mmTextRenderer = NULL;
    mmShapeRenderer = NULL;

    memset(mMenuItemText, 0, sizeof(mMenuItemText));
    mMenuItemText[MENUITEM_UNPAUSE] = S_UNPAUSE;
    mMenuItemText[MENUITEM_QUIT] = S_QUIT;
    mMenuItemText[MENUITEM_START_OVER] = S_START_OVER;
    mMenuItemText[MENUITEM_RESUME] = S_RESUME;

    memset(mMenuItems, 0, sizeof(mMenuItems));
    mMenuItemCount = 0;

    mMenu = MENU_NONE;
    mMenuSel = 0;

    mMenuTouchActive = false;
}

void PlayCapAfriScene::OnStartGraphics(int width, int height) {
    current_width = width;
    current_height = height;

    game = new Game(current_width, current_height);
    game->systemCallback_WindowResize(current_width, current_height);

    mmTrivialShader = new TrivialShader();
    mmTrivialShader->Compile();

    mmTextRenderer = new TextRenderer(mmTrivialShader);
    mmShapeRenderer = new ShapeRenderer(mmTrivialShader);
}

void PlayCapAfriScene::OnKillGraphics()
{
    delete game;
    CleanUp(&mmTextRenderer);
    CleanUp(&mmShapeRenderer);
    CleanUp(&mmTrivialShader);
}

void PlayCapAfriScene::DoFrame()
{

    if (mMenu) {
        RenderMenu();
        return;
    }


    game->systemCallback_Render();
    game->systemCallback_TimerTick();

}
void PlayCapAfriScene::UpdateMenuSelFromTouch(float x, float y) {
    float sh = SceneManager::GetInstance()->GetScreenHeight();
    int item = (int)floor((y / sh) * (mMenuItemCount));
    mMenuSel = Clamp(item, 0, mMenuItemCount - 1);
}

void PlayCapAfriScene::OnPointerDown(int pointerId, const struct PointerCoords *coords) {
    float x = coords->x, y = coords->y;
    if (mMenu) {
        if (coords->isScreen) {
            UpdateMenuSelFromTouch(x, y);
            mMenuTouchActive = true;
        }
    }
    else {

        if (coords->x > current_width / 2) {
            game->systemCallback_keyboard(SystemAbstraction::EVENT_DOWN, 'd', coords->x, coords->y);
        }

        if (coords->x < current_width / 2) {
            game->systemCallback_keyboard(SystemAbstraction::EVENT_DOWN, 'a', coords->x, coords->y);
        }
    }
}

void PlayCapAfriScene::OnPointerUp(int pointerId, const struct PointerCoords *coords)
{
    if (mMenu && mMenuTouchActive)
    {
        if (coords->isScreen)
        {
            mMenuTouchActive = false;
            HandleMenu(mMenuItems[mMenuSel]);
        }
    }
    else
    {
        if (coords->x > current_width / 2)
        {
            game->systemCallback_keyboard(SystemAbstraction::EVENT_UP, 'd', coords->x, coords->y);
        }

        if (coords->x < current_width / 2)
        {
            game->systemCallback_keyboard(SystemAbstraction::EVENT_UP, 'a', coords->x, coords->y);
        }
    }
}

void PlayCapAfriScene::OnPointerMove(int pointerId, const struct PointerCoords *coords) {
    float x = coords->x, y = coords->y;
    if (mMenu && mMenuTouchActive) {
        UpdateMenuSelFromTouch(x, y);
    }
}

void PlayCapAfriScene::RenderMenu() {
    glClearColor(0.0,0.0,0.0,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    glm::mat4 modelMat;
    glm::mat4 mat;

    glDisable(GL_DEPTH_TEST);

    RenderBackgroundAnimation(mmShapeRenderer);

    float scaleFactor = SineWave(1.0f, MENUITEM_PULSE_AMOUNT, MENUITEM_PULSE_PERIOD, 0.0f);

    int i;
    for (i = 0; i < mMenuItemCount; i++) {
        float thisFactor = (mMenuSel == i) ? scaleFactor : 1.0f;
        float y = 1.0f - (i + 1) / ((float)mMenuItemCount + 1);
        float x = aspect * 0.5f;
        mmTextRenderer->SetFontScale(thisFactor * MENUITEM_FONT_SCALE);
        mmTextRenderer->SetColor(mMenuSel == i ? MENUITEM_SEL_COLOR : MENUITEM_COLOR);
        mmTextRenderer->RenderText(mMenuItemText[mMenuItems[i]], x, y);
    }
    mmTextRenderer->ResetColor();

    glEnable(GL_DEPTH_TEST);
}


bool PlayCapAfriScene::OnBackKeyPressed() {
    if (mMenu) {
        // reset frame clock so that the animation doesn't jump:
        //mFrameClock.Reset();

        // leave menu
        ShowMenu(MENU_NONE);
    } else {
        // enter pause menu
        ShowMenu(MENU_PAUSE);
    }
    return true;
}

void PlayCapAfriScene::OnJoy(float joyX, float joyY) {

}

void PlayCapAfriScene::OnKeyDown(int keyCode)
{
    if (mMenu) {
        if (keyCode == OURKEY_UP) {
            mMenuSel = mMenuSel > 0 ? mMenuSel - 1 : mMenuSel;
        }
        else if (keyCode == OURKEY_DOWN) {
            mMenuSel = mMenuSel + 1 < mMenuItemCount ? mMenuSel + 1 : mMenuSel;
        }
        else if (keyCode == OURKEY_ENTER) {
            HandleMenu(mMenuItems[mMenuSel]);
        }
    }
}

void PlayCapAfriScene::ShowMenu(int menu) {
    mMenu = menu;
    mMenuSel = 0;
    switch (menu) {
        case MENU_PAUSE:
            mMenuItems[0] = MENUITEM_UNPAUSE;
            mMenuItems[1] = MENUITEM_QUIT;
            mMenuItemCount = 2;
            break;
        case MENU_LEVEL:
            mMenuItems[0] = MENUITEM_RESUME;
            mMenuItems[1] = MENUITEM_START_OVER;
            mMenuItemCount = 2;
            break;
        default:
            // since we're leaving the menu, reset the frame clock to avoid a skip
            // in the animation
            //mFrameClock.Reset();
            ;
    }
}

void PlayCapAfriScene::HandleMenu(int menuItem) {
    switch (menuItem) {
        case MENUITEM_QUIT:
            SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
            break;
        case MENUITEM_UNPAUSE:
            ShowMenu(MENU_NONE);
            break;
        case MENUITEM_RESUME:
            // resume from saved level
            //mDifficulty = (mSavedCheckpoint / LEVELS_PER_CHECKPOINT) * LEVELS_PER_CHECKPOINT;
            //SetScore(SCORE_PER_LEVEL * mDifficulty);
            //mObstacleGen.SetDifficulty(mDifficulty);
            //ShowLevelSign();
            ShowMenu(MENU_NONE);
            break;
        case MENUITEM_START_OVER:
            // start over from scratch
            ShowMenu(MENU_NONE);
            break;
    }
}

void PlayCapAfriScene::OnPause() {
    if (mMenu == MENU_NONE) {
        ShowMenu(MENU_PAUSE);
    }
}

void PlayCapAfriScene::OnScreenResized(int width, int height) {
    current_width = width;
    current_height = height;
    game->systemCallback_WindowResize(width, height);
}


