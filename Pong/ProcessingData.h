#pragma once

#include <stdio.h>
#include <SDL.h>

#include "StartData.h"
#include "Background.h"
#include "Counter.h"
#include "Player.h"
#include "Ball.h"
#include "Bot.h"

// ----------------------------------------------------------------------------
class ProcessingData
    {
    // ------------------------------------------------------------------------
    public:
        ProcessingData( StartData* inputStartData );
        ~ProcessingData();

        // --------------------------------------------------------------------
        // Read push keys and respond to it. Calculate pushes of move players.
        void keyControl();
        
        // --------------------------------------------------------------------
        // Calculates the new position of the game objects (players and ball)
        // and the state of the players point counters. Then call refresh.
        void actionThread();

        // --------------------------------------------------------------------
        // The function updates the picture.
        void refresh();

    // ------------------------------------------------------------------------
    protected:
        StartData* startData;
        
        SDL_Renderer* renderer;

        Background* background;
        Counter* counterPlayer01;
        Counter* counterPlayer02;
        Player* player01;
        Player* player02;
        Ball* ball;
        Bot* bot;

        bool run;
};
// ----------------------------------------------------------------------------