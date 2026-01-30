#include "stdafx.h"

#include "Game.hpp"
#include "box2d/box2d.h"


int main()
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    mmt_gd::Game game;
    game.run();

    return 0;
}
