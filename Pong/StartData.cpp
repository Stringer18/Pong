#include <SDL.h>
#include <stdio.h>
#include <Windows.h>


#include "StartData.h"
#include "IntPoint.h"
#include "MoveLine.h"

StartData::StartData()
{
    // ------------------------------------------------------------------------
    // Read size and place from file. Then init. Create window with renderer.
    // It's size for the background too, but about it - later.
    windowWidth = (int) GetPrivateProfileInt( "background",
            "windowWidth", 800, _APP_FILE_INI_ );                       // 800;
    windowHeight = (int) GetPrivateProfileInt( "background",
            "windowHeight", 600, _APP_FILE_INI_ );                      // 600;
    windowPositionX = (int) GetPrivateProfileInt( "background",
            "windowPositionX", 100, _APP_FILE_INI_ );                   // 100;
    windowPositionY = (int) GetPrivateProfileInt( "background",
            "windowPositionY", 100, _APP_FILE_INI_ );                   // 100;

    isSuccess = true;

    if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
    {
        printf( "SDL_Init Error: %s\n", SDL_GetError() );
        isSuccess = false;
    }
    else
    {
        gameWindow = SDL_CreateWindow( "Pong", windowPositionX,
                windowPositionY, windowWidth, windowHeight, SDL_WINDOW_SHOWN );
        if ( gameWindow == nullptr )
        {
            printf( "SDL_CreateWindow Error: %s\n", SDL_GetError() );
            isSuccess = false;
        }
        else
        {
            renderer = SDL_CreateRenderer( gameWindow, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if ( renderer == nullptr )
            {
                printf( "SDL_CreateRenderer Error: %s\n", SDL_GetError() );
                isSuccess = false;
            }
        }
    }

    if( isSuccess )
    {
        // --------------------------------------------------------------------
        // Counters
        counterWidth =  (int) GetPrivateProfileInt( "counters",
                "counterWidth", 50, _APP_FILE_INI_ );                   // 50;
        counterHeight =  (int) GetPrivateProfileInt( "counters",
                "counterHeight", 100, _APP_FILE_INI_ );                 // 100;
        counterPlayer01PositionX = windowWidth/4;
        counterPlayer02PositionX = windowWidth*3/4;
        counterPositionY = 20 + counterHeight/2;
        maxCounter =  (unsigned int) GetPrivateProfileInt( "counters",
                "maxCounter", 10, _APP_FILE_INI_ );                     // 10;
        if ( maxCounter < 0 ) { maxCounter = 0; }

        // --------------------------------------------------------------------
        // Players
        playerWidth = (int) GetPrivateProfileInt( "player",
                "playerWidth", 10, _APP_FILE_INI_ );                    // 10;
        playerHeight = (int) GetPrivateProfileInt( "player",
                "playerHeight", 50, _APP_FILE_INI_ );                   // 50;
        playerIndent = (int) GetPrivateProfileInt( "player",
                "playerIndent", 20, _APP_FILE_INI_ );                   // 20;
        player01PositionX = playerIndent + playerWidth/2;
        player02PositionX = windowWidth - playerIndent - playerWidth/2;
        playerStartPositionY = windowHeight/2;
        playerMoveRateStep = (int) GetPrivateProfileInt( "player",
                "playerMoveRateStep", 10, _APP_FILE_INI_ );             // 10;

        // --------------------------------------------------------------------
        // Ball
        char szBuff[_MAX_STR_BUFF_SIZE_];
        ballWidth = (int) GetPrivateProfileInt( "ball",
                "ballWidth", 10, _APP_FILE_INI_ );                      // 10;
        ballHeight = (int) GetPrivateProfileInt( "ball",
                "ballHeight", 10, _APP_FILE_INI_ );                     // 10;
        ballStartPositionX = windowWidth/2;
        ballStartPositionY = windowHeight/2;
        ballStartMove.startPoint = IntPoint( ballStartPositionX, 
                ballStartPositionY );
        GetPrivateProfileString( "ball", "ballStartK", "1.0", szBuff,
                _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
        ballStartMove.lineK = atof( szBuff );                           // 1.0;
        ballStartMove.lineB = ballStartPositionY -
                ballStartMove.lineK * ballStartPositionX;
        GetPrivateProfileString( "ball", "ballStartV", "1.0", szBuff,
                _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
        ballStartMove.lineV = atof( szBuff );                           // 1.0;
        ballStartMove.lineMoveOnX = (int) GetPrivateProfileInt( "ball",
                "ballStartMoveOnX", 1, _APP_FILE_INI_ );                // 1;
        
        // --------------------------------------------------------------------
        // Bot
        botMaxStep = (int) GetPrivateProfileInt( "bot",
                "botMaxStep", 10, _APP_FILE_INI_ );                     // 10;

        // --------------------------------------------------------------------
        // Textures
        isSuccess = prepareTextures();
        
    }
}



// ----------------------------------------------------------------------------
StartData::~StartData()
{
    if( ballTexture != nullptr) { SDL_DestroyTexture( ballTexture ); }
    if( player02Texture != nullptr) { SDL_DestroyTexture( player02Texture ); }
    if( player01Texture != nullptr) { SDL_DestroyTexture( player01Texture ); }
    for( unsigned int i = 0 ; i < maxCounter ; i++ )
    { 
        if( vecCounterTextures[i] != nullptr)
        {
            SDL_DestroyTexture( vecCounterTextures[i] );
        }

    }
    vecCounterTextures.clear();
    if( backgroundTexture != nullptr)
            { SDL_DestroyTexture( backgroundTexture ); }
    if( renderer != nullptr ) { SDL_DestroyRenderer( renderer ); }
	if( gameWindow != nullptr ) { SDL_DestroyWindow( gameWindow ); }	
    SDL_Quit();
}



// ----------------------------------------------------------------------------
bool StartData::prepareTextures()
{
    char szBuff[_MAX_STR_BUFF_SIZE_];

    // Background
    backgroundTexture = nullptr;
    GetPrivateProfileString( "textures", "strBackground",
            ".\\Resources\\background_800x600.bmp", szBuff,
            _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
    isSuccess = strBmpTextureToSdlTexture( szBuff, backgroundTexture );

    if ( !( isSuccess ) )
    {
        printf( "Failed to create background texture." );
        return isSuccess;
    }

    // ------------------------------------------------------------------------
    // Counters
    // Both counters use the same textures.
    for( unsigned int i = 0 ; i < maxCounter ; i++ )
    {
        char szBuffKey[_MAX_STR_BUFF_SIZE_];
        char szBuffVal[_MAX_STR_BUFF_SIZE_];
        sprintf_s( szBuffKey, _MAX_STR_BUFF_SIZE_,
                "strCounterTextures%02d", i );
        sprintf_s( szBuffVal, _MAX_STR_BUFF_SIZE_,
                ".\\Resources\\counter_50x100_%02d.bmp", i );
        GetPrivateProfileString( "textures", szBuffKey, szBuffVal,
                szBuff, _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );

        SDL_Texture* buffTexture = nullptr;
        if ( !( strBmpTextureToSdlTexture( szBuff, buffTexture ) ) )
        {
            printf( "Failed to create counter texture ¹%d.", i );
            isSuccess = false;
            break;
        }
        vecCounterTextures.push_back( buffTexture );
    }

    if ( !( isSuccess ) )
    {
        return isSuccess;
    }

    // ------------------------------------------------------------------------
    // First player
    player01Texture = nullptr;
    GetPrivateProfileString( "textures", "strPlayer01",
            ".\\Resources\\player01_10x50.bmp", szBuff,
            _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
    isSuccess = strBmpTextureToSdlTexture( szBuff, player01Texture );

    if ( !( isSuccess ) )
    {
        printf( "Failed to create first player texture." );
        return isSuccess;
    }

    // ------------------------------------------------------------------------
    // Second player
    player02Texture = nullptr;
    GetPrivateProfileString( "textures", "strPlayer02",
            ".\\Resources\\player02_10x50.bmp", szBuff,
            _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
    isSuccess = strBmpTextureToSdlTexture( szBuff, player02Texture );

    if ( !( isSuccess ) )
    {
        printf( "Failed to create second player texture." );
        return isSuccess;
    }

    // ------------------------------------------------------------------------
    // Ball
    ballTexture = nullptr;
    GetPrivateProfileString( "textures", "strBall",
            ".\\Resources\\ball_10x10.bmp", szBuff,
            _MAX_STR_BUFF_SIZE_, _APP_FILE_INI_ );
    isSuccess = strBmpTextureToSdlTexture( szBuff, ballTexture );

    if ( !( isSuccess ) )
    {
        printf( "Failed to create ball texture." );
    }

    return isSuccess;
}



// ----------------------------------------------------------------------------
bool StartData::strBmpTextureToSdlTexture( const char* strBmpTexture,
        SDL_Texture*& resultTexture )
{
    if ( resultTexture != nullptr )
    {
        printf( "Texture is not empty." );
        return false;
    }
    SDL_Surface* bufferSurfaceBmp = SDL_LoadBMP( strBmpTexture );
    if ( bufferSurfaceBmp == nullptr )
    {	
        printf( "SDL_LoadBMP Error: %s\n", SDL_GetError() );
        return false;
    }
    resultTexture = SDL_CreateTextureFromSurface( renderer, bufferSurfaceBmp );
    SDL_FreeSurface( bufferSurfaceBmp );
    return true;
}



// ----------------------------------------------------------------------------
// Getters

SDL_Renderer* StartData::getRenderer()
        { return renderer; }



IntPoint StartData::getWindowSize()
        { return IntPoint( windowWidth, windowHeight ); }

IntPoint StartData::getWindowPosition()
        { return IntPoint( windowPositionX, windowPositionY ); }

IntPoint StartData::getBackgroundPosition()
        { return IntPoint( windowWidth/2, windowHeight/2 ); }



IntPoint StartData::getCounterSize()
        { return IntPoint( counterWidth, counterHeight ); }

IntPoint StartData::getCounter01Position()
        { return IntPoint( counterPlayer01PositionX, counterPositionY ); }

IntPoint StartData::getCounter02Position()
        { return IntPoint( counterPlayer02PositionX, counterPositionY ); }

unsigned int StartData::getMaxCounter()
        { return maxCounter; }



IntPoint StartData::getPlayerSize()
        { return IntPoint( playerWidth, playerHeight ); }

IntPoint StartData::getPlayer01StartPosition()
        { return IntPoint( player01PositionX, playerStartPositionY ); }

IntPoint StartData::getPlayer02StartPosition()
        { return IntPoint( player02PositionX, playerStartPositionY ); }

int StartData::getPlayerMoveRateStep()
        { return playerMoveRateStep; }



IntPoint StartData::getBallSize()
        { return IntPoint( ballWidth, ballHeight ); }

IntPoint StartData::getBallStartPosition()
        { return IntPoint( ballStartPositionX, ballStartPositionY ); }

MoveLine StartData::getBallStartMove()
        { return ballStartMove; }



int StartData::getBotMaxStep()
        { return botMaxStep; }



SDL_Texture* StartData::getBackgroundTexture()
        { return backgroundTexture; }

SDL_Texture* StartData::getCounterTexture( 
        const unsigned int numCounter /*= 0*/ )
{
    return ( ( numCounter >= maxCounter ) ?
            nullptr: vecCounterTextures[numCounter] );
}

SDL_Texture* StartData::getPlayer01Texture()
        { return player01Texture; }

SDL_Texture* StartData::getPlayer02Texture()
        { return player02Texture; }

SDL_Texture* StartData::getBallTexture()
        { return ballTexture; }


// ----------------------------------------------------------------------------
