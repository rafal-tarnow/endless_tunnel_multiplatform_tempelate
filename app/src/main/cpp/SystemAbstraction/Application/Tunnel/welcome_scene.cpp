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
#include "anim.hpp"
#include "dialog_scene.hpp"
#include "play_scene.hpp"
#include "play_capafri_scene.hpp"
#include "map_editor_scene.hpp"
#include "welcome_scene.hpp"

#include "./data/blurb.inl"
#include "./data/strings.inl"

#define TITLE_POS center, 0.85f
#define TITLE_FONT_SCALE 1.0f
#define TITLE_COLOR 0.0f, 1.0f, 0.0f

// button defaults:
#define BUTTON_COLOR 0.0f, 1.0f, 0.0f
#define BUTTON_SIZE 0.2f, 0.2f
#define BUTTON_FONT_SCALE 0.5f

// button geometry
#define BUTTON_PLAY_POS center, 0.5f
#define BUTTON_PLAY_SIZE 0.4f, 0.4f
#define BUTTON_PLAY_FONT_SCALE 1.0f

// button geometry
#define BUTTON_ENDLESS_TUNNEL_POS center, 0.15f
#define BUTTON_ENDLESS_TUNNEL_SIZE 0.7f, 0.2f
#define BUTTON_ENDLESS_TUNNEL_FONT_SCALE 0.7f

#define BUTTON_MAP_EDITOR_POS center + 0.6f, 0.15f
#define BUTTON_MAP_EDITOR_SIZE 0.4f, 0.2f
#define BUTTON_MAP_EDITOR_FONT_SCALE 0.5f

// size of all side buttons (story, about)
#define BUTTON_SIDEBUTTON_WIDTH (center - 0.4f)
#define BUTTON_SIDEBUTTON_HEIGHT 0.2f
#define BUTTON_SIDEBUTTON_SIZE BUTTON_SIDEBUTTON_WIDTH, BUTTON_SIDEBUTTON_HEIGHT

// position of each side button (the buttons on the sides of the PLAY button)
#define BUTTON_STORY_POS 0.1f + 0.5f * BUTTON_SIDEBUTTON_WIDTH, 0.5f
#define BUTTON_ABOUT_POS center + 0.3f + 0.5f * BUTTON_SIDEBUTTON_WIDTH, 0.5f



WelcomeScene::WelcomeScene() {
}

WelcomeScene::~WelcomeScene() {
}

void WelcomeScene::RenderBackground() {
    RenderBackgroundAnimation(mShapeRenderer);
}

void WelcomeScene::OnButtonClicked(int id) {
    SceneManager *mgr = SceneManager::GetInstance();

    if (id == mPlayCapAfriButtonId)
    {
        mgr->RequestNewScene(new PlayCapAfriScene());
    }
    else if(id == mmPlayTunnelButtonId)
    {
        mgr->RequestNewScene(new PlayScene());
    }
    else if(id == mMapEditorButtonId)
    {
        mgr->RequestNewScene(new MapEditorScene());
    }
    else if (id == mStoryButtonId)
    {
        mgr->RequestNewScene((new DialogScene())->SetText(BLURB_STORY)->SetSingleButton(S_OK, DialogScene::ACTION_RETURN));
    }
    else if (id == mAboutButtonId)
    {
        mgr->RequestNewScene((new DialogScene())->SetText(BLURB_ABOUT)->SetSingleButton(S_OK, DialogScene::ACTION_RETURN));
    }
}

void WelcomeScene::DoFrame() {
    // update widget states based on signed-in status
    UpdateWidgetStates();

    // if the sign in or cloud save process is in progress, show a wait screen. Otherwise, not:
    SetWaitScreen(false);

    // draw the UI
    UiScene::DoFrame();
}

void WelcomeScene::UpdateWidgetStates() {
    // Build navigation
    AddNav(mPlayCapAfriButtonId, UI_DIR_LEFT, mStoryButtonId);
    AddNav(mPlayCapAfriButtonId, UI_DIR_RIGHT, mAboutButtonId);

    AddNav(mStoryButtonId, UI_DIR_RIGHT, mPlayCapAfriButtonId);

    AddNav(mAboutButtonId, UI_DIR_LEFT, mPlayCapAfriButtonId);

    AddNav(mPlayCapAfriButtonId, UI_DIR_DOWN, mmPlayTunnelButtonId);
    AddNav(mmPlayTunnelButtonId, UI_DIR_UP, mPlayCapAfriButtonId);
    AddNav(mmPlayTunnelButtonId, UI_DIR_RIGHT, mMapEditorButtonId);
    AddNav(mmPlayTunnelButtonId, UI_DIR_LEFT, mStoryButtonId);

    AddNav(mMapEditorButtonId, UI_DIR_UP, mAboutButtonId);
    AddNav(mMapEditorButtonId, UI_DIR_LEFT, mmPlayTunnelButtonId);

}

void WelcomeScene::OnStartGraphics(int width, int height) {
    UiScene::OnStartGraphics(width, height);
}

void WelcomeScene::OnCreateWidgets() {

    // create widgets
    float maxX = SceneManager::GetInstance()->GetScreenAspect();
    float center = 0.5f * maxX;

    // create the static title
    NewWidget()->SetText(S_TITLE)->SetCenter(TITLE_POS)->SetTextColor(TITLE_COLOR)
            ->SetFontScale(TITLE_FONT_SCALE)->SetTransition(UiWidget::TRANS_FROM_TOP);

    // create the "play" button
    mPlayCapAfriButtonId = NewWidget()->SetText(S_PLAY)->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_PLAY_POS)->SetSize(BUTTON_PLAY_SIZE)
            ->SetFontScale(BUTTON_PLAY_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    // create the "Endless Tunnel" button
    mmPlayTunnelButtonId = NewWidget()->SetText(S_ENDLESS_TUNNEL)->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_ENDLESS_TUNNEL_POS)->SetSize(BUTTON_ENDLESS_TUNNEL_SIZE)
            ->SetFontScale(BUTTON_ENDLESS_TUNNEL_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    // create the "Map Editor" button
    mMapEditorButtonId = NewWidget()->SetText(S_MAP_EDITOR)->SetTextColor(BUTTON_COLOR)
            ->SetCenter(BUTTON_MAP_EDITOR_POS)->SetSize(BUTTON_MAP_EDITOR_SIZE)
            ->SetFontScale(BUTTON_MAP_EDITOR_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    // story button
    mStoryButtonId = NewWidget()->SetTextColor(BUTTON_COLOR)->SetText(S_STORY)
            ->SetCenter(BUTTON_STORY_POS)->SetSize(BUTTON_SIDEBUTTON_SIZE)
            ->SetFontScale(BUTTON_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)->GetId();

    // about button
    mAboutButtonId = NewWidget()->SetTextColor(BUTTON_COLOR)->SetText(S_ABOUT)
            ->SetCenter(BUTTON_ABOUT_POS)->SetSize(BUTTON_SIDEBUTTON_SIZE)
            ->SetFontScale(BUTTON_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)->GetId();

    // "Play" button is the default button
    SetDefaultButton(mPlayCapAfriButtonId);

    // enable/disable widgets as appropriate to signed in state
    UpdateWidgetStates();
}

void WelcomeScene::OnKillGraphics() {
    UiScene::OnKillGraphics();
}

