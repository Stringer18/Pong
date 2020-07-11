#pragma once

#include <SDL.h>

#include "GameObject.h"
#include "StartData.h"
#include "IntPoint.h"

// ----------------------------------------------------------------------------
class Background : public GameObject
{
    public:
        Background( StartData*      inputStartData  = nullptr,
                    SDL_Texture*    inputTexture    = nullptr,
                    const IntPoint  size            = IntPoint( 0, 0 ),
                    const IntPoint  position        = IntPoint( 0, 0 ) );
};
// ----------------------------------------------------------------------------