#pragma once
#include <SDL3/SDL.h>
#include "Engine/Scene.h"
#include "Config.h"

class BreakoutScene : public Scene {
public:
    BreakoutScene(Window& ParentWindow)
    : Scene{ParentWindow} {
        Load(1);
    }

    void HandleEvent(const SDL_Event& E) override;
    
    void Render(SDL_Surface* Surface);

    void Load(int Level);

private:
    int LoadedLevel{ 1 };
    int BlocksRemaining{ 0 };

    void CompleteLevel() {
        if (LoadedLevel == 3) {
            SDL_Event WonEvent{};
            WonEvent.type = UserEvents::GAME_WON;
            SDL_PushEvent(&WonEvent);
        }
        else {
            Load(LoadedLevel + 1);
        }
    }
};