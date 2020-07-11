#include "IntPoint.h"

IntPoint::IntPoint( const int inputX /*= 0*/, const int inputY /*= 0*/ )
        { x = inputX; y = inputY; }

IntPoint IntPoint::plusPoint ( const int plusX, const int plusY )
        { return IntPoint( x + plusX, y + plusY ); }
