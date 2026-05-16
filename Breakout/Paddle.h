#pragma once
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/CollisionComponent.h"
#include "Breakout/BreakoutScene.h"
#include "Breakout/PaddleInputComponent.h"
#include "Config.h"

class Paddle : public Entity {
public:
	Paddle(BreakoutScene& Scene) : Entity{Scene} {
		Transform = AddComponent<TransformComponent>();
		Transform->SetPosition({
			5.0f * Config::PIXELS_PER_METER,
			5.5f * Config::PIXELS_PER_METER
		});

		Input = AddComponent<PaddleInputComponent>();

		float CollisionWidth{ 3.1f * Config::PIXELS_PER_METER };
		float CollisionOffsetX{ 1.f * Config::PIXELS_PER_METER };

		Physics = AddComponent<PhysicsComponent>();
		Physics->SetGravity({ 0.f, 0.f });
		Physics->ConstrainHorizontalMovement(
			-CollisionOffsetX,
			Scene.GetWidth() - (CollisionOffsetX + CollisionWidth)
		);

		Collision = AddComponent<CollisionComponent>();
		Collision->SetSize(
			CollisionWidth,
			0.6f * Config::PIXELS_PER_METER
		);
		Collision->SetOffset({
			CollisionOffsetX,
			0.f
		});

		AddComponent<ImageComponent>(Config::BASE_PATH + "Assets/Paddle_Frame_B.png");
	}
	
	void Tick(float DeltaTime) override {
		Entity::Tick(DeltaTime);
		Physics->SetVelocity({ 0.f, 0.f });
	}

private:
	TransformComponent* Transform{ nullptr };
	PaddleInputComponent* Input{ nullptr };
	PhysicsComponent* Physics{ nullptr };
	CollisionComponent* Collision{ nullptr };

	static CommandPtr CreateMoveLeftCommand() {
		using namespace Config::Breakout;
		return std::make_unique<MovementCommand>(
			Vec2{ -PADDLE_SPEED * Config::PIXELS_PER_METER, 0.f }
		);
	}

	static CommandPtr CreateMoveRightCommand() {
		using namespace Config::Breakout;
		return std::make_unique<MovementCommand>(
			Vec2{ PADDLE_SPEED * Config::PIXELS_PER_METER, 0.f }
		);
	}
};