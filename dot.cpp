/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "dot.h"
#include "constants.h"

extern SDL_Renderer* gRenderer;


double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

bool checkCollision( Circle& a, Circle& b )
{
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool checkCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a.x < b.x )
    {
        cX = b.x;
    }
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

Dot::Dot(int x, int y, int w, int h) : vel(4), width(w), height(h)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Set collision circle size
    collider.r = width / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Move collider relative to the circle
    shiftColliders();
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
        case SDLK_w: mVelY -= vel; break;
        case SDLK_s: mVelY += vel; break;
        case SDLK_a: mVelX -= vel; break;
        case SDLK_d: mVelX += vel; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
        case SDLK_w: mVelY += vel; break;
        case SDLK_s: mVelY -= vel; break;
        case SDLK_a: mVelX += vel; break;
        case SDLK_d: mVelX -= vel; break;
        }
    }
}

void Dot::move( SDL_Rect& square, Circle& circle )
{
    //Move the dot left or right
    mPosX += mVelX;
    shiftColliders();

    if ( mPosX + collider.r > SCREEN_WIDTH ) {
        mPosX = SCREEN_WIDTH - collider.r;
        shiftColliders();        
    }
    if( ( mPosX - collider.r < 0 ) || checkCollision( collider, square ) || checkCollision( collider, circle ) ) {
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
    shiftColliders();
    if ( mPosY + collider.r > SCREEN_HEIGHT ) {
        mPosY = SCREEN_HEIGHT - collider.r;
        shiftColliders();
    }
    
    //If the dot collided or went too far up or down
    if( ( mPosY - collider.r < 0 ) || checkCollision( collider, square ) || checkCollision( collider, circle ) ) {
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Dot::render(LTexture& texture, SDL_Rect& clip)
{
    //Show the dot
    texture.render( mPosX - collider.r, mPosY - collider.r, &clip);
}

void Dot::shiftColliders()
{
    //Align collider to center of dot
    collider.x = mPosX;
    collider.y = mPosY;
}
