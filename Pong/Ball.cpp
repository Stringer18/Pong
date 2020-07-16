#include "Ball.h"

// ----------------------------------------------------------------------------
Ball::Ball(     StartData*      inputStartData  /*= nullptr*/,
                SDL_Texture*    inputTexture    /*= nullptr*/,
                const IntPoint  size            /*= IntPoint( 0, 0 )*/,
                const IntPoint  position        /*= IntPoint( 0, 0 )*/,
                const MoveLine  inputMoveLine   /*= MoveLine()*/ ) :
        GameObject( inputStartData, inputTexture, size, position )
{
    minPoint = IntPoint( (rect->w)/2, (rect->h)/2 );
    maxPoint = IntPoint(    startData->getWindowSize().x - minPoint.x,
                            startData->getWindowSize().y - minPoint.y );

    moveLine = inputMoveLine;
}


// ----------------------------------------------------------------------------
void Ball::resetToStartPosition(    const IntPoint  position,
                                    const MoveLine  inputMoveLine )
{
    GameObject::setPosition( position );
    moveLine = inputMoveLine;
}


// ----------------------------------------------------------------------------
int Ball::nextPosition( const Player* const player01,
                        const Player* const player02 )
{
    moveLine.moveNumStep++;
    
    // A * x^2 - 2 * B * x + C = 0
    //
    // A = k^2 + 1
    // B = x0 - k * ( b - y0 )
    // C = x0^2 + ( b - y0 )^2 - ( moveNumStep * lineV )^2
    //
    // x = ( B +- sqrt( B^2 - A * C ) ) / A
    // y = k * x + b
    double A = pow( moveLine.lineK, 2 ) + 1;
    double B = moveLine.startPoint.x - 
            moveLine.lineK * ( moveLine.lineB - moveLine.startPoint.y );
    double C = pow( (double) moveLine.startPoint.x, 2 ) + 
            pow( (double) ( moveLine.lineB - moveLine.startPoint.y ), 2 ) -
            pow( moveLine.moveNumStep * moveLine.lineV, 2);
    double x = ( B + moveLine.lineMoveOnX * sqrt( B  *B - A * C ) ) / A;
    double y = moveLine.lineK * x + moveLine.lineB;

    IntPoint position = IntPoint( (int) x, (int) y ) ;
    int plusCounter = testPosition( &position, player01, player02 );
    setPosition( position );
    return plusCounter;
}


// ----------------------------------------------------------------------------
int Ball::testPosition( IntPoint*           position,
                        const Player* const player01,
                        const Player* const player02 )
{
    // At first we do all the checks and after it - all changes.
    // changeFlag = 0   no changes
    // changeFlag = 1   change on Y
    // changeFlag = 2   change on X
    // changeFlag = 3   reverse movement
    int changeFlag = 0;

    // Check bump with upper and lower walls. 
    if( position->y <= minPoint.y )
    {
        position->y = minPoint.y;
        position->x = (int) ( ( position->y -
                moveLine.lineB ) / moveLine.lineK );
        changeFlag += 1;
    }
    if( position->y >= maxPoint.y )
    {
        position->y = maxPoint.y;
        position->x = (int) ( ( position->y -
                moveLine.lineB ) / moveLine.lineK );
        changeFlag += 1;
    }
    
    // Check bump with players.
    double correctionFactorForK = bumpPlayer( position, player01, player02 );
    if( correctionFactorForK != -100.0 )
    {
        changeFlag += 2;
    }

    // Check bump with left and right walls.
    if( position->x <= minPoint.x )
    {
        position->x = minPoint.x;
        position->y = (int) ( moveLine.lineK * position->x + moveLine.lineB );
        return 2;   // Player02 win round.
    }
    if( position->x >= maxPoint.x )
    {
        position->x = maxPoint.x;
        position->y = (int) ( moveLine.lineK * position->x + moveLine.lineB );
        return 1;   // Player01 win round.
    }

    // Change trajectory, if it's necessary.
    switch ( changeFlag )
    {
        case 1:
            changeMoveLineOnY( position );
            break;
        case 2:
            changeMoveLineOnX( position, correctionFactorForK );
            break;
        case 3:
            changeMoveLineReverse( position );
            break;
    }
    return 0;
}


