#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene.h"

void PhysicsComponent::Initialize() {
	// Physics needs a Transform to know where the entity is
	if (!GetOwner()->GetComponent<TransformComponent>()) {
		std::cerr << "Error: PhysicsComponent "
			"requires TransformComponent on its Owner.\n";

		// Request self-removal
		GetOwner()->RemoveComponent(this);
	}
}

void PhysicsComponent::ApplyForce(const Vec2& Force) {
	if (Mass > 0.0f) {
		Acceleration += Force / Mass;
	}
}

void PhysicsComponent::ApplyImpulse(const Vec2& Impulse) {
	if (Mass > 0.0f) {
		Velocity += Impulse / Mass;
	}
}

void PhysicsComponent::Tick(float DeltaTime) {
	// 1. Apply persisent forces like gravity
	ApplyForce(Gravity * Mass);

	// 2. Update velocity based on acceleration
	Velocity += Acceleration * DeltaTime;

	// 3. Update position based on velocity
	SetOwnerPosition(GetOwnerPosition() + Velocity * DeltaTime);

	// 4. Reset acceleration for the next frame.
	Acceleration = { 0.0, 0.0 };

	if (ShouldConstrainHorizontalMovement) {
		auto [x, y] {GetOwnerPosition()};
		if (x < ConstrainLeft) {
			SetOwnerPosition({ ConstrainLeft, y });
		}
		else if (x > ConstrainRight) {
			SetOwnerPosition({ ConstrainRight, y });
		}
	}
}

void PhysicsComponent::DrawDebugHelpers(SDL_Surface* Surface) {
#ifdef DRAW_DEBUG_HELPERS
	auto [x, y] {GetOwnerPosition()};
	SDL_Rect PositionIndicator{
	  int(x) - 2, int(y) - 2, 4, 4
	};
	SDL_FillSurfaceRect(
		GetScene().Trajectories,
		&PositionIndicator,
		SDL_MapRGB(
			SDL_GetPixelFormatDetails(
				GetScene().Trajectories->format
			),
			nullptr, 0, 0, 255
		)
	);
#endif
}