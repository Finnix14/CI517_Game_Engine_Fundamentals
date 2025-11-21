#include "InputManager.h"
#include <iostream>

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
        if (event.button.button == SDL_BUTTON_LEFT)
        {
			//update mouse left button states
            mouseLeftPressed = true;
            mouseLeftHeld = true;
        }
    }

	// --- MOUSE BUTTON UP ---
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
			//update mouse left button states
            mouseLeftReleased = true;
            mouseLeftHeld = false;
        }
    }
}

void InputManager::Update()
{
    // --- keyboard ---
    const Uint8* keyboard = SDL_GetKeyboardState(&numKeys);
    currentKeyStates = keyboard;

    if (previousKeyStates.empty())
        previousKeyStates.resize(numKeys);

	// --- AXIS CALCULATION ---
    xAxis = (currentKeyStates[SDL_SCANCODE_D] - currentKeyStates[SDL_SCANCODE_A]);
    yAxis = (currentKeyStates[SDL_SCANCODE_S] - currentKeyStates[SDL_SCANCODE_W]);
}

bool InputManager::IsKeyHeld(SDL_Scancode key)
{
    return currentKeyStates[key];
}

bool InputManager::IsKeyPressed(SDL_Scancode key)
{
    return currentKeyStates[key] && !previousKeyStates[key];
}

bool InputManager::IsKeyReleased(SDL_Scancode key)
{
    return !currentKeyStates[key] && previousKeyStates[key];
}

void InputManager::StorePreviousKeyStates()
{
	// --- MOUSE RESET FLAG ---
    mouseLeftPressed = false;
    mouseLeftReleased = false;

    if (!previousKeyStates.empty())
    {
        previousKeyStates.assign(
            currentKeyStates,
            currentKeyStates + numKeys
        );
    }
}
