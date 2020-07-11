#pragma once

#include <SDL.h>

#include "StartData.h"
#include "IntPoint.h"

// ----------------------------------------------------------------------------
class GameObject
{
    public:
        GameObject( StartData*      inputStartData  = nullptr,
                    SDL_Texture*    inputTexture    = nullptr,
                    const IntPoint  size            = IntPoint( 0, 0 ),
                    const IntPoint  position        = IntPoint( 0, 0 ) );
        ~GameObject();

        IntPoint getSize() const;
        IntPoint getPosition() const;
        SDL_Rect* getRect() const;
        SDL_Texture* getTexture() const;


        void setPosition( const IntPoint position );
        
        // An instance is valid if it's texture is not nullptr.
        bool isValid();

    // ------------------------------------------------------------------------
    protected: 
        StartData* startData;
        SDL_Texture* texture;
        SDL_Rect* rect;
};
// ----------------------------------------------------------------------------