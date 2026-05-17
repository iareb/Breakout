#include <iostream>
#include <SDL3_image/SDL_image.h> 
#include <algorithm>
#include "Engine/ECS/ImageComponent.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/AssetManager.h"

namespace {
	struct BlitInfo {
		SDL_Rect SourceRect;
		SDL_Rect DestRect;
	};

	BlitInfo CalculateBlitInfo(
		ScalingMode Mode,
		// Natural surface dimensions
		int SurfaceW, int SurfaceH,
		// Target top-left screen positions
		float TargetX, float TargetY,
		// Target rendering dimensions
		float TargetW, float TargetH
	) {
		BlitInfo Info{
			.SourceRect = { 0, 0, SurfaceW, SurfaceH }
		};

		if (Mode == ScalingMode::None) {
			// Render at natural image size
			// and at the target position
			Info.DestRect = Utilities::Round({
			  TargetX,
			  TargetY,
			  static_cast<float>(SurfaceW),
			  static_cast<float>(SurfaceH)
			});
			return Info;
		}
		
		if (Mode == ScalingMode::Fill) {
			// Stretch source to fill the exact
			// target dimensions
			Info.DestRect = Utilities::Round({
				TargetX, TargetY, TargetW, TargetH
			});
			return Info;
		}

		if (Mode == ScalingMode::Contain) {
			float Scale{ std::min(
				TargetW / SurfaceW,
				TargetH / SurfaceH
			)};

			Info.DestRect = Utilities::Round({
				TargetX, TargetY,
				SurfaceW * Scale,
				SurfaceH * Scale
			});

			return Info;
		}

		if (Mode == ScalingMode::Cover) {
			float Scale{ std::max(
			  TargetW / SurfaceW,
			  TargetH / SurfaceH
			)};

			if (Scale * SurfaceW > TargetW) {
				float ClipW{ TargetW / Scale };
				Info.SourceRect = Utilities::Round({
				  0, 0,
				  ClipW,
				  static_cast<float>(SurfaceH)
				});
			}
			else if (Scale * SurfaceH > TargetH) {
				float ClipH{ TargetH / Scale };
				Info.SourceRect = Utilities::Round({
				  0, 0,
				  static_cast<float>(SurfaceW),
				  ClipH
					});
			}

			Info.DestRect = Utilities::Round({
			  TargetX,
			  TargetY,
			  TargetW,
			  TargetH
			});
			
			return Info;
		}

		std::cerr << "Error: unknown scaling mode\n";
		return Info;
	}
}

ImageComponent::ImageComponent(Entity* Owner, const std::string& FilePath)
	: Component{ Owner }, ImageFilePath{ FilePath } 
{
	ImageSurface = GetAssetManager().LoadSurface(ImageFilePath);
}

void ImageComponent::Initialize() {
	Entity* Owner{ GetOwner() };
	if (!Owner->GetComponent<TransformComponent>()) {
		std::cout << "Error: ImageComponent "
			"requires TransformComponent on its Owner\n";

		// Request removal
		Owner->RemoveComponent(this);
	}
}

void ImageComponent::Render(SDL_Surface* Surface) {
	if (!GetIsEnabled()) return;

	if (!ImageSurface) return;

	auto [TargetX, TargetY] { GetOwnerPosition() + Offset };

	float TargetW{ GetWidth() * GetOwnerScale() };
	float TargetH{ GetHeight() * GetOwnerScale() };

	int SurfaceW{ GetSurfaceWidth() };
	int SurfaceH{ GetSurfaceHeight() };

	BlitInfo Info{
		CalculateBlitInfo(
			ScaleMode,
			SurfaceW, SurfaceH,
			TargetX, TargetY,
			TargetW, TargetH
	)};

	if (!SDL_BlitSurfaceScaled(
		ImageSurface.get(),
		&Info.SourceRect,
		Surface,
		&Info.DestRect,
		SDL_SCALEMODE_LINEAR
	)) {
		std::cerr << "Error: Blit failed: "
			<< SDL_GetError() << "\n";
	}
}

/**
* This method returns a boolean, to indicate whether loading
* the new image was successful.
* We store the new file path and use the asset manager to load the surface.
*/
bool ImageComponent::LoadNewImage(const std::string& NewPath) {
	ImageFilePath = NewPath;
	ImageSurface = GetAssetManager().LoadSurface(NewPath);
	return ImageSurface != nullptr;
}

void ImageComponent::SetWidth(float NewWidth) {
	Width = NewWidth;
}

void ImageComponent::SetHeight(float NewHeight) {
	Height = NewHeight;
}

void ImageComponent::ResetWidth() {
	Width = std::nullopt;
}

void ImageComponent::ResetHeight() {
	Height = std::nullopt;
}

int ImageComponent::GetSurfaceWidth() const {
	if (!ImageSurface) {
		std::cerr << "Warning: Attempted to get "
			"width from null ImageSurface.\n";
		return 0;
	}
	return ImageSurface->w;
}

int ImageComponent::GetSurfaceHeight() const {
	if (!ImageSurface) {
		std::cerr << "Warning: Attempted to get "
			"height from null ImageSurface.\n";
		return 0;
	}
	return ImageSurface->h;
}

// If Width has a value, return it.
// Otherwise, return surface width.
float ImageComponent::GetWidth() const {
	return Width.value_or(GetSurfaceWidth());
}

// If Height has a value, return it.
// Otherwise, return surface height.
float ImageComponent::GetHeight() const {
	return Height.value_or(GetSurfaceHeight());
}

void ImageComponent::SetScalingMode(ScalingMode Mode) {
	ScaleMode = Mode;
}

void ImageComponent::DrawDebugHelpers(SDL_Surface* Surface) {
	using Utilities::DrawRectOutline;

	if (!GetIsEnabled()) return;
	if (!ImageSurface) return;

	// Gather our position and dimensions for
	// the CalculateBlitInfo() function,
	// similar to what we did in Render()
	auto [TargetX, TargetY] {
		GetOwnerPosition() + Offset
	};
	float OwnerScale{ GetOwnerScale() };
	float TargetW{ GetWidth() * OwnerScale };
	float TargetH{ GetHeight() * OwnerScale };
	int SurfaceW{ GetSurfaceWidth() };
	int SurfaceH{ GetSurfaceHeight() };

	// 1. Draw Natural Bounds (Green Outline)
	SDL_Rect NaturalBounds{
		Utilities::Round({
			TargetX, TargetY,
			SurfaceW * OwnerScale,
			SurfaceH * OwnerScale
		}
	)};

	DrawRectOutline(
		Surface, NaturalBounds,
		SDL_MapRGB(
			SDL_GetPixelFormatDetails(Surface->format),
			nullptr, 0, 255, 0
		)
	);

	// 2. Draw Rendered Bounds (Red Outline)
	BlitInfo Info{ CalculateBlitInfo(
		ScaleMode,
		SurfaceW, SurfaceH,
		TargetX, TargetY,
		TargetW, TargetH
	)};

	DrawRectOutline(
		Surface, Info.DestRect,
		SDL_MapRGB(
			SDL_GetPixelFormatDetails(Surface->format),
			nullptr, 255, 0, 0
		)
	);

	SDL_Rect DebugRect{ Utilities::Round({TargetX - 5, TargetY - 5, 10, 10}) };

	SDL_FillSurfaceRect(
		Surface, &DebugRect,
		SDL_MapRGB(
			SDL_GetPixelFormatDetails(Surface->format),
			nullptr, 0, 0, 255
		)
	);
}