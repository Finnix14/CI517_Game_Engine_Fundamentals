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
    std::cout << "PLAYER CREATED AT ADDRESS: " << this << std::endl;
}

//updates player each frame
void PlayerCharacter::Update(int windowWidth, int windowHeight, float deltaTime)
{
    //store previous position for collision resolution
    previousX = x;
    previousY = y;

    // --- MOVEMENT ---
    x += vx * deltaTime;
    y += vy * deltaTime;

    // === ROTATION TOWARD MOUSE ===
    int mx = inputManager->GetMouseX();
    int my = inputManager->GetMouseY();

    //convert to world space
    float worldMouseX = mx + cameraX;
    float worldMouseY = my + cameraY;

    //player center
    float centerX = x + width * 0.5f;
    float centerY = y + height * 0.5f;

    //direction
    float dx = worldMouseX - centerX;
    float dy = worldMouseY - centerY;

    //angle (radians)
    angle = atan2(dy, dx);

    //convert to degrees
    rotationDegrees = (angle * 180.0f / M_PI);

    //call GameObject update
    GameObject::Update(windowWidth, windowHeight, 0.0f);
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

    setVelocity(vx, vy);
}
//renders player normally
void PlayerCharacter::Render(float angle)
{
    sprite->setPosition(x, y);
    sprite->Render(angle);
}
//sets camera position
void PlayerCharacter::SetCamera(float camX, float camY)
{
    cameraX = camX;
    cameraY = camY;
}

