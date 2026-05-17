#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Engine/ECS/Entity.h"
#include "Window.h"
#include "AssetManager.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/InputComponent.h"

enum class GameState { InProgress, Won, Lost };

using EntityPtr = std::unique_ptr<Entity>;
using EntityPtrs = std::vector<EntityPtr>;

class Scene {
public:

	#ifdef DRAW_DEBUG_HELPERS
	SDL_Surface* Trajectories{
		SDL_CreateSurface(
			700, 300, SDL_PIXELFORMAT_RGBA32
		)
	};
	#endif

	Scene(Window& ParentWindow);

	float GetWidth() const;
	float GetHeight() const;

	virtual void HandleEvent(const SDL_Event& Event);
	void Tick(float DeltaTime);
	void Render(SDL_Surface* Surface);

	AssetManager& GetAssetManager();
	Window& GetWindow() const;

	GameState GetState() const { return State; }
	void SetState(GameState NewState) {
		State = NewState;
	}

protected:
	EntityPtrs Entities;
	Window& ParentWindow;
	AssetManager Assets;

private:
	void CheckCollisions();
	GameState State{ GameState::InProgress };
};