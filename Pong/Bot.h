#pragma once

#include <math.h>

#include "Ball.h"
#include "Player.h"
#include "IntPoint.h"

// ----------------------------------------------------------------------------
class Bot
{
    // ------------------------------------------------------------------------
    public:
        Bot(    int     inputBotMaxStep  = 10,
                Ball*   inputBall           = nullptr,
                Player* inputPlayer         = nullptr );

        // --------------------------------------------------------------------
        // Activates or deactivates the bot, depending on its current state.
        void onOffBot();

        // --------------------------------------------------------------------
        bool isEnableBot();

        // --------------------------------------------------------------------
        // Control player02, while bot is enable.
        void botStep();

    // ------------------------------------------------------------------------
    protected:
        //bool run;
        bool enableBot;
        int botMaxStep;
        Ball* ball;
        Player* player;
};
// ----------------------------------------------------------------------------