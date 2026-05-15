#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Config.h"

int main(int argc, char** argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        CheckSDLError("SDL_Init");
        return 1;
    }

    if (!TTF_Init()) {
        CheckSDLError("TTF_Init");
        return 1;
    }

    Uint64 LastTick{ SDL_GetPerformanceCounter() };
    SDL_Event E;
    while (true) {
        while (SDL_PollEvent(&E)) {


        }

        Uint64 CurrentTick{ SDL_GetPerformanceCounter() };
        float DeltaTime{
          static_cast<float>(CurrentTick - LastTick) /
            static_cast<float>(SDL_GetPerformanceFrequency())
        };
        LastTick = CurrentTick;

    }

    return 0;
}