#include "NPC.h"
#include <cmath>
#include <iostream>

NPC::NPC(SDL_Renderer* renderer, const std::string& filePath, int x, int y)
    : GameObject(renderer, filePath, x, y)
{
    this->setSize(32, 32);
}

void NPC::Update(int windowWidth, int windowHeight, float deltaTime)
{
    // store last safe position for collision
    previousX = x;
    previousY = y;

    // move based on velocity set in ChasePlayer
    x += vx * deltaTime;
    y += vy * deltaTime;

    // update sprite/collider
    GameObject::Update(windowWidth, windowHeight, 0.0f);
}

void NPC::ChasePlayer(float playerX, float playerY, float deltaTime)
{
    float dx = playerX - x;
    float dy = playerY - y;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0)
    {
        dx /= length;
        dy /= length;
    }

    // set the velocity instead of moving directly
    float chaseSpeed = speed;
    vx = dx * chaseSpeed;
    vy = dy * chaseSpeed;
}
void NPC::TakeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        isAlive = false;
        setAlive(false);  // inherited GameObject flag (for rendering)
        std::cout << "NPC died!\n";
    }
}

