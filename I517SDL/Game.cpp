#include "Game.h"
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
	//update player
    player->HandleInput(inputManager, deltaTime);
    player->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);
	player->ScreenWrap(SCREEN_WIDTH, SCREEN_HEIGHT);

    //update items
    for (auto& item : items) {
        item->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);
        item->ScreenWrap(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

	//move items
    for (auto& item : items)
    {
        float vx = 0.0f;
        float vy = 0.0f;

        //apply combined velocity
        if (vx != 0 || vy != 0)
            item->setVelocity(vx, vy);
    }
}

//renders all game objects each frame to the window
void Game::Render()
{
	//render player
    player->Render(player->rotationDegrees);

	//render items
    for (auto& item : items)
        if (item->getAliveState()) item->Render(item->rotationAngle);
}

Game::~Game()
{
    for (auto& item : items)
        delete item;
    items.clear();
    delete player;

}
