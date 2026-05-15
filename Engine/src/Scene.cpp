#include <vector>
#include <iostream>
#include "Scene.h"
#include "Character.h"
#include "Floor.h"

Scene::Scene(Window& ParentWindow)
: ParentWindow{ ParentWindow }  {
    Entities.emplace_back(
        std::make_unique<Character>(*this)
    );

    Entities.emplace_back(
        std::make_unique<Floor>(*this)
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
