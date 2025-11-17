#include "PlayerCharacter.h"
#include <iostream>
#include <cmath>
#include <vector>
#include "Projectile.h"


//constructor, sets up input manager
PlayerCharacter::PlayerCharacter(SDL_Renderer* renderer, InputManager* input, const std::string& filePath, int x, int y)
    : GameObject(renderer, filePath, x, y)
{
    this->inputManager = input;
    std::cout << "PlayerCharacter created.\n";
}
//updates player each frame
void PlayerCharacter::Update(int windowWidth, int windowHeight, float deltaTime)
{
    //check rotation mode (replace with your own state if needed)
    RotateMove(deltaTime);

    //render player rotation
    sprite->setPosition((int)x, (int)y);
    sprite->Render(rotationDegrees);
}

//processes input for movement and sets velocity directly
void PlayerCharacter::HandleInput(InputManager* input, float deltaTime)
{
    const float MOVE_SPEED = 200.0f;

    float vx = 0.0f, vy = 0.0f;

    if (input->IsKeyHeld(SDL_SCANCODE_W)) vy -= MOVE_SPEED;
    if (input->IsKeyHeld(SDL_SCANCODE_S)) vy += MOVE_SPEED;
    if (input->IsKeyHeld(SDL_SCANCODE_A)) vx -= MOVE_SPEED;
    if (input->IsKeyHeld(SDL_SCANCODE_D)) vx += MOVE_SPEED;

    this->setVelocity(vx, vy);
}
//rotate movement implementation, rotates character and moves forward
void PlayerCharacter::RotateMove(float deltaTime)
{
    const float TURN_SPEED = 3.5f;
    const float MOVE_SPEED = 180.0f;

    //adjust angle smoothly (scale by deltaTime so rotation speed is consistent)
    if (inputManager->IsKeyHeld(SDL_SCANCODE_A))
        angle -= TURN_SPEED * deltaTime;
    if (inputManager->IsKeyHeld(SDL_SCANCODE_D))
        angle += TURN_SPEED * deltaTime;

	//keep angle within 0 to 2*PI range
    if (angle > M_PI * 2) angle -= M_PI * 2;
    if (angle < 0) angle += M_PI * 2;

	rotationDegrees = angle * (180.0f / M_PI); //convert to degrees for rendering


    // move forward
    if (inputManager->IsKeyHeld(SDL_SCANCODE_W)) {
        vx = cos(angle) * MOVE_SPEED; 
        vy = sin(angle) * MOVE_SPEED;
        x += vx * deltaTime;
        y += vy * deltaTime;
    }

    if (inputManager->IsKeyHeld(SDL_SCANCODE_S)) {
        vx = cos(angle) * MOVE_SPEED;
        vy = sin(angle) * MOVE_SPEED;
        x -= vx * deltaTime;
        y -= vy * deltaTime;
    }

    sprite->setPosition((int)x, (int)y);
    sprite->Render(rotationDegrees);
}