#include "Bot.h"

// ----------------------------------------------------------------------------
Bot::Bot(   int     inputBotMaxStep /*= 10*/,
            Ball*   inputBall       /*= nullptr*/,
            Player* inputPlayer     /*= nullptr*/ )
{
    //run = false;
    enableBot = false;
    botMaxStep = inputBotMaxStep;
    ball = inputBall;
    player = inputPlayer;
}


// ----------------------------------------------------------------------------
void Bot::onOffBot() { enableBot = !enableBot; }


// ----------------------------------------------------------------------------
bool Bot::isEnableBot() { return enableBot; }


// ----------------------------------------------------------------------------
void Bot::botStep()
{
    if( enableBot )
    {
        int playerNumStep = ( ball->getPosition().y -
                player->getPosition().y ) / player->getMoveRateStep();
        player->moveAddNumStep( botMaxStep < abs( playerNumStep ) ?
                ( botMaxStep * abs( playerNumStep ) / playerNumStep ) :
                playerNumStep );
    }
}


// ----------------------------------------------------------------------------