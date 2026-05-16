#include "Breakout/BreakoutScene.h"
#include "Breakout/Ball.h"
#include "Breakout/Wall.h"

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
}
