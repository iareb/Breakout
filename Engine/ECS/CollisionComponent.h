#pragma once
#include <SDL3/SDL.h>
#include "Engine/ECS/Component.h"
#include "Engine/Vec2.h"

class CollisionComponent : public Component {
public:
	using Component::Component;

	void Initialize() override;
	
	// Called each frame to update Bounds
	void Tick(float DeltaTime) override;
	
	// Setters for defining the collision shape
	void SetOffset(const Vec2& NewOffset);
	
	Vec2 GetSize() const { return { Width, Height }; }
	void SetSize(float NewWidth, float NewHeight);

	// Returns the position at the center of the collision rectangle.
	Vec2 GetCenter() const {
		const auto& [x, y, w, h] {GetBounds()};
		return {
			x + w / 2,
			y + h / 2
		}
	}

	// Getter for the calculated bounds
	const SDL_FRect& GetBounds() const;

	// Check collision with another component
	bool IsCollidingWith(const CollisionComponent& Other) const;

	// Check collision and get intersection rectangle
	bool GetCollisionRectangle(
		const CollisionComponent& Other, SDL_FRect* OutIntersection
	) const;

	void DrawDebugHelpers(SDL_Surface* Surface) override;

private:
	// Shape definition relative to owner's origin
	Vec2 Offset{ 0.0, 0.0 };
	float Width{ 1.0 };
	float Height{ 1.0 };

	// Calculated bounds updated each tick
	SDL_FRect Bounds{ 0.0, 0.0, 0.0, 0.0 };
};