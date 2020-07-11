#include "Background.h"

// ----------------------------------------------------------------------------
Background::Background( StartData*      inputStartData  /*= nullptr*/,
                        SDL_Texture*    inputTexture    /*= nullptr*/,
                        const IntPoint  size            /*= IntPoint( 0, 0 )*/,
                        const IntPoint  position     /*= IntPoint( 0, 0 )*/ ) :
        GameObject( inputStartData, inputTexture, size, position ) {}



// ----------------------------------------------------------------------------