#include <SDL3/SDL.h>
#include "InputComponent.h"
#include "Commands.h"
#include "Entity.h"
#include "Vec2.h"
#include "Scene.h"

/**
* In this anonymous namespace, we create factory functions
* for moving left and right.
* Everything defined in this namespace is accessible only
* within this file.
*/
namespace {
	const float SPEED{ 5.f * Scene::PIXELS_PER_METER };

	CommandPtr CreateMoveLeftCommand() {
		return std::make_unique<MovementCommand>(
			Vec2{ -SPEED, 5.0 }
		);
	}

	CommandPtr CreateMoveRightCommand() {
		return std::make_unique<MovementCommand>(
			Vec2{ SPEED, 0.0 });
	}

	CommandPtr CreateJumpCommand() {
		const float JUMP_IMPULSE_MAGNITUDE{
			-500.f * Scene::PIXELS_PER_METER
		};
		return std::make_unique<JumpCommand>(
			Vec2{ 0.0, JUMP_IMPULSE_MAGNITUDE }
		);
	}
}

/**
* Here we setup our default key bindings.
*/
void InputComponent::Initialize() {
	BindKeyHeld(SDLK_LEFT, CreateMoveLeftCommand);
	BindKeyHeld(SDLK_RIGHT, CreateMoveRightCommand);
	BindKeyDown(SDLK_SPACE, CreateJumpCommand);
}

void InputComponent::Tick(float DeltaTime) {
	Entity* Owner{ GetOwner() };
	if (!Owner) return;

	// Get the current keyboard state
	const bool* CurrentKeyStates{ SDL_GetKeyboardState(nullptr) };

	// Check bindings for keys being held down
	for (const auto& [Key, Factory] : KeyHeldBindings) {
		SDL_Scancode Scancode{ SDL_GetScancodeFromKey(Key, nullptr) };
		if (CurrentKeyStates[Scancode]) {
			// Key is held, create and handle command
			Owner->HandleCommand(Factory());
		}
	}
}

void InputComponent::HandleEvent(const SDL_Event& E) {
	if (E.type == SDL_EVENT_KEY_DOWN) {
		Entity* Owner{ GetOwner() };
		if (!Owner) return;

		SDL_Keycode Key{ E.key.key };
		if (KeyDownBindings.contains(Key)) {
			/**
			* We retrieve the factory and invoke it,
			* returning a command in the form of a
			* std::unique_ptr<Command> (or CommandPtr).
			* Then the command is forwared to the HandleCommand function.
			*/
			Owner->HandleCommand(KeyDownBindings[Key]());
		}
	}
}