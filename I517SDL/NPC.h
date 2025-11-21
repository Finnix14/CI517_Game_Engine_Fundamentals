#pragma once
#include "GameObject.h"

class NPC : public GameObject
{
public:
    NPC(SDL_Renderer* renderer, const std::string& filePath, int x, int y);

    void Update(int windowWidth, int windowHeight, float deltaTime) override;
    void ChasePlayer(float playerX, float playerY, float deltaTime);

	//getters and setters for health
    void TakeDamage(int amount);
    bool IsDead() const { return !isAlive; }
    int GetHealth() const { return health; }

    float previousX = 0, previousY = 0;


private:
    float speed = 60.0f; // NPC movement speed
    int health = 3;          // default health
    bool isAlive = true;     // NPC alive state
};
