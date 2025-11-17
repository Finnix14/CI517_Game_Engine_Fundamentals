#include "Game.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Levels.h"

//current game state
GameState currentState = GameState::PLAYING;
const int TILE_SIZE = 32;

//constructor, gets renderer and input manager from engine
Game::Game(SDL_Renderer* renderer, InputManager* input)
{
	this->renderer = renderer;//assign renderer from engine
    this->inputManager = input;	//assign input manager from engine
    srand(static_cast<unsigned int>(time(nullptr)));


    uiText = new TextRenderer(renderer, "assets/arial.ttf", 28); //creating text and setting the size, where it should be rendered etc.

	//create game objects
    std::cout << "Game initialised.\n";
    loadMap(currentLevel);

}
//updates all game logic each frame (with delta time per week 3 exercise)
void Game::Update(float deltaTime)
{
    switch (currentState)
    {
    case GameState::MENU:
        if (inputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
            currentState = GameState::PLAYING;
        break;

    case GameState::PLAYING:
        // existing logic here//updates player input and movement
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

		//collision detection between player and barriers
        for (auto& item : items)
        {
            if (player->CheckCollision(*item))
                player->BounceFrom(*item);
        }

		//collision detection between player and barriers
        for (auto& b : barriers)
        {
            if (player->CheckCollision(*b))
            {
                player->BounceFrom(*b);
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
            projectiles.end());break;

    case GameState::GAMEOVER:
        if (inputManager->IsKeyPressed(SDL_SCANCODE_R))
            RestartGame();
        break;
    }
	
}
//restarts the game by resetting state and recreating objects
void Game::RestartGame()
{
    //reset game state and objects
    currentState = GameState::PLAYING;
    //clear existing items and projectiles
    for (auto& item : items)
        delete item;
    items.clear();
    for (auto& p : projectiles)
        delete p;
    projectiles.clear();
    //recreate game objects
    loadMap(currentLevel);

}
//renders all game objects each frame to the window
void Game::Render()
{
    if (currentState == GameState::PLAYING)
    {
        uiText->RenderText("PRESS SPACE TO START", 250, 250, { 255, 255, 255 });

    }

    if (currentState == GameState::GAMEOVER)
    {
        uiText->RenderText("GAME OVER - PRESS R TO RESTART", 180, 250, { 255, 0, 0 });
    }


    for(auto& b : barriers)
		b->Render(0.0f);


	//render player
    player->Render(player->rotationDegrees);

	//render items
    for (auto& item : items)
        if (item->getAliveState()) item->Render(item->rotationAngle);

    //render projectiles
    for (auto& p : projectiles)
        if (p->IsAlive()) p->Render(p->rotationAngle);




}
//loads a level map from the Levels class
void Game::loadMap(int levelNumber)
{
    for (auto& b : barriers) delete b;
    barriers.clear();
    std::cout << "Loading level " << levelNumber << "\n";

    //clear previous items
    for (auto& item : items)
        delete item;
    items.clear();

    //delete player
    if (player)
    {
        delete player;
        player = nullptr;
    }

    //loop through level grid
    for (int y = 0; y < LEVEL_HEIGHT; y++)
    {
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
            int cell = levels.getTileContent(levelNumber, x, y);
            int worldX = x * TILE_SIZE;
            int worldY = y * TILE_SIZE;

            switch (cell)
            {
            case 1: // Wall
            {
                GameObject* wall = new GameObject(renderer, "assets/Square_Cross_Grey.png", worldX, worldY);
                wall->setSize(TILE_SIZE, TILE_SIZE);
                barriers.push_back(wall);
                break;
            }

            case 2: // Barrier
            {
                GameObject* wall = new GameObject(renderer, "assets/Square_Cross_Red.png", worldX, worldY);
                wall->setSize(TILE_SIZE, TILE_SIZE);
                barriers.push_back(wall);
                break;
            }

            case 3: // NPC placeholder
                items.push_back(
                    new GameObject(renderer, "assets/Pawn_Red.png", worldX, worldY)
                );
                items.back()->setSize(TILE_SIZE, TILE_SIZE);
                break;

            case 4: // Player start
                player = new PlayerCharacter(renderer, inputManager, "assets/Fin.png", worldX, worldY);
                player->setSize(TILE_SIZE, TILE_SIZE);
                break;

            case 5: // Collectible
                items.push_back(
                    new GameObject(renderer, "assets/Fin_2.png", worldX, worldY)
                );
                items.back()->setSize(TILE_SIZE, TILE_SIZE);
                break;

            case 9: // Level exit
                items.push_back(
                    new GameObject(renderer, "assets/Star_Yellow.png", worldX, worldY)
                );
                items.back()->setSize(TILE_SIZE, TILE_SIZE);
                break;

            default:
                break; // 0 (empty)
            }
        }
    }
}
//destructor, cleans up all game objects
Game::~Game()
{
    for (auto& item : items)
        delete item;
    items.clear();

    for (auto& p : projectiles)
        delete p;
    projectiles.clear();

    delete uiText;
    uiText = nullptr;

    delete player;

}
