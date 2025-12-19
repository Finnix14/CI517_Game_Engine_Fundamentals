#include "InputManager.h"
#include <iostream>

//processes input events
void InputManager::ProcessInput(SDL_Event& event)
{
    // Handle quit
    if (event.type == SDL_QUIT)
        return;

	// --- MOUSE MOTION ---
    if (event.type == SDL_MOUSEMOTION)
    {
		//update mouse position
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }

	// --- MOUSE BUTTON DOWN ---
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
		if (event.button.button == SDL_BUTTON_LEFT) //left button pressed
        {
			//update mouse left button states
            mouseLeftPressed = true;
            mouseLeftHeld = true;
        }
    }

	// --- MOUSE BUTTON UP ---
    if (event.type == SDL_MOUSEBUTTONUP)
    {
		if (event.button.button == SDL_BUTTON_LEFT) //left button released
        {
			//update mouse left button states
            mouseLeftReleased = true;
            mouseLeftHeld = false;
        }
    }
}
//updates input states each frame
void InputManager::Update()
{
    // --- keyboard ---
	const Uint8* keyboard = SDL_GetKeyboardState(&numKeys); //get current keyboard state
    currentKeyStates = keyboard;

    if (previousKeyStates.empty())
		previousKeyStates.resize(numKeys); //initialize previous states on first run

	// --- AXIS CALCULATION ---
	xAxis = (currentKeyStates[SDL_SCANCODE_D] - currentKeyStates[SDL_SCANCODE_A]); //D - A
	yAxis = (currentKeyStates[SDL_SCANCODE_S] - currentKeyStates[SDL_SCANCODE_W]); //S - W
}
//checks if a key is being held down
bool InputManager::IsKeyHeld(SDL_Scancode key)
{
	return currentKeyStates[key]; //true if held down
}
//checks if a key was pressed this frame
bool InputManager::IsKeyPressed(SDL_Scancode key)
{
	return currentKeyStates[key] && !previousKeyStates[key]; //not held last frame, held now
}
//checks if a key was released this frame
bool InputManager::IsKeyReleased(SDL_Scancode key)
{
	return !currentKeyStates[key] && previousKeyStates[key]; //was held last frame, not held now
}
//stores the current key states as previous for next frame
void InputManager::StorePreviousKeyStates()
{
	// --- MOUSE RESET FLAG ---
    mouseLeftPressed = false;
    mouseLeftReleased = false;

	if (!previousKeyStates.empty()) //safety check
    {
        previousKeyStates.assign( 
            currentKeyStates,
			currentKeyStates + numKeys //assign current states to previous
        );
    }
}
