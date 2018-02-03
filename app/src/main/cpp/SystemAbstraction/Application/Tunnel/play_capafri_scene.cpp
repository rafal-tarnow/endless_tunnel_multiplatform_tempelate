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


PlayCapAfriScene::PlayCapAfriScene() : Scene() {

}

void PlayCapAfriScene::OnStartGraphics() {
    //TODO dorobić dynamiczna obsluge rozdzielczosci
    game = new Game(current_width, current_height);
    game->systemCallback_WindowResize(current_width, current_height);
}

void PlayCapAfriScene::OnKillGraphics() {
    delete game;
}

void PlayCapAfriScene::DoFrame() {
    game->systemCallback_Render();
    game->systemCallback_TimerTick();
}

void PlayCapAfriScene::OnPointerDown(int pointerId, const struct PointerCoords *coords) {
    if(coords->x > current_width/2)
    {
        game->systemCallback_keyboard(SystemAbstraction::EVENT_DOWN, 'd', coords->x, coords->y);
    }

    if(coords->x < current_width/2)
    {
        game->systemCallback_keyboard(SystemAbstraction::EVENT_DOWN, 'a', coords->x, coords->y);
    }
}

void PlayCapAfriScene::OnPointerUp(int pointerId, const struct PointerCoords *coords) {
    if(coords->x > current_width/2)
    {
        game->systemCallback_keyboard(SystemAbstraction::EVENT_UP, 'd', coords->x, coords->y);
    }

    if(coords->x < current_width/2)
    {
        game->systemCallback_keyboard(SystemAbstraction::EVENT_UP, 'a', coords->x, coords->y);
    }
}

void PlayCapAfriScene::OnPointerMove(int pointerId, const struct PointerCoords *coords) {

}

bool PlayCapAfriScene::OnBackKeyPressed() {

}

void PlayCapAfriScene::OnJoy(float joyX, float joyY) {

}

void PlayCapAfriScene::OnKeyDown(int keyCode) {

}

void PlayCapAfriScene::OnPause() {

}

void PlayCapAfriScene::OnScreenResized(int width, int height) {
    current_width = width;
    current_height = height;
    game->systemCallback_WindowResize(width, height);
}


