#include "Breakout/BreakoutScene.h"
#include "Breakout/Ball.h"

void BreakoutScene::Load(int Level) {
    Entities.clear();
    Entities.emplace_back(
        std::make_unique<Ball>(*this)
    );
}
