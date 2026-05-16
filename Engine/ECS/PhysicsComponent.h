#pragma once
#include "Engine/Scene.h"
#include "Engine/ECS/Component.h"
#include "Engine/Vec2.h"

class PhysicsComponent : public Component {
public:
	using Component::Component;

	void Initialize() override;
	void Tick(float DeltaTime) override;

	void ConstrainHorizontalMovement(float Left, float Right) {
		ShouldConstrainHorizontalMovement = true;
		ConstrainLeft = Left;
		ConstrainRight = Right;
	}

	Vec2 GetVelocity() const { return Velocity; }
	void SetVelocity(const Vec2& NewVelocity) {
		Velocity = NewVelocity;
	}

	float GetMass() const { return Mass; }
	void SetMass(float NewMass) { Mass = NewMass;  }

	Vec2 GetGravity() const { return Gravity; }
	void SetGravity(Vec2 NewGravity) {
		Gravity = NewGravity;
	}

	void ApplyForce(const Vec2& Force);
	void ApplyImpulse(const Vec2& Impulse);

	void DrawDebugHelpers(SDL_Surface* Surface) override;

private:
	Vec2 Velocity{ 0.0f, 0.0f };
	Vec2 Acceleration{ 0.0f, 0.0f };
	float Mass{ 1.0f };
	Vec2 Gravity{ Config::Breakout::GRAVITY };
	bool ShouldConstrainHorizontalMovement{ false };
	float ConstrainLeft{ 0.0f };
	float ConstrainRight{ 0.0f };
};