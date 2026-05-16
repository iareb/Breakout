#include <vector>
#include <iostream>
#include "Engine/Scene.h"
#include "Engine/ECS/CollisionComponent.h"
#include "Config.h"

Scene::Scene(Window& ParentWindow)
: ParentWindow{ ParentWindow }  {
#ifdef DRAW_DEBUG_HELPERS
    Trajectories = SDL_CreateSurface(
      Config::Engine::WINDOW_WIDTH,
      Config::Engine::WINDOW_HEIGHT,
      SDL_PIXELFORMAT_RGBA32
    );
#endif
}

float Scene::GetWidth() const {
    return static_cast<float>(
      Config::Engine::WINDOW_WIDTH
    );
}

float Scene::GetHeight() const {
    return static_cast<float>(
      Config::Engine::WINDOW_HEIGHT
    );
}

void Scene::HandleEvent(const SDL_Event& Event) {
    for (EntityPtr& E : Entities) {
        E->HandleEvent(Event);
    }
}

void Scene::Tick(float DeltaTime) {
    for (EntityPtr& E : Entities) {
        E->Tick(DeltaTime);
    }

    CheckCollisions();
}

void Scene::Render(SDL_Surface* Surface) {
    for (EntityPtr& E : Entities) {
        E->Render(Surface);
    }

#ifdef DRAW_DEBUG_HELPERS
    SDL_BlitSurface(
        Trajectories, nullptr, Surface, nullptr
    );
#endif
}

AssetManager& Scene::GetAssetManager() {
    return Assets;
}

Window& Scene::GetWindow() const
{
    return ParentWindow;
}

/**
* Basic O(n^2) check, inefficient!
* Need to use spatial partitioning.
*/
void Scene::CheckCollisions()
{
	for (size_t i = 0; i < Entities.size(); ++i) {
		CollisionComponent* ColA{
			Entities[i]->GetComponent<CollisionComponent>()
		};

		if (!ColA) continue;

        for (size_t j{ i + 1 }; j < Entities.size(); ++j) {
            CollisionComponent* ColB{
              Entities[j]->GetComponent<CollisionComponent>()
            };

            // Skip if no collision component
            if (!ColB) continue;

            if (ColA->IsCollidingWith(*ColB)) {
                Entities[i]->HandleCollision(*Entities[j]);
                Entities[j]->HandleCollision(*Entities[i]);
            }
        }
	}
}
