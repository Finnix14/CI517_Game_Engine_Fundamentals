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
    void Update(int windowWidth, int windowHeight, float deltaTime);
    void Render(float angle);

	//movement modes (like a state machine)
    enum MovementMode { STEP, SMOOTH, ROTATE };
    MovementMode currentMode = SMOOTH;

	//movement implementations
   // --- Accessors for position and facing ---
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetAngle() const { return angle; }
    float GetVX() const { return vx; }
    float GetVY() const { return vy; }

    float rotationDegrees = 0.0f;
    float previousX = 0.0f;
    float previousY = 0.0f;
    float camX;
    float camY;

	//camera position
    float cameraX = 0;
    float cameraY = 0;
	//sets camera position
    void SetCamera(float camX, float camY);


protected:
    InputManager* inputManager;
    float acceleration = 2.0f;
    float drag = 0.92f;          //slows down movement
    float angle = 0.0f; //current facing direction in radians
  


};
