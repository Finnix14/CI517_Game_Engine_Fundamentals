#pragma once
#include <SDL.h>
#include <unordered_map> //allows for key state storage

//handles keyboard input states
class InputManager {
public:
	void ProcessInput(SDL_Event& event); //input event processing
    void Update();  //clears transient states per frame

	bool IsKeyPressed(SDL_Scancode key); //if key pressed this frame
	bool IsKeyHeld(SDL_Scancode key); //which key is being held down
	bool IsKeyReleased(SDL_Scancode key); //if key released this frame

	void StorePreviousKeyStates();

	float xAxis = 0.0f;
	float yAxis = 0.0f;

private:
	const Uint8* currentKeyStates = nullptr;
	std::vector<Uint8> previousKeyStates;

	int numKeys = 0;
};
