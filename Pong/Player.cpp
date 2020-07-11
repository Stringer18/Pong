#include "Player.h"

// ----------------------------------------------------------------------------
Player::Player( StartData*      inputStartData  /*= nullptr*/,
                SDL_Texture*    inputTexture    /*= nullptr*/,
                const IntPoint  size            /*= IntPoint( 0, 0 )*/,
                const IntPoint  position        /*= IntPoint( 0, 0 )*/,
                const int inputMoveRateStep         /*= 10*/ ) :
        GameObject( inputStartData, inputTexture, size, position )
{
    moveRateStep = inputMoveRateStep;
    moveNumStep = 0;

    minY = (rect->h)/2;
    maxY = startData->getWindowSize().y - minY;
}



// ----------------------------------------------------------------------------
void Player::moveAddNumStep( int addNum ) { moveNumStep += addNum; }



// ----------------------------------------------------------------------------
void Player::nextPosition()
{
    IntPoint position =
            getPosition().plusPoint( 0, ( moveNumStep * moveRateStep ) ) ;
    moveNumStep = 0;
    if( position.y < minY ) { position.y = minY; }
    if( position.y > maxY ) { position.y = maxY; }
    setPosition( position );
}



// ----------------------------------------------------------------------------
int Player::getMoveRateStep() const { return moveRateStep; }


// ----------------------------------------------------------------------------