#include <SDL3/SDL.h>
#include "Engine/Scene.h"

class BreakoutScene : public Scene {
    BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {}
};