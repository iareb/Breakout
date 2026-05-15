#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Floor.h"

class Character : public Entity {
public:
	Character(Scene& Scene)
	: Entity{ Scene }
	{
		std::string BasePath{ SDL_GetBasePath() };

		AddComponent<TransformComponent>()->SetPosition({
			6.f * Scene.PIXELS_PER_METER,
			5.f * Scene.PIXELS_PER_METER
		});

		AddComponent<PhysicsComponent>()->SetMass(50.0);
		AddComponent<ImageComponent>(BasePath + "assets/player.png");
		AddComponent<CollisionComponent>()->SetSize(
			1.9f * Scene.PIXELS_PER_METER,
			1.7f * Scene.PIXELS_PER_METER
		);
	}

	void HandleCollision(Entity& Other) override {
		// Check if Character collided with a Floor
		Floor* FloorPtr{ dynamic_cast<Floor*>(&Other) };

		// It's not a floor, so we don't care
		if (!FloorPtr) return;

		CollisionComponent* CharacterCollider{
			GetComponent<CollisionComponent>()
		};

		CollisionComponent* FloorCollider{
			FloorPtr->GetComponent<CollisionComponent>()
		};

		SDL_FRect Intersection;
		CharacterCollider->GetCollisionRectangle(
			*FloorCollider, &Intersection
		);

		Vec2 CurrentPos{
			GetComponent<TransformComponent>()->GetPosition()
		};
		GetComponent<TransformComponent>()->SetPosition({
			CurrentPos.x,
			CurrentPos.y - Intersection.h
		});

		PhysicsComponent* Physics{ GetComponent<PhysicsComponent>() };
		if (Physics) {
			Vec2 CurrentVel{ Physics->GetVelocity() };
			// Stop vertical movement upon landing
			if (CurrentVel.y < 0) {
				Physics->SetVelocity({ CurrentVel.x, 0.0 });
			}
		}
	}
};