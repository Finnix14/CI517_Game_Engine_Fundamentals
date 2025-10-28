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
    //get current keyboard state
    currentKeyStates = SDL_GetKeyboardState(&numKeys);

    //initialise previousKeyStates on first run
    if (previousKeyStates.empty())
        previousKeyStates.resize(numKeys, 0);

    //update axis each frame
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
	if (!previousKeyStates.empty()) //make sure it's initialised
    {
        previousKeyStates.assign(
            currentKeyStates,
			currentKeyStates + numKeys //copy current states
        );
    }
}

