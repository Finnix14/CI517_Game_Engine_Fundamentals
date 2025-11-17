#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Sprite.h"

//basic game object with position, velocity and sprite
class GameObject
{
public:
    GameObject(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
    ~GameObject();

    virtual void Update(int windowWidth, int windowHeight, float deltaTime);

    void Render(float angle);

    //movement
    void setVelocity(float vx, float vy);
    void SetPosition(float newX, float newY);
    void MultiplyVelocity(float factor);
    void Stop();

	//collision with screen edges
    void ScreenWrap(int screenWidth, int screenHeight);
    void ScreenLimit(int screenWidth, int screenHeight);
    void ScreenBounce(int screenWidth, int screenHeight);
    bool CheckCollision(const GameObject& other) const;
    void BounceFrom(const GameObject& other);

	//getters / setters
    bool getAliveState() const { return isActive; }
    void setAlive(bool newState) { isActive = newState; }
    void setSize(int width, int height);
    void Destroy() { isActive = false; }


	float rotationAngle = 0.0f;
	//make varables accessible to child classes
protected:
    bool isActive = false;
    float x = 0, y = 0;

    float vx = 00.0f;
    float vy = 00.0f;

    Sprite* sprite = nullptr;
    SDL_Renderer* renderer = nullptr;

    int width = 32, height = 32;
};

