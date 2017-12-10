#ifndef DOT_H
#define DOT_H

#include <SDL2/SDL.h>

#include "circle.h"
#include "ltexture.h"

class Dot
{
public:
    //The dimensions of the dot
    static int DOT_WIDTH;
    static int DOT_HEIGHT;

    int mPosX, mPosY;
    int mW, mH;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 1;

    //Initializes the variables
    Dot(int x, int y );

//Takes key presses and adjusts the dot's velocity
    void handleEvent( SDL_Event& e );

    //Moves the dot and checks collision
    void move( SDL_Rect& square, Circle& circle );

    //Shows the dot on the screen
    void render(LTexture& t, SDL_Rect& r);

    //Gets collision circle
    Circle& getCollider();

private:
    //The X and Y offsets of the dot

    //The velocity of the dot
    int mVelX, mVelY;
		
    //Dot's collision circle
    Circle mCollider;

    //Moves the collision circle relative to the dot's offset
    void shiftColliders();
};

#endif
