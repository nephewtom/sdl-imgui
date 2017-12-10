#ifndef DOT_H
#define DOT_H

#include <SDL2/SDL.h>

#include "circle.h"
#include "ltexture.h"

class Dot
{
public:
    int vel;
    int width;
    int height;
    Circle collider;

    int mPosX, mPosY;

    //Maximum axis velocity of the dot

    //Initializes the variables
    Dot(int x, int y, int w, int h);

//Takes key presses and adjusts the dot's velocity
    void handleEvent( SDL_Event& e );

    //Moves the dot and checks collision
    void move( SDL_Rect& square, Circle& circle );

    //Shows the dot on the screen
    void render(LTexture& t, SDL_Rect& r);

private:
    //The X and Y offsets of the dot

    //The velocity of the dot
    int mVelX, mVelY;
		
    //Dot's collision circle

    //Moves the collision circle relative to the dot's offset
    void shiftColliders();
};

#endif
