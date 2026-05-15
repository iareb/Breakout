#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class Window {
public:
    Window() {
        SDLWindow = SDL_CreateWindow(
            "Scene",
            700, 300, 0
        );
    }

    ~Window() {
        if (SDLWindow && SDL_WasInit(SDL_INIT_VIDEO)) {
            SDL_DestroyWindow(SDLWindow);
        }
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Render() {
        const auto* Fmt = SDL_GetPixelFormatDetails(
            GetSurface()->format
        );

        SDL_FillSurfaceRect(
            GetSurface(), nullptr,
            SDL_MapRGB(Fmt, nullptr, 220, 220, 220));
    }

    void Update() {
        SDL_UpdateWindowSurface(SDLWindow);
    }

    SDL_Surface* GetSurface() {
        return SDL_GetWindowSurface(SDLWindow);
    }

private:
    SDL_Window* SDLWindow;
};