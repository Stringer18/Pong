#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "IntPoint.h"
#include "MoveLine.h"

#define    _APP_FILE_INI_ ".\\Resources\\Pong.ini"
#define    _MAX_STR_BUFF_SIZE_ 1024

class StartData
{
    // ------------------------------------------------------------------------
    public:
        StartData();
        ~StartData();

        // --------------------------------------------------------------------
        // Getters
        SDL_Renderer* getRenderer();

        IntPoint getWindowSize();
        IntPoint getWindowPosition();
        IntPoint getBackgroundPosition();

        IntPoint getCounterSize();
        IntPoint getCounter01Position();
        IntPoint getCounter02Position();
        unsigned int getMaxCounter();

        IntPoint getPlayerSize();
        IntPoint getPlayer01StartPosition();
        IntPoint getPlayer02StartPosition();
        int getPlayerMoveRateStep();

        IntPoint getBallSize();
        IntPoint getBallStartPosition();
        MoveLine getBallStartMove();

        int getBotMaxStep();

        SDL_Texture* getBackgroundTexture();
        SDL_Texture* getCounterTexture( const unsigned int numCounter = 0 );
        SDL_Texture* getPlayer01Texture();
        SDL_Texture* getPlayer02Texture();
        SDL_Texture* getBallTexture();


        // --------------------------------------------------------------------
        // This flag is in case an error occurs while creating an object.
        // If it false, we correctly break all and destroy objects, which were
        // created earlier.
        bool isSuccess;

    // ------------------------------------------------------------------------
    protected:
        SDL_Window* gameWindow;
        SDL_Renderer* renderer;

        // --------------------------------------------------------------------
        // Constants for positioning objects and size (in pixels),
        // and other start settings.

        // Window for game and size of background.
        int windowWidth;
        int windowHeight;
        int windowPositionX;
        int windowPositionY;

        // Counters wins for first and second players.
        int counterWidth;
        int counterHeight;
        int counterPlayer01PositionX;
        int counterPlayer02PositionX;
        int counterPositionY;
            // Game is win when counter == maxCounter. This constant also
            // stores the number of textures in counterTexturesArray.
        unsigned int maxCounter;

        // Players
        int playerWidth;
        int playerHeight;
            //Indent from the edge of the window to the player.
        int playerIndent;
        int player01PositionX;
        int player02PositionX;
        int playerStartPositionY;
        int playerMoveRateStep;

        // Ball
        int ballWidth;
        int ballHeight;
        int ballStartPositionX;
        int ballStartPositionY;
            // Ball motion equation is y=kx+b.
            // For start will put k, then b=y0-k*x0.
            // If moveOnX == 1, then ball move from left to right.
            // If moveOnX == -1, then ball move from right to left.
        MoveLine ballStartMove;

        // Bot
        int botMaxStep;

        // --------------------------------------------------------------------
        // Textures will create from *.bmp
        // File paths are in StartData.cpp
        SDL_Texture* backgroundTexture;
        std::vector<SDL_Texture*> vecCounterTextures;
        SDL_Texture* player01Texture;
        SDL_Texture* player02Texture;
        SDL_Texture* ballTexture;

        // --------------------------------------------------------------------
        // This font is used for all letterings in the game.
        // Including and primarily for counters.
        TTF_Font *font;

    // ------------------------------------------------------------------------
    private:
        // prepareTextures is the part of the constructor that reads file paths
        // from _APP_FILE_INI_ and passes it to strBmpTextureToSdlTexture
        // to turn into a texture.
        bool prepareTextures();

        // Turns the path to the bmp-file (strBmpTexture) into texture
        // (resultTexture). It return true if everything is successful.
        // If you give a pointer to a non-nullptr texture,
        // then nothing will happen and the function will return false.
        bool strBmpTextureToSdlTexture( const char* strBmpTexture,
                SDL_Texture*& resultTexture );
};
// ----------------------------------------------------------------------------