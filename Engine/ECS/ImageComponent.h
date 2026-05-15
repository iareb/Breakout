#pragma once
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <optional>
#include "Component.h"
#include "Vec2.h"

/**
* Four scaling modes for our images.
* 
* None: render the image at its natural size.
* 
* Fill: stretch the image to completely fill
* the target width and height.
* 
* Contain: scale the image as large as possible
* while preserving its aspect ratio, ensuring 
* the entire image fits within the target width and height.
* 
* Cover: scale the image as small as possible
* while preserving its aspect ratio, ensuring
* the image completely covers the target width and height.
* If the image has a different aspect ratio to the requested 
* Width / Height, parts of the image will be cropped.
*/
enum class ScalingMode {
	None, Fill, Contain, Cover
};

class ImageComponent : public Component {
public:
	using Component::Component;

	ImageComponent(Entity* Owner, const std::string& Path);

	void Initialize() override;
	void Render(SDL_Surface* Surface) override;
	void DrawDebugHelpers(SDL_Surface* Surface) override;
	bool LoadNewImage(const std::string& NewPath);

	int GetSurfaceWidth() const;
	int GetSurfaceHeight() const;
	void SetWidth(float NewWidth);
	void SetHeight(float NewHeight);
	void ResetWidth();
	void ResetHeight();
	float GetWidth() const;
	float GetHeight() const;

	void SetOffset(const Vec2& NewOffset) {
		Offset = NewOffset;
	}

	void SetScalingMode(ScalingMode Mode);

private:
	// Pointer to the loaded surface data
	std::shared_ptr<SDL_Surface> ImageSurface{ nullptr };
	// Store the path used to load this image
	std::string ImageFilePath;
	Vec2 Offset{ 0, 0 };
	std::optional<float> Width{ std::nullopt };
	std::optional<float> Height{ std::nullopt };
	ScalingMode ScaleMode{ ScalingMode::Fill };
};