#pragma once

#include <SDL.h>
#include <math.h>

#include "GameObject.h"
#include "StartData.h"
#include "IntPoint.h"
#include "MoveLine.h"

#include "Player.h"

// ----------------------------------------------------------------------------
class Ball : public GameObject
{
    public:
        Ball(   StartData*      inputStartData  = nullptr,
                SDL_Texture*    inputTexture    = nullptr,
                const IntPoint  size            = IntPoint( 0, 0 ),
                const IntPoint  position        = IntPoint( 0, 0 ),
                const MoveLine  inputMoveLine   = MoveLine() );


        // --------------------------------------------------------------------
        void resetToStartPosition(  const IntPoint  position,
                                    const MoveLine  inputMoveLine );


        // --------------------------------------------------------------------
        // Calculate new position of ball. Than use testPosition for check and
        // correct it. Return result of function testPosition.
        int nextPosition(   const Player* const player01,
                            const Player* const player02 );


    // ------------------------------------------------------------------------
    protected:
        MoveLine moveLine;

        IntPoint minPoint;
        IntPoint maxPoint;

        // --------------------------------------------------------------------
        // Check and correct ( if it's necessary ) new position of ball,
        // bump with walls and players. 
        // Returns:
        //      0       don't bump left and right wall
        //      1       bump right wall (player01 win round)
        //      2       bump left wall (player02 win round)
        int testPosition(   IntPoint*           position, 
                            const Player* const player01,
                            const Player* const player02 );

        // --------------------------------------------------------------------
        // Change move line, when ball bump with upper or lower walls.
        void changeMoveLineOnY( const IntPoint* const position );

        // --------------------------------------------------------------------
        // Change move line, when ball bump with left or right players.
        void changeMoveLineOnX( const IntPoint* const position,
                                double correctionFactorForK );

        // --------------------------------------------------------------------
        // Change move line, when ball bump with player and wall
        // at the same time.
        void changeMoveLineReverse( const IntPoint* const position );

        // --------------------------------------------------------------------
        // Check bump ball with players. If bump occurs, then correct position.
        // Returns extension coefficient trajectory deviation (for lineK).
        // It's can be inclusively from -1.0 to 1.0 .
        // If function return -100.0 , it means that there was no bump.
        double bumpPlayer(  IntPoint*           position, 
                            const Player* const player01,
                            const Player* const player02 );

        // --------------------------------------------------------------------
        // Check that two segments intersect. Point of intersect will be in
        // segmentsIntersection. If function return false, than
        // segmentsIntersection will be without changes.
        // There is special case - p00.x == p01.x and ball cannot move strictly
        // vertically, so point of intersection of the lines will be required.
        // 1st segment (player) p00-p01
        // 2nd segment (ball)   p10-p11
        bool isSegmentsIntersection(    const IntPoint* const p00,
                                        const IntPoint* const p01,
                                        const IntPoint* const p10,
                                        const IntPoint* const p11,
                                        IntPoint* segmentsIntersection );
            
};
// ----------------------------------------------------------------------------