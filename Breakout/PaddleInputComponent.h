#pragma once
#include "Engine/ECS/InputComponent.h"
#include "Engine/Scene.h"
#include "Config.h"

using namespace Config::Breakout;

class PaddleInputComponent : public InputComponent {
public:
    // Inherit constructors
    using InputComponent::InputComponent;

    void Initialize() override {
        BindKeyHeld(SDLK_LEFT, [] {
            return std::make_unique<MovementCommand>(
                Vec2{
                  -PADDLE_SPEED * Config::PIXELS_PER_METER,
                  0.0
                });
            }
        );

        BindKeyHeld(SDLK_RIGHT, [] {
            return std::make_unique<MovementCommand>(
                Vec2{
                  PADDLE_SPEED * Config::PIXELS_PER_METER,
                  0.0
                });
            }
        );
    }
};