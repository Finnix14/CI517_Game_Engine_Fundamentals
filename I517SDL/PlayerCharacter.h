#pragma once
#include "GameObject.h"
#include "InputManager.h"

//handles player controlled character
class PlayerCharacter : public GameObject //inherit from GameObject
{
public:
	//constructor, input and update handling
    PlayerCharacter(SDL_Renderer* renderer, InputManager* input, const std::string& filePath, int x, int y);
    void HandleInput(InputManager* input, float deltaTime);
    void Update(int windowWidth, int windowHeight, float deltaTime) override;

	//movement modes (like a state machine)
    enum MovementMode { STEP, SMOOTH, ROTATE };
    MovementMode currentMode = SMOOTH;

	//movement implementations
    void StepMove(float deltaTime);
    void SmoothMove(float deltaTime);
    void RotateMove(float deltaTime);
    float rotationDegrees = 0.0f;

protected:
    InputManager* inputManager;
    float acceleration = 2.0f;
    float drag = 0.92f;          //slows down movement
    float angle = 0.0f; //current facing direction in radians
};
