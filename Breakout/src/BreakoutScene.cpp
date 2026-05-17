#include <iostream>
#include <format>
#include "Breakout/BreakoutScene.h"
#include "Breakout/Ball.h"
#include "Breakout/Wall.h"
#include "Breakout/Paddle.h"
#include "Breakout/Block.h"

void BreakoutScene::HandleEvent(const SDL_Event& E) {
    Scene::HandleEvent(E);
    using namespace UserEvents;
    if (E.type == BLOCK_DESTROYED) {
        --BlocksRemaining;
        if (BlocksRemaining == 0) {
            CompleteLevel();
        }
    }
#ifdef ENABLE_CHEATS
    else if (
        E.type == SDL_EVENT_KEY_DOWN &&
        E.key.key == SDLK_C
        ) {
        CompleteLevel();
    }
#endif
    else if (E.type == LAUNCH_LEVEL) {
        Load(E.user.code);
    }
    else if (E.type == GAME_WON) {
        SetState(GameState::Won);
    }
    else if (E.type == GAME_LOST) {
        SetState(GameState::Lost);
    }
    else if (
        E.type == SDL_EVENT_KEY_DOWN &&
        E.key.key == SDLK_R
        ) {
        SetState(GameState::InProgress);
        Load(1);
    }
}

void BreakoutScene::Load(int Level) {
    Entities.clear();
    Entities.emplace_back(
        std::make_unique<Ball>(*this)
    );

    using enum WallPosition;
    Entities.emplace_back(
        std::make_unique<Wall>(Top, *this)
    );
    Entities.emplace_back(
        std::make_unique<Wall>(Left, *this)
    );
    Entities.emplace_back(
        std::make_unique<Wall>(Bottom, *this)
    );
    Entities.emplace_back(
        std::make_unique<Wall>(Right, *this)
    );

    Entities.emplace_back(
        std::make_unique<Paddle>(*this)
    );

    // Deserializing levels
    std::string Filename{ Config::BASE_PATH + 
        std::format("Assets/Level{}.bin", Level) 
    };
    SDL_IOStream* Handle{
        SDL_IOFromFile(Filename.c_str(), "rb")
    };
    if (!Handle) {
        CheckSDLError("Loading Level");
        return;
    }

    Uint8 FileVersion{ 0 };
    SDL_ReadU8(Handle, &FileVersion);

    Uint8 GridWidth{ 0 };
    SDL_ReadU8(Handle, &GridWidth);

    Uint8 GridHeight{ 0 };
    SDL_ReadU8(Handle, &GridHeight);

    Uint32 BlockCount{ 0 };
    SDL_ReadU32LE(Handle, &BlockCount);

    std::cout << std::format(
        "Loading a version "
        "{} level ({}x{}) with {} blocks\n",
        FileVersion, GridWidth, GridHeight,
        BlockCount
    );

    for (size_t i{ 0 }; i < BlockCount; ++i) {
        Entities.emplace_back(
            std::make_unique<Block>(Handle, *this)
        );
    }

    SDL_CloseIO(Handle);

    BlocksRemaining = BlockCount;
    LoadedLevel = Level;
}

void BreakoutScene::Render(SDL_Surface* Surface) {
    const auto* Fmt{ SDL_GetPixelFormatDetails(
      Surface->format
    ) };

    if (GetState() == GameState::Won) {
        SDL_FillSurfaceRect(Surface, nullptr,
            SDL_MapRGB(Fmt, nullptr, 20, 50, 20)
        );
    }
    else if (GetState() == GameState::Lost) {
        SDL_FillSurfaceRect(Surface, nullptr,
            SDL_MapRGB(Fmt, nullptr, 50, 0, 0)
        );
    }
    Scene::Render(Surface);
}