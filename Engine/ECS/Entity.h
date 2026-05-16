#pragma once
#include <memory>
#include <vector>
#include <ranges>
#include <SDL3/SDL.h>
#include "Component.h"
#include "Commands.h"
#include "TransformComponent.h"

class Scene;
using ComponentPtr = std::unique_ptr<Component>;
using ComponentPtrs = std::vector<ComponentPtr>;

// #define DRAW_DEBUG_HELPERS

class Entity {
public:
	Entity(Scene& Scene) : OwningScene{Scene} {}
	Scene& GetScene() const { return OwningScene; }

	virtual void HandleEvent(const SDL_Event& E) {
		for (ComponentPtr& C : Components) {
			C->HandleEvent(E);
		}
	}

	virtual void Tick(float DeltaTime) {
		for (ComponentPtr& C : Components) {
			C->Tick(DeltaTime);
		}
	}

	virtual void Render(SDL_Surface* Surface) {
		for (ComponentPtr& C : Components) {
			C->Render(Surface);
		}
		#ifdef DRAW_DEBUG_HELPERS
		for (ComponentPtr& C : Components) {
			C->DrawDebugHelpers(Surface);
		}
		#endif
	}

	virtual void HandleCommand(std::unique_ptr<Command> Cmd) {
		Cmd->Execute(this);
	}

	// Called when this entity collides with 'Other'
	virtual void HandleCollision(Entity& Other) {}

	virtual ~Entity() = default;

	/**
	* We use requires to ensure the ComponentType template argument 
	* that the external code supplied is either the Component type, 
	* or a type that derives from Component.
	* 
	* The ... syntax is used to define a function or template 
	* with an unknown number of parameters, called a variadic function. 
	* 
	* The double-ampersand && next to the Args type, and the use of the std::forward() function template, 
	* relates to a technique called perfect forwarding. 
	* This ensures each argument get forwarded from one function to the next 
	* without performance loss through unnecessary copying, and without losing characteristics such as const.
	*/
	template <typename CType, typename... CArgs>
	requires std::derived_from<CType, Component>
	CType* AddComponent(CArgs&&... ConstructorArgs) {
		// Construct the component in our vector
		ComponentPtr& NewComponent{
			Components.emplace_back(
				std::make_unique<CType>(
					this, std::forward<CArgs>(ConstructorArgs)...
				)
			)
		};

		NewComponent->Initialize();

		return static_cast<CType*>(NewComponent.get());
	}

	template <typename CType>
	requires std::derived_from<CType, Component>
	CType* GetComponent() {
		for (const ComponentPtr& C : Components) {
			// Try to cast the base Component pointer to a CType pointer
			if (auto Ptr{ dynamic_cast<CType*>(C.get()) }) {
				return Ptr;
			}
		}

		return nullptr;
	}

	template <typename CType>
	requires std::derived_from<CType, Component>
	std::vector<CType*> GetComponents() {
		std::vector<CType*> Results;
		for (const ComponentPtr& C : Components) {
			if (auto Ptr{ dynamic_cast<CType*>(C.get()) }) {
				Results.push_back(Ptr);
			}
		}
		return Results;
	}

	void RemoveComponent(Component* PtrToRemove) {
		// Iterate through the vector to find the component to remove
		for (size_t i = 0; i < Components.size(); ++i) {
			if (Components[i].get() == PtrToRemove) {
				// Components.begin() + i gives an iterator to the element.
				Components.erase(Components.begin() + i);
				return;
			}
		}

		// If the loop finishes, the component wasn't found.
		std::cout << "Warning: Attempted to remove "
			"a component not found on this entity.\n";
	}

private:
	ComponentPtrs Components;
	Scene& OwningScene;
};