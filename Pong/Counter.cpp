#include "Counter.h"

// ----------------------------------------------------------------------------
Counter::Counter(   StartData*      inputStartData  /*= nullptr*/,
                    SDL_Texture*    inputTexture    /*= nullptr*/,
                    const IntPoint  size            /*= IntPoint( 0, 0 )*/,
                    const IntPoint  position        /*= IntPoint( 0, 0 )*/ ) :
        GameObject( inputStartData, inputTexture, size, position )
{
    numTexture = 0;
    isWin = false;
}



// ----------------------------------------------------------------------------
bool Counter::getIsWin() { return isWin; }



// ----------------------------------------------------------------------------
bool Counter::nextNum()
{
    numTexture++;
    if( numTexture >= startData->getMaxCounter() )
    {
        isWin = true;
    }
    else
    {
        texture = startData->getCounterTexture( numTexture );
    }
    return isValid();
}



// ----------------------------------------------------------------------------
bool Counter::reset()
{
    numTexture = 0;
    isWin = false;
    texture = startData->getCounterTexture( numTexture );
    return isValid();
}



// ----------------------------------------------------------------------------