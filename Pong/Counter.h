#pragma once

#include <SDL.h>

#include "GameObject.h"
#include "StartData.h"
#include "IntPoint.h"

// ----------------------------------------------------------------------------
class Counter : public GameObject
{
    public:
        Counter(    StartData*      inputStartData  = nullptr,
                    SDL_Texture*    inputTexture    = nullptr,
                    const IntPoint  size            = IntPoint( 0, 0 ),
                    const IntPoint  position        = IntPoint( 0, 0 ) );

        bool getIsWin();

        bool nextNum();
        bool reset();

    // ------------------------------------------------------------------------
    protected:
        // The current texture number is stored here. This number corresponds
        // to the number of points scored by the player and the index under
        // which the desired texture is stored in the corresponding
        // variable (vecCounterTextures) of the class StartData.
        unsigned int numTexture;

        bool isWin;

};
// ----------------------------------------------------------------------------