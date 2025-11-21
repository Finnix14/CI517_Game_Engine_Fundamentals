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

    previousX = x;
    previousY = y;

    // --- MOVEMENT (same as before) ---
    x += vx * deltaTime;
    y += vy * deltaTime;

    // === ROTATION TOWARD MOUSE ===
    const float SPRITE_ROT_OFFSET = 180.0f; // adjust if sprite faces wrong direction

    int mx = inputManager->GetMouseX();
    int my = inputManager->GetMouseY();

    // Convert mouse to world position
    float worldMouseX = mx + camX;
    float worldMouseY = my + camY;

    // Calculate direction vector
    float dx = worldMouseX - (x + width / 2);
    float dy = worldMouseY - (y + height / 2);

    // Angle in radians
    angle = atan2(dy, dx);

    GameObject::Update(windowWidth, windowHeight, 0.0f);

    // Convert to degrees + sprite offset
    rotationDegrees = (angle * 180.0f / M_PI) + SPRITE_ROT_OFFSET;
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
