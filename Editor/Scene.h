#pragma once
#include <SDL3/SDL.h>
#include "Editor/ActorMenu.h"
#include "Editor/ActorTooltip.h"
#include "Editor/AssetManager.h"
#include "Editor/Level.h"
#include "Editor/Window.h"
#include "Editor/EditorButtons.h"

namespace Editor{
class Scene {
public:
    Scene(Window& ParentWindow)
    : ParentWindow{ParentWindow}
    {}

    void HandleEvent(const SDL_Event& E) {
        ActorShelf.HandleEvent(E);
        TooltipWindow.HandleEvent(E);
        CurrentLevel.HandleEvent(E);
        
        LoadButton1.HandleEvent(E);
        LoadButton2.HandleEvent(E);
        LoadButton3.HandleEvent(E);
        SaveButton.HandleEvent(E);
        PlayButton.HandleEvent(E);
    }
    
    void Tick(float DeltaTime) {
        ActorShelf.Tick(DeltaTime);
        TooltipWindow.Tick(DeltaTime);
        CurrentLevel.Tick(DeltaTime);
        
        LoadButton1.Tick(DeltaTime);
        LoadButton2.Tick(DeltaTime);
        LoadButton3.Tick(DeltaTime);
        SaveButton.Tick(DeltaTime);
        PlayButton.Tick(DeltaTime);
    }

    void Render(SDL_Surface* Surface) {
        ActorShelf.Render(Surface);
        TooltipWindow.Render();
        CurrentLevel.Render(Surface);
        
        LoadButton1.Render(Surface);
        LoadButton2.Render(Surface);
        LoadButton3.Render(Surface);
        SaveButton.Render(Surface);
        PlayButton.Render(Surface);
    }
    
    AssetManager& GetAssets() {
        return Assets;
    }
    
    bool HasMouseFocus() const {
        return ParentWindow.HasMouseFocus();
    }
    
    Window& GetWindow() const {
        return ParentWindow;
    }

    Level& GetLevel() {
        return CurrentLevel;
    }

private:
    ActorMenu ActorShelf{*this};
    Window& ParentWindow;
    ActorTooltip TooltipWindow{*this};
    Level CurrentLevel{*this};
    AssetManager Assets;
    
    LoadLevelButton LoadButton1{*this, 1};
    LoadLevelButton LoadButton2{*this, 2};
    LoadLevelButton LoadButton3{*this, 3};
    SaveLevelButton SaveButton{*this};
    PlayLevelButton PlayButton{*this};
};
}