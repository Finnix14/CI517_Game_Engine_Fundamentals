#include "GameObject.h"
#include <iostream>
#include <algorithm>

//constructor, sets up object and loads its sprite
GameObject::GameObject(SDL_Renderer* renderer, const std::string& filePath, int x, int y)
{
    this->renderer = renderer;
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);

    //actually creates the sprite for this object
    sprite = new Sprite(renderer, filePath, x, y);
    isActive = true;
}
//destructor, frees sprite memory   
GameObject::~GameObject()
{
    delete sprite; //free memory when object is deleted
}
//sets the velocity of the object
void GameObject::setVelocity(float vx, float vy)
{
    this->vx = vx;
    this->vy = vy;
}
//handles movement and bouncing off edges
void GameObject::Update(int windowWidth, int windowHeight, float deltaTime)
{
    x += vx * deltaTime;
    y += vy * deltaTime;
    //update sprite pos so x/y match
    sprite->setPosition(static_cast<int>(x), static_cast<int>(y));
    ScreenBounce(windowWidth, windowHeight);
}
//tells sprite to draw itself with optional rotation
void GameObject::Render(float angle)
{
    if (sprite)
        sprite->Render(angle);
}
//resizes sprite and updates internal size for collision on edges
void GameObject::setSize(int width, int height)
{
    this->width = width;
    this->height = height;

    if (sprite)
        sprite->SetSize(width, height);
}
//updates velocity by a multiplier
void GameObject::MultiplyVelocity(float factor)
{
	vx *= factor; //multiply velcity by our multiplier
    vy *= factor;
}
//stops all movement
void GameObject::Stop()
{
    vx = 0;
    vy = 0;
}
//collision handling methods. instead of hardcoding edge checks, use methods to improve readbility
//wraps object to opposite side of screen when it goes off one edge
void GameObject::ScreenWrap(int windowWidth, int windowHeight)
{
    if (x < 0) x = 0;
    else if (x + width > windowWidth) x = windowWidth - width;

    if (y < 0) y = 0;
    else if (y + height > windowHeight) y = windowHeight - height;

}
//limits object to stay within screen bounds
void GameObject::ScreenLimit(int w, int h)
{
    if (x < 0) x = 0;
    else if (x + width > w) x = w - width;
    if (y < 0) y = 0;
    else if (y + height > h) y = h - height;
}
//bounces object off screen edges by reversing velocity
void GameObject::ScreenBounce(int w, int h)
{
    if (x <= 0 || x + width >= w) vx = -vx;
    if (y <= 0 || y + height >= h) vy = -vy;
}