// ----------------------------------------------------------------------------
void Ball::changeMoveLineOnY( const IntPoint* const position )
{
    if ( position->x == moveLine.startPoint.x )
    {
        moveLine.lineMoveOnX *= -1;
    }
    else
    {
        moveLine.lineK = ( (double) moveLine.startPoint.y - position->y ) / 
                ( position->x - moveLine.startPoint.x );
        moveLine.lineB = position->y - moveLine.lineK * position->x;
        moveLine.startPoint.x = position->x;
    }
    moveLine.startPoint.y = position->y;
    moveLine.moveNumStep = 0;
}


// ----------------------------------------------------------------------------
void Ball::changeMoveLineOnX(   const IntPoint* const position,
                                double correctionFactorForK )
{
    if ( position->x != moveLine.startPoint.x )
    {
        moveLine.lineK = ( (double) moveLine.startPoint.y - position->y ) / 
                ( position->x - moveLine.startPoint.x ) + correctionFactorForK;
        moveLine.lineB = position->y - moveLine.lineK * position->x;
        moveLine.startPoint.x = position->x;
    }
    moveLine.startPoint.y = position->y;
    moveLine.lineMoveOnX *= -1;
    moveLine.moveNumStep = 0;
    moveLine.lineV += 0.1;
}


// ----------------------------------------------------------------------------
void Ball::changeMoveLineReverse( const IntPoint* const position )
{
    moveLine.startPoint.x = position->x;
    moveLine.startPoint.y = position->y;
    moveLine.lineMoveOnX *= -1;
    moveLine.moveNumStep = 0;
    moveLine.lineV += 0.1;
}


// ----------------------------------------------------------------------------
double Ball::bumpPlayer(    IntPoint*           position, 
                            const Player* const player01,
                            const Player* const player02 )
{
    IntPoint ballPoint = getPosition();
    
    IntPoint playerPoint = player01->getPosition();
    IntPoint playerSize = player01->getSize();
    IntPoint playerP00 = IntPoint( playerPoint.x + ( playerSize.x +
            rect->w ) / 2 , playerPoint.y - ( playerSize.y + rect->h ) / 2 );
    IntPoint playerP01 = IntPoint( playerPoint.x + ( playerSize.x +
            rect->w ) / 2 , playerPoint.y + ( playerSize.y + rect->h ) / 2 );

    // Points playerP00 and playerP01 are ends of player segment.
    // ballPoint and position - ball segment.
    if( isSegmentsIntersection( &playerP00, &playerP01, &ballPoint, position,
            position ) )
    {
        return ( ( (double) playerPoint.y - position->y ) /
                ( playerPoint.y - playerP01.y ) );
    }

    playerPoint = player02->getPosition();
    playerSize = player02->getSize();
    playerP00 = IntPoint( playerPoint.x - ( playerSize.x +
            rect->w ) / 2 , playerPoint.y - ( playerSize.y + rect->h ) / 2 );
    playerP01 = IntPoint( playerPoint.x - ( playerSize.x +
            rect->w ) / 2 , playerPoint.y + ( playerSize.y + rect->h ) / 2 );

    if( isSegmentsIntersection( &playerP00, &playerP01, &ballPoint, position,
            position ) )
    {
        return ( ( (double) playerPoint.y - position->y ) /
                ( playerPoint.y - playerP01.y ) );
    }

    return -100.0;
}


// ----------------------------------------------------------------------------
bool Ball::isSegmentsIntersection(  const IntPoint* const p00,
                                    const IntPoint* const p01,
                                    const IntPoint* const p10,
                                    const IntPoint* const p11,
                                    IntPoint* segmentsIntersection )
{
    IntPoint result = IntPoint();
    
    result.x = p01->x;
    result.y = (int) ( moveLine.lineK * result.x + moveLine.lineB );

    if( p00->y < p01->y )
    {
        if( ( result.y < p00->y ) || ( result.y > p01->y ) ) { return false; }
    }
    else
    {
        if( ( result.y < p01->y ) || ( result.y > p00->y ) ) { return false; }
    }
    if( p10->y < p11->y )
    {
        if( ( result.y < p10->y ) || ( result.y > p11->y ) ) { return false; }
    }
    else
    {
        if( ( result.y < p11->y ) || ( result.y > p10->y ) ) { return false; }
    }
    if( p10->x < p11->x )
    {
        if( ( result.x <= p10->x ) || ( result.x > p11->x ) ) { return false; }
    }
    else
    {
        if( ( result.x < p11->x ) || ( result.x >= p10->x ) ) { return false; }
    }

    segmentsIntersection->x = result.x;
    segmentsIntersection->y = result.y;
    return true;
}


// ----------------------------------------------------------------------------