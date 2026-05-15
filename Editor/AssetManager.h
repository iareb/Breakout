#pragma once
#include "Editor/Image.h"

namespace Editor {
struct AssetManager {
  Image BlueBlock{
    Config::BASE_PATH + "Assets/Brick_Blue_A.png"
  };
  Image GreenBlock{
    Config::BASE_PATH + "Assets/Brick_Green_A.png"
  };
  Image CyanBlock{
    Config::BASE_PATH + "Assets/Brick_Cyan_A.png"
  };
  Image OrangeBlock{
    Config::BASE_PATH + "Assets/Brick_Orange_A.png"
  };
  Image RedBlock{
    Config::BASE_PATH + "Assets/Brick_Red_A.png"
  };
  Image YellowBlock{
    Config::BASE_PATH + "Assets/Brick_Yellow_A.png"
  };
};
}