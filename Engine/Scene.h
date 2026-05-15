#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Engine/ECS/Entity.h"
#include "Window.h"
#include "AssetManager.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/InputComponent.h"
#include "Engine/ECS/CollisionComponent.h"

using EntityPtr = std::unique_ptr<Entity>;
using EntityPtrs = std::vector<EntityPtr>;

class Scene {
public:
	static inline int PIXELS_PER_METER{ 50 };
	static inline Vec2 GRAVITY = { 0, 9.8 * PIXELS_PER_METER };

	#ifdef DRAW_DEBUG_HELPERS
	SDL_Surface* Trajectories{
		SDL_CreateSurface(
			700, 300, SDL_PIXELFORMAT_RGBA32
		)
	};
	#endif

	Scene(Window& ParentWindow);

	void HandleEvent(const SDL_Event& Event);
	void Tick(float DeltaTime);
	void Render(SDL_Surface* Surface);

	AssetManager& GetAssetManager();
	Window& GetWindow() const;

protected:
	EntityPtrs Entities;
	Window& ParentWindow;
	AssetManager Assets;

private:
	void CheckCollisions();
};