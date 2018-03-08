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
#include "scene.hpp"

// These are all stubs. Subclasses should override to implement their
// specific functionality.

void Scene::OnInstall() {}
void Scene::DoFrame() {}
void Scene::OnUninstall() {}
void Scene::OnStartGraphics(int width, int height) {}
void Scene::OnKillGraphics() {}
void Scene::OnPointerDown(int pointerId, const struct PointerCoords* coords) {}
void Scene::OnPointerUp(int pointerId, const struct PointerCoords* coords) {}
void Scene::OnMouseButton(SystemAbstraction::MouseButton mouseButton, SystemAbstraction::ButtonEvent event, int x, int y) {}
void Scene::OnScroll(double yoffset) {}
void Scene::OnPointerMove(int pointerId, const struct PointerCoords* coords) {}
bool Scene::OnBackKeyPressed() { return false; }
void Scene::OnKey(SystemAbstraction::ButtonEvent event,SystemAbstraction:: Key key, SystemAbstraction::Mods mods, int x, int y){}
void Scene::OnChar(unsigned int codepoint){}
void Scene::OnJoy(float x, float y) {}
void Scene::OnScreenResized(int width, int height) {}
void Scene::OnPause() {}
void Scene::OnResume() {}
     Scene::~Scene() {}

