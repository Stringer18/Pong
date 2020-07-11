#pragma once

// ----------------------------------------------------------------------------
class IntPoint
{
    public:
        IntPoint( const int inputX = 0, const int inputY = 0 );

        IntPoint plusPoint ( const int plusX, const int plusY );


        int x;
        int y;
};
// ----------------------------------------------------------------------------