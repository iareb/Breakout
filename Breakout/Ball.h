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
        Transform->SetScale(0.5f);

        Physics = AddComponent<PhysicsComponent>();
        Physics->SetGravity({ 0.f, 0.f });
        Physics->SetVelocity(
            Vec2{1.f, -2.f}.Normalize()
              * Config::Breakout::BALL_SPEED
              * Config::PIXELS_PER_METER
        );

        // The collision component comes after the
        // physics component to ensure it picks
        // up the movement from this frame
        Collision = AddComponent<CollisionComponent>();
        Collision->SetSize(
            1.2f * Config::PIXELS_PER_METER,
            1.2f * Config::PIXELS_PER_METER
        );

        Image = AddComponent<ImageComponent>(Config::BASE_PATH + "Assets/Grey.png");
        Image->SetWidth(1.2f * Config::PIXELS_PER_METER);
        Image->SetHeight(1.2f * Config::PIXELS_PER_METER);

        SetIsPaused(true);
    }

    void HandleEvent(const SDL_Event& E) override {
        if (
            E.type == SDL_EVENT_KEY_DOWN &&
            E.key.key == SDLK_SPACE &&
            GetScene().GetState() == GameState::InProgress
        ) {
            SetIsPaused(false);
        }
        else if (
            E.type == UserEvents::GAME_WON ||
            E.type == UserEvents::GAME_LOST
        ) {
            SetIsPaused(true);
        }
    }

    void HandleCollision(Entity &Other) override;

private:
    TransformComponent* Transform;
    PhysicsComponent* Physics;
    CollisionComponent* Collision;
    ImageComponent* Image;

    void SetIsPaused(bool isPaused) {
        Physics->SetIsEnabled(!isPaused);
        Collision->SetIsEnabled(!isPaused);
    }
};