#pragma once
#include "Component.h"
#include "Vec2.h"

class PhysicsComponent : public Component {
public:
	using Component::Component;

	void Initialize() override;
	void Tick(float DeltaTime) override;

	Vec2 GetVelocity() { return Velocity; }
	void SetVelocity(const Vec2& NewVelocity) {
		Velocity = NewVelocity;
	}

	float GetMass() const { return Mass; }
	void SetMass(float NewMass) { Mass = NewMass;  }

	void ApplyForce(const Vec2& Force);
	void ApplyImpulse(const Vec2& Impulse);

	void DrawDebugHelpers(SDL_Surface* Surface) override;

private:
	Vec2 Velocity{ 0.0, 0.0 };
	Vec2 Acceleration{ 0.0, 0.0 };
	float Mass{ 1.0 };
};