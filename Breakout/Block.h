#pragma once
#include <format>
#include <iostream>
#include <unordered_map>
#include <SDL3/SDL.h>
#include "Engine/ECS/Entity.h"
#include "BreakoutScene.h"

class Block : public Entity {
public:
	Block(SDL_IOStream* Handle, BreakoutScene& Scene)
	: Entity{ Scene } 
	{
		Uint8 Type{ 0 };
		SDL_ReadU8(Handle, &Type);
		Uint8 GridRow{ 0 };
		SDL_ReadU8(Handle, &GridRow);
		Uint8 GridCol{ 0 };
		SDL_ReadU8(Handle, &GridCol);

		std::cout << std::format(
			"Loaded type {} block (Row={}, Col={})\n",
			Type, GridRow, GridCol
		);

		/**
		* Green blocks have additional data.
		* If we don't need to read that, we can advance (seek)
		* the read/write offset to the end of the object.
		* To do this, we need to calculate how many bytes 
		* we need to seek forward, and then call SDL_SeekIO().
		* 1. We call SDL_SeekIO() to advance past our 16-bit (2-byte) integer
		* 2. We then read the next 32-bit integer so we can understand how many elements are in our array
		* 3. We then call SDL_SeekIO() again to advance past the array. We know each element in the array
		*    is 32 bits (4 bytes), so we multiply that by the number of elements in the array
		*/
		if (
			static_cast<Config::ActorType>(Type) ==
			Config::ActorType::GreenBlock
		) {
			SDL_SeekIO(Handle, 2, SDL_IO_SEEK_CUR);
			Uint32 ArraySize{ 0 };
			SDL_ReadU32LE(Handle, &ArraySize);
			SDL_SeekIO(
				Handle, ArraySize * 4, SDL_IO_SEEK_CUR
			);
		}

		float Width{ 
			(Config::Engine::WINDOW_WIDTH * 0.8f) /
			 Config::Editor::GRID_WIDTH
		};
		float Height{ 25.5f };

		float totalWidth{ Config::Editor::GRID_WIDTH * Width };
		float totalHeight{ Config::Editor::GRID_HEIGHT * Height };
		float StartX{ (Config::Engine::WINDOW_WIDTH - totalWidth) / 2.0f };
		float StartY{ 40.0f };

		Transform = AddComponent<TransformComponent>();
		Transform->SetPosition({
			StartX + GridCol * Width,
			StartY + GridRow * Height
		});

		Collision = AddComponent<CollisionComponent>();
		Collision->SetSize(Width, Height);

		Image = AddComponent<ImageComponent>(
			Config::BASE_PATH + Images[static_cast<Config::ActorType>(Type)]
		);
		Image->SetWidth(Width);
		Image->SetHeight(Height);
	}

private:
	TransformComponent* Transform{ nullptr };
	CollisionComponent* Collision{ nullptr };
	ImageComponent* Image{ nullptr };

	using ImageMap = std::unordered_map<Config::ActorType, std::string>;	
	using enum Config::ActorType;
	inline static ImageMap Images{
		{ Actor,		  ""					       },
		{ BlueBlock,	  "Assets/Brick_Blue_A.png"    },
		{ GreenBlock,     "Assets/Brick_Green_A.png"   },
		{ CyanBlock,      "Assets/Brick_Cyan_A.png"    },
		{ OrangeBlock,    "Assets/Brick_Orange_A.png"  },
		{ RedBlock,       "Assets/Brick_Red_A.png"     },
		{ YellowBlock,    "Assets/Brick_Yellow_A.png"  },
	};
};