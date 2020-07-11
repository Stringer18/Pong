#pragma once

#include <SDL.h>

#include "GameObject.h"
#include "StartData.h"
#include "IntPoint.h"

// ----------------------------------------------------------------------------
class Player : public GameObject
{
    public:
        Player( StartData*      inputStartData  = nullptr,
                SDL_Texture*    inputTexture    = nullptr,
                const IntPoint  size            = IntPoint( 0, 0 ),
                const IntPoint  position        = IntPoint( 0, 0 ),
                const int inputMoveRateStep = 10 );


        // --------------------------------------------------------------------
        // Moving.

        // Adds the number of steps the player will take on the next render.
        void moveAddNumStep( int addNum );

        // Shifts the player a step in size moveNumStep * moveRateStep .
        // But only to the extent possible.
        void nextPosition();

        int getMoveRateStep() const;

    // ------------------------------------------------------------------------
    protected:
        int moveRateStep;
        int moveNumStep;

        int minY;
        int maxY;
};
// ----------------------------------------------------------------------------