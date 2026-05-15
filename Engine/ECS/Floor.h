#pragma once
#include "Entity.h"
#include "Scene.h"

class Floor : public Entity {
public:
    Floor(Scene& Scene) : Entity{ Scene } {
        std::string BasePath{ SDL_GetBasePath() };
        AddComponent<TransformComponent>()->SetPosition({
          4.5f * Scene.PIXELS_PER_METER,
          1.f * Scene.PIXELS_PER_METER
            });
        AddComponent<ImageComponent>(BasePath + "assets/floor.png");
        AddComponent<CollisionComponent>()->SetSize(
            5.0f * Scene.PIXELS_PER_METER,
            2.0f * Scene.PIXELS_PER_METER
        );
    }
};