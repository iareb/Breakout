#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <SDL3/SDL.h>
#include "Engine/ECS/Component.h"

class Command;
/**
* We use the Factory Pattern: function designed to create and return objects.
* These functions are capable of creating the commands.
* Our factory returns a std::unique_ptr<Command> instance, so
* whoever calls our factory assumes ownership of the command it created.
*/
using CommandPtr = std::unique_ptr<Command>;
using CommandFactory = std::function<CommandPtr()>;
using KeyToFactoryMap = std::unordered_map<SDL_Keycode, CommandFactory>;

class InputComponent : public Component {
public:
	using Component::Component;

	void Initialize() override;
	void Tick(float DeltaTime) override;
	void HandleEvent(const SDL_Event& E) override;

	void BindKeyDown(SDL_Keycode Key, CommandFactory Factory) {
		KeyDownBindings[Key] = Factory;
	}

	void BindKeyHeld(SDL_Keycode Key, CommandFactory Factory) {
		KeyHeldBindings[Key] = Factory;
	}

	void UnbindKey(SDL_Keycode Key) {
		KeyDownBindings.erase(Key);
		KeyHeldBindings.erase(Key);
	}

private:
	// Map for discrete key presses (events)
	KeyToFactoryMap KeyDownBindings;
	// Map for continuous key holds (polling)
	KeyToFactoryMap KeyHeldBindings;
};