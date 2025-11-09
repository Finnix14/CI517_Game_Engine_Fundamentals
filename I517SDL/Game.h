#pragma once
#include <SDL.h>
#include <vector>
#include "GameObject.h"
#include "InputManager.h"
#include "PlayerCharacter.h"
#include "Projectile.h"


//screen settings and constants (never change)
constexpr auto GAME_TITLE = "CI517 SDL Game Example";
constexpr auto SCREEN_WIDTH = 800;
constexpr auto SCREEN_HEIGHT = 600;
constexpr auto FPS = 60;

//controls object creation, updates and rendering
class Game
{
public:
    Game(SDL_Renderer* renderer, InputManager* input); //gets renderer from engine and input manager
    ~Game();

    void Update(float deltaTime);
    void Render(); 

private:
    void createGameObjects();
    InputManager* inputManager;
    SDL_Renderer* renderer = nullptr; 
    GameObject* backGround = nullptr; 
    std::vector<GameObject*> items;   //list of active objects

    std::vector<Projectile*> projectiles;
    float projectileCooldown = 0.0f;    //time remaining before next shot
    const float projectileCooldownTime = 0.3f; //seconds between shots

    bool isPaused = false;
    PlayerCharacter* player;
};
