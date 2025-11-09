#pragma once
#include "GameObject.h"
#include <cmath>

class Projectile : public GameObject
{
public:
    Projectile(SDL_Renderer* renderer, const std::string& filePath, float startX, float startY, float angle)
        : GameObject(renderer, filePath, (int)startX, (int)startY)
    {
        const float SPEED = 600.0f;
        vx = cos(angle) * SPEED;
        vy = sin(angle) * SPEED;
        lifetime = 2.0f; // seconds
    }

    void Update(int windowWidth, int windowHeight, float deltaTime) override
    {
        lifetime -= deltaTime;
        x += vx * deltaTime;
        y += vy * deltaTime;

        if (lifetime <= 0 || x < 0 || y < 0 || x > windowWidth || y > windowHeight)
            isActive = false;

        sprite->setPosition((int)x, (int)y);
    }

    bool IsAlive() const { return isActive; }

private:
    float lifetime;
};
