#include "ProcessingData.h"

// ----------------------------------------------------------------------------
ProcessingData::ProcessingData( StartData* inputStartData )
{
    startData = inputStartData;
    renderer = startData->getRenderer();

    background = new Background(    startData,
                                    startData->getBackgroundTexture(),
                                    startData->getWindowSize(),
                                    startData->getBackgroundPosition() );

    counterPlayer01 = new Counter(  startData,
                                    startData->getCounterTexture(),
                                    startData->getCounterSize(),
                                    startData->getCounter01Position() );

    counterPlayer02 = new Counter(  startData,
                                    startData->getCounterTexture(),
                                    startData->getCounterSize(),
                                    startData->getCounter02Position() );

    player01 = new Player(          startData,
                                    startData->getPlayer01Texture(),
                                    startData->getPlayerSize(),
                                    startData->getPlayer01StartPosition(),
                                    startData->getPlayerMoveRateStep() );

    player02 = new Player(          startData,
                                    startData->getPlayer02Texture(),
                                    startData->getPlayerSize(),
                                    startData->getPlayer02StartPosition(),
                                    startData->getPlayerMoveRateStep() );

    ball = new Ball(                startData,
                                    startData->getBallTexture(),
                                    startData->getBallSize(),
                                    startData->getBallStartPosition(),
                                    startData->getBallStartMove() );
    
    bot = new Bot(                  startData->getBotMaxStep(),
                                    ball, player02 );
    run = false;
}



// ----------------------------------------------------------------------------
ProcessingData::~ProcessingData()
{
    if( bot != nullptr )                { delete bot; }
    if( ball != nullptr )               { delete ball; }
    if( player02 != nullptr )           { delete player02; }
    if( player01 != nullptr )           { delete player01; }
    if( counterPlayer02 != nullptr )    { delete counterPlayer02; }
    if( counterPlayer01 != nullptr )    { delete counterPlayer01; }
    if( background != nullptr )         { delete background; }
}



// ----------------------------------------------------------------------------
void ProcessingData::keyControl()
{
    int step = 1;
    SDL_Event doEvent;
    run = true;
    while( run )
    {
        SDL_WaitEvent(&doEvent);
        switch(doEvent.type)
        {
            case SDL_QUIT:
            {
                run = false;
                return; // break;
            }
            case SDL_KEYDOWN:
            {
                switch(doEvent.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        run = false;
                        return; // break;
                    }
                    case SDLK_b:
                    {
                        bot->onOffBot();
                        break;
                    }
                    case SDLK_a:
                    {
                        player01->moveAddNumStep( -step );
                        break;
                    }
                    case SDLK_z:
                    {
                        player01->moveAddNumStep( step );
                        break;
                    }
                    case SDLK_k:
                    {
                        if( !(bot->isEnableBot()) )
                        { player02->moveAddNumStep( -step ); }
                        break;
                    }
                    case SDLK_m:
                    {
                        if( !(bot->isEnableBot()) )
                        { player02->moveAddNumStep( step ); }
                        break;
                    }
                }
                break;
            }
        }
    }
}



// ----------------------------------------------------------------------------
void ProcessingData::actionThread()
{
    run = true;
    bot->onOffBot();
    SDL_Delay( 1000 );
    while( run )
    {
        SDL_Delay( 50 );
        
        bot->botStep();

        player01->nextPosition();
        player02->nextPosition();
        
        int plusCounter = ball->nextPosition( player01, player02 );
        switch ( plusCounter )
        {
            case 1:
                counterPlayer01->nextNum();
                ball->resetToStartPosition(    
                        startData->getBallStartPosition(),
                        startData->getBallStartMove() );
                SDL_Delay( 1000 );
                break;
            case 2:
                counterPlayer02->nextNum();
                ball->resetToStartPosition(    
                        startData->getBallStartPosition(),
                        startData->getBallStartMove() );
                SDL_Delay( 1000 );
                break;
        }
        if( counterPlayer01->getIsWin() || counterPlayer02->getIsWin() )
        {
            refresh();
            SDL_Delay( 1000 );
            counterPlayer01->reset();
            counterPlayer02->reset();
        }

        refresh();
    }
}



// ----------------------------------------------------------------------------
void ProcessingData::refresh()
{
    SDL_RenderClear( renderer );
    SDL_RenderCopy( renderer,   background->getTexture(),
                    NULL,       background->getRect() );
    SDL_RenderCopy( renderer,   counterPlayer01->getTexture(),
                    NULL,       counterPlayer01->getRect() );
    SDL_RenderCopy( renderer,   counterPlayer02->getTexture(),
                    NULL,       counterPlayer02->getRect() );
    SDL_RenderCopy( renderer,   player01->getTexture(),
                    NULL,       player01->getRect() );
    SDL_RenderCopy( renderer,   player02->getTexture(),
                    NULL,       player02->getRect() );
    SDL_RenderCopy( renderer,   ball->getTexture(),
                    NULL,       ball->getRect() );
    SDL_RenderPresent( renderer );
}


// ----------------------------------------------------------------------------