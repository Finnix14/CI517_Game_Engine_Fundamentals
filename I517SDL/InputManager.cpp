#include "InputManager.h"
#include <iostream>

//processes input events
void InputManager::ProcessInput(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        return;
}
//updates input states each frame
void InputManager::Update()
{
	const Uint8* keyboard = SDL_GetKeyboardState(&numKeys); //retrieve current key states
	currentKeyStates = keyboard; //make it an accessable variable

    if (previousKeyStates.empty())
        previousKeyStates.resize(numKeys);

    xAxis = (currentKeyStates[SDL_SCANCODE_D] - currentKeyStates[SDL_SCANCODE_A]);
    yAxis = (currentKeyStates[SDL_SCANCODE_S] - currentKeyStates[SDL_SCANCODE_W]);
}

//returns true if key is currently held down
bool InputManager::IsKeyHeld(SDL_Scancode key)
{
    return currentKeyStates[key];
}
//returns true if key was pressed this frame
bool InputManager::IsKeyPressed(SDL_Scancode key)
{
    return currentKeyStates[key] && !previousKeyStates[key];
}
//returns true if key was released this frame
bool InputManager::IsKeyReleased(SDL_Scancode key)
{
    return !currentKeyStates[key] && previousKeyStates[key];
}
//stores our current key states as previous for next frame
void InputManager::StorePreviousKeyStates()
{
	if (!previousKeyStates.empty()) //only store if we have previous states
    {
        previousKeyStates.assign(
            currentKeyStates,
			currentKeyStates + numKeys //store current states as previous
        );
    }
}


