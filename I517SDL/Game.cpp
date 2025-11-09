#include "Game.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

//constructor, gets renderer and input manager from engine
Game::Game(SDL_Renderer* renderer, InputManager* input)
{
	this->renderer = renderer;//assign renderer from engine
    this->inputManager = input;	//assign input manager from engine
    srand(static_cast<unsigned int>(time(nullptr)));

	//create player character


    std::cout << "Game initialised.\n";
    createGameObjects();
}

//creates all game objects at start
void Game::createGameObjects()
{
    std::cout << "Creating Game Objects\n";

    const int SPRITE_SIZE = 64;

    // Create player at a fixed position
    player = new PlayerCharacter(renderer, inputManager, "assets/fin.png", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    player->setSize(SPRITE_SIZE, SPRITE_SIZE);

    // Create autonomous objects
    for (int i = 0; i < 10; i++)
    {
        int xPos = rand() % (SCREEN_WIDTH - SPRITE_SIZE);
        int yPos = rand() % (SCREEN_HEIGHT - SPRITE_SIZE);

        GameObject* item = new GameObject(renderer, "assets/fin_2.png", xPos, yPos);
        item->setSize(SPRITE_SIZE, SPRITE_SIZE);

        float vx = (rand() % 12 + 2) * (rand() % 2 ? 1 : -1) * 25.0f;
        float vy = (rand() % 18 + 2) * (rand() % 2 ? 1 : -1) * 25.0f;
        item->setVelocity(vx, vy);

        items.push_back(item);
    }
}

//updates all game logic each frame (with delta time per week 3 exercise)
void Game::Update(float deltaTime)
{
	//updates player input and movement
    player->HandleInput(inputManager, deltaTime);
    player->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);
    player->ScreenWrap(SCREEN_WIDTH, SCREEN_HEIGHT);

     
	//projectile cooldown timer
    if (projectileCooldown > 0.0f)
        projectileCooldown -= deltaTime;

	//if space is pressed and cooldown is 0, fire projectile
    if (inputManager->IsKeyPressed(SDL_SCANCODE_SPACE) && projectileCooldown <= 0.0f)
    {
		//create projectile at player position, offset forward
        float px = player->GetX() + cos(player->GetAngle()) * 32.0f;
        float py = player->GetY() + sin(player->GetAngle()) * 32.0f;
		//add new projectile to list
		projectiles.push_back(new Projectile(renderer, "assets/bullet.png", px, py, player->GetAngle())); //create projectile

        projectileCooldown = projectileCooldownTime; //reset cooldown
    }

	//update GameObjects on screen
    for (auto& item : items) {
        item->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);
        item->ScreenWrap(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

	//collision detection between player and GameObjects
    for (auto& item : items)
    {
        if (item->getAliveState() && player->getAliveState())
        {
            if (player->CheckCollision(*item))
            {
                std::cout << "Collision detected! Bouncing off player.\n";
                item->BounceFrom(*player);
            }
        }
    }
    
	//cleanup inactive GameObjects
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [](GameObject* item) { return !item->getAliveState(); }),
        items.end());

	//update projectiles
    for (auto& p : projectiles)
        p->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);

	//cleamup inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](Projectile* p) { return !p->IsAlive(); }),
        projectiles.end());
}

//renders all game objects each frame to the window
void Game::Render()
{
	//render player
    player->Render(player->rotationDegrees);

	//render items
    for (auto& item : items)
        if (item->getAliveState()) item->Render(item->rotationAngle);

    // render projectiles
    for (auto& p : projectiles)
        if (p->IsAlive()) p->Render(p->rotationAngle);

}

Game::~Game()
{
    for (auto& item : items)
        delete item;
    items.clear();

    for (auto& p : projectiles)
        delete p;
    projectiles.clear();

    delete player;

}
