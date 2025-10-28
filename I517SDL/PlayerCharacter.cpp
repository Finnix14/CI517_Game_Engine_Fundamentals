#include "PlayerCharacter.h"
#include <iostream>
#include <cmath> 

//constructor, sets up input manager
PlayerCharacter::PlayerCharacter(SDL_Renderer* renderer, InputManager* input, const std::string& filePath, int x, int y)
    : GameObject(renderer, filePath, x, y)
{
    this->inputManager = input;
    std::cout << "PlayerCharacter created.\n";
}
//updates each frame, handles movement mode / state switching
void PlayerCharacter::Update(int windowWidth, int windowHeight, float deltaTime)
{
    if (inputManager->IsKeyPressed(SDL_SCANCODE_1)) currentMode = STEP;
    if (inputManager->IsKeyPressed(SDL_SCANCODE_2)) currentMode = SMOOTH;
    if (inputManager->IsKeyPressed(SDL_SCANCODE_3)) currentMode = ROTATE;



	//using a switch to select movement mode instead of if/else or selecting one and running
    switch (currentMode)
    {
    case STEP: StepMove(deltaTime);
        break;
    case SMOOTH: SmoothMove(deltaTime);
        break;
    case ROTATE: RotateMove(deltaTime); 
        break;
    }

    //smooth acceleration based movement
    float ax = 0.0f, ay = 0.0f;

    if (inputManager->IsKeyHeld(SDL_SCANCODE_W)) ay -= acceleration;
    if (inputManager->IsKeyHeld(SDL_SCANCODE_S)) ay += acceleration;
    if (inputManager->IsKeyHeld(SDL_SCANCODE_A)) ax -= acceleration;
    if (inputManager->IsKeyHeld(SDL_SCANCODE_D)) ax += acceleration;

    //apply acceleration
    this->vx += ax * deltaTime;
    this->vy += ay * deltaTime;

    //apply drag
    this->vx *= drag;
    this->vy *= drag;

    //update position
    this->x += this->vx * deltaTime;
    this->y += this->vy * deltaTime;

    //update sprite position
    sprite->setPosition(static_cast<int>(this->x), static_cast<int>(this->y));

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
//step movement implementation, moves at fixed speed based on input
void PlayerCharacter::StepMove(float deltaTime)
{
    const float MOVE_SPEED = 200.0f;
    x += inputManager->xAxis * MOVE_SPEED * deltaTime;
    y += inputManager->yAxis * MOVE_SPEED * deltaTime;
    sprite->setPosition((int)x, (int)y);
}
//smooth movement implementation, accelerates and decelerates based on input
void PlayerCharacter::SmoothMove(float deltaTime)
{
    const float ACCEL = 50.0f;
    const float DRAG = 0.9f;

    vx += inputManager->xAxis * ACCEL * deltaTime;
    vy += inputManager->yAxis * ACCEL * deltaTime;

    x += vx * deltaTime;
    y += vy * deltaTime;

    vx *= DRAG;
    vy *= DRAG;

    sprite->setPosition((int)x, (int)y);
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

    rotationDegrees = angle * (180.0f / M_PI);


    // move forward
    if (inputManager->IsKeyHeld(SDL_SCANCODE_W)) {
        vx = cos(angle) * MOVE_SPEED;
        vy = sin(angle) * MOVE_SPEED;
        x += vx * deltaTime;
        y += vy * deltaTime;
    }

    sprite->setPosition((int)x, (int)y);
    sprite->Render(rotationDegrees);
}