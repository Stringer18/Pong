#pragma once

#include "IntPoint.h"

// ----------------------------------------------------------------------------
class MoveLine
{
    public:
        MoveLine(   const IntPoint  inputStartPoint = IntPoint( 0, 0 ),
                    const double    inputK = 0.0,
                    const double    inputB = 0.0,
                    const double    inputV = 0.0,
                    const int       inputM = 1 )
        { 
            startPoint = inputStartPoint;
            lineK = inputK;
            lineB = inputB;
            lineV = inputV;
            lineMoveOnX = inputM;
            moveNumStep = 0;
        }

        // The point from which we begin to move
        IntPoint startPoint;

        // Equation of movement on a straight
        // line given by the equation y = k * x + b.
        double lineK;       // k
        double lineB;       // b
        double lineV;       // speed of moving
        int lineMoveOnX;    // go to left ( == -1 ) or right ( == 1 )

        int moveNumStep;

};
// ----------------------------------------------------------------------------