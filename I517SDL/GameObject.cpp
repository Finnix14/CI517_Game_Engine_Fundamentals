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
//sets the position of the object and updates sprite position
void GameObject::SetPosition(float newX, float newY)
{
    x = newX;
    y = newY;
    if (sprite)
        sprite->setPosition((int)x, (int)y);
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
//checks for collision with another GameObject using AABB
bool GameObject::CheckCollision(const GameObject& other) const
{
    SDL_Rect a = { static_cast<int>(x), static_cast<int>(y), width, height };
    SDL_Rect b = { static_cast<int>(other.x), static_cast<int>(other.y), other.width, other.height };
    return SDL_HasIntersection(&a, &b);
}
//bounces object away from another GameObject upon collision
void GameObject::BounceFrom(const GameObject& other)
{
    // More advanced physics could reflect based on position difference.
    if (x < other.x + other.width && x + width > other.x)
        vx = -vx;  // horizontal bounce

    if (y < other.y + other.height && y + height > other.y)
        vy = -vy;  // vertical bounce

    //small position adjustment to prevent getting stuck inside the player
    x += vx * 0.08f;
    y += vy * 0.08f;
}

