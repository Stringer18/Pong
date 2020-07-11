#include "GameObject.h"

// ----------------------------------------------------------------------------
GameObject::GameObject( StartData*      inputStartData  /*= nullptr*/,
                        SDL_Texture*    inputTexture    /*= nullptr*/,
                        const IntPoint  size            /*= IntPoint( 0, 0 )*/,
                        const IntPoint  position        /*= IntPoint( 0, 0 )*/)

{
    startData = inputStartData;
    texture = inputTexture;
    rect = new SDL_Rect();
    rect->w = size.x;
    rect->h = size.y;
    setPosition( position );
}


// ----------------------------------------------------------------------------
GameObject::~GameObject()
{
    if( rect != nullptr ) { delete rect; }
}


// ----------------------------------------------------------------------------
IntPoint GameObject::getSize() const
{
    return IntPoint( rect->w, rect->h );
}


// ----------------------------------------------------------------------------
IntPoint GameObject::getPosition() const
{
    return IntPoint( rect->x + (rect->w)/2, rect->y + (rect->h)/2 );
}


// ----------------------------------------------------------------------------
SDL_Rect* GameObject::getRect() const { return rect; }


// ----------------------------------------------------------------------------
SDL_Texture* GameObject::getTexture() const { return texture; }



// ----------------------------------------------------------------------------
void GameObject::setPosition( const IntPoint position )
{
    rect->x = position.x - (rect->w)/2;
    rect->y = position.y - (rect->h)/2;
}



// ----------------------------------------------------------------------------
bool GameObject::isValid() { return ( texture != nullptr ); }



// ----------------------------------------------------------------------------