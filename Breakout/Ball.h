#pragma once
#include "Config.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/CollisionComponent.h"
#include "Breakout/BreakoutScene.h"

class Ball : public Entity {
public:
    Ball(BreakoutScene& Scene) : Entity(Scene) {
        Transform = AddComponent<TransformComponent>();
        Transform->SetPosition({
            Config::Engine::WINDOW_WIDTH / 2.f,
            Config::Engine::WINDOW_HEIGHT * 0.5f
        });
        // Transform->SetScale(0.3f);

        Collision = AddComponent<CollisionComponent>();
        Collision->SetSize(
            1.2f * Scene::PIXELS_PER_METER,
            1.2f * Scene::PIXELS_PER_METER
        );

        Image = AddComponent<ImageComponent>(Config::BASE_PATH + "Assets/Grey.png");
        Image->SetWidth(1.2f * Scene::PIXELS_PER_METER);
        Image->SetHeight(1.2f * Scene::PIXELS_PER_METER);
    }

private:
    TransformComponent* Transform;
    CollisionComponent* Collision;
    ImageComponent* Image;
};