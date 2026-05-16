#pragma once
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/CollisionComponent.h"
#include "Breakout/BreakoutScene.h"
#include "Breakout/PaddleInputComponent.h"
#include "Breakout/Ball.h"
#include "Config.h"

class Paddle : public Entity {
public:
	Paddle(BreakoutScene& Scene) : Entity{Scene} {
		Transform = AddComponent<TransformComponent>();
		Transform->SetPosition({
			7.f * Config::PIXELS_PER_METER,
			10.f * Config::PIXELS_PER_METER
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

	/**
	* If HandleCollision() is called a second time, whilst the timer is still running,
	* the callback is still going to be called on its original schedule, meaning 
	* collisions will be reenabled again sooner than intended.
	*/
	void HandleCollision(Entity& Other) override {
		if (Ball* Ptr{ dynamic_cast<Ball*>(&Other) }) {
			HandleBallCollision(Ptr);
			Collision->SetIsEnabled(false);

			/**
			* Since HandleCollision() is a public function,
			* external code can call it at any time.
			*/
			if (TimerID != 0) {
				SDL_RemoveTimer(TimerID);
			}

			/**
			* Three arguments:
			* 1. The interval, in milliseconds
			* 2. A callback function to invoke at the end of the timer,
			* 3. A pointer to some data we want to pass to the callback.
			*    This argument is a void pointer (void*) so we can pass 
			*    a pointer to anything, or a nullptr if we don't need 
			*    any additional data.
			*/
			TimerID = SDL_AddTimer(
				500, &Paddle::EnableCollision, this
			);
		}
	}

	~Paddle() {
		if (TimerID) {
			SDL_RemoveTimer(TimerID);
		}
	}

	/**
	* Since we defined the destructor above, we need to address
	* the Rule of Three. 
	* We don't need paddles to be copyable, so we can delete 
	* the copy constructor and copy assignment operator.
	*/
	Paddle(const Paddle& Other) = delete;
	Paddle& operator=(const Paddle& Other) = delete;

private:
	TransformComponent* Transform{ nullptr };
	PaddleInputComponent* Input{ nullptr };
	PhysicsComponent* Physics{ nullptr };
	CollisionComponent* Collision{ nullptr };

	SDL_TimerID TimerID{ 0 };

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

	/**
	* To get the position of the ball relative to the paddle,
	* we subtract the ball's horizontal position from the
	* paddle's horizontal position.
	* 
	* The maximum possible value this will return is 
	* half of the combined width of the paddle and ball, 
	* whilst its minimum will be the negative form of that same distance.
	* 
	* To convert this to a value ranging from -1 to 1, we'd divide it by 
	* (PaddleWidth + BallWidth)/2.
	*/
	void HandleBallCollision(Ball* BallPtr) {
		Vec2 PaddlePos{ Collision->GetCenter() };
		float PaddleWidth{ Collision->GetSize().x };

		CollisionComponent* BallCollision{
			BallPtr->GetComponent<CollisionComponent>()
		};

		Vec2 BallPos{ BallCollision->GetCenter() };
		float BallWidth{ BallCollision->GetSize().x };

		// Where on the paddle the ball hit from
		// -1.0 (left edge) to 1.0 (right edge)
		float HitOffset{
			(BallPos.x - PaddlePos.x) / (
			  (PaddleWidth + BallWidth) / 2
			)
		};

		/**
		* Direction vector.
		* The horizontal direction is influenced by where 
		* the ball hit the paddle, so we set it to HitOffset
		* Also, we want the ball to move upwards after hitting
		* the paddle, so we set it to -1 (up is negative Y).
		*/
		Vec2 Direction{ HitOffset, -1.0f };

		// Now we update the ball's velocity.
		PhysicsComponent* BallPhysics{
			BallPtr->GetComponent<PhysicsComponent>()
		};
		BallPhysics->SetVelocity(
			Direction.Normalize() *
			Config::Breakout::BALL_SPEED *
			Config::PIXELS_PER_METER
		);
	}

	/**
	* Enable collisions after a brief pause.
	*/
	static Uint32 EnableCollision(void* Entity, SDL_TimerID, Uint32) {
		Paddle* Target{ static_cast<Paddle*>(Entity) };
		if (Target && Target->Collision) {
			Target->Collision->SetIsEnabled(true);
		}

		return 0;
	}
};