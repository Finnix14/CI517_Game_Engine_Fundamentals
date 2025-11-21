#include "Game.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "NPC.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Levels.h"
#include <SDL_mixer.h>

//current game state
GameState currentState = GameState::MENU;
const int TILE_SIZE = 32;
const float SPRITE_ROT_OFFSET = -90.0f;

//audio
Mix_Music* music = NULL;
Mix_Chunk* soundEffect = NULL;
Mix_Chunk* laserSound = NULL;

//constructor, gets renderer and input manager from engine
Game::Game(SDL_Renderer* renderer, InputManager* input)
{
	this->renderer = renderer;//assign renderer from engine
    this->inputManager = input;	//assign input manager from engine
    srand(static_cast<unsigned int>(time(nullptr)));


    uiText = new TextRenderer(renderer, "assets/arial.ttf", 28); //creating text and setting the size, where it should be rendered etc.


    //initialize SDL_mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    music = Mix_LoadMUS("assets/audio/DDLoop1.wav");
    laserSound = Mix_LoadWAV("assets/audio/shoot.wav");
    soundEffect = Mix_LoadWAV("assets/audio/explosion.wav");
	//create game objects
    std::cout << "Game initialised.\n";

}
//updates all game logic each frame (with delta time per week 3 exercise)
void Game::Update(float deltaTime)
{


    switch (currentState)
    {
    case GameState::MENU:

        if (inputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
        {
            Mix_PlayMusic(music, -1);
            loadMap(currentLevel);
            currentState = GameState::PLAYING;
        }
        break;


    case GameState::PLAYING:
    {
         camX = player->GetX() - SCREEN_WIDTH / 2;
         camY = player->GetY() - SCREEN_HEIGHT / 2;
        // 1. Handle input FIRST
        player->HandleInput(inputManager, deltaTime);

        // 2. Update player (rotation + world position)
        player->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);


        // 3. NPCs chase the updated player world coords
        for (auto& npc : npcs)
            npc->ChasePlayer(player->GetX(), player->GetY(), deltaTime);


		// 4. Player collision with walls
        for (auto& b : barriers)
        {
            if (player->CheckCollision(*b))
                player->resolveCollision(*b, player->previousX, player->previousY);
        }
        // 5. NPC physics + wall collision
        for (auto& npc : npcs)
        {
            npc->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);
            for (auto& b : barriers)
            {
                if (npc->CheckCollision(*b))
                    npc->resolveCollision(*b, npc->previousX, npc->previousY);
                    npc->setVelocity(0, 0);
            }
            
        }
		// PROJECTILE COLLISIONS WITH NPCS
        for (auto& p : projectiles)
        {
            if (!p->IsAlive()) continue;

            for (auto& npc : npcs)
            {
                if (!npc->getAliveState()) continue;

                if (p->CheckCollision(*npc))
                {
                    npc->TakeDamage(1);
                    p->setAlive(false);
                    break; // stop checking other NPCs for this projectile
                }
            }
        }
		// 6. Player collision with walls (again, after NPCs moved)
        for (auto& b : barriers)
        {
            if (player->CheckCollision(*b))
                player->resolveCollision(*b, player->previousX, player->previousY);
        }
        // 7. Items collision etc
        for (auto& item : items)
        {
            if (player->CheckCollision(*item))
                item->BounceFrom(*player);
        }

        // ---- PROJECTILES ----
        if (projectileCooldown > 0.0f)
            projectileCooldown -= deltaTime;

        if (inputManager->IsMousePressed() && projectileCooldown <= 0.0f)
        {
            float px = player->GetX() + cos(player->GetAngle()) * 32.0f;
            float py = player->GetY() + sin(player->GetAngle()) * 32.0f;

            projectiles.push_back(new Projectile(renderer, "assets/bullet.png", px, py, player->GetAngle()));
            Mix_PlayChannel(-1, laserSound, 0);
            projectileCooldown = projectileCooldownTime;
        }

        for (auto& p : projectiles)
            p->Update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime);

        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](Projectile* p) { return !p->IsAlive(); }),
            projectiles.end()
        );

        npcs.erase(
            std::remove_if(npcs.begin(), npcs.end(),
                [](NPC* n) { return !n->getAliveState(); }),
            npcs.end()
        );

        items.erase(
            std::remove_if(items.begin(), items.end(),
                [](GameObject* o) { return !o->getAliveState(); }),
            items.end()
        );


        break;
    }


    case GameState::GAMEOVER:
        if (inputManager->IsKeyPressed(SDL_SCANCODE_R))

            Mix_HaltMusic(); //stop music indefinitely
            RestartGame();
        break;

    case GameState::LEVEL_COMPLETE:
        if (inputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
        {
            currentLevel++;
            Mix_HaltMusic();
            loadMap(currentLevel);
            Mix_PlayMusic(music, -1);
            currentState = GameState::PLAYING;
        }
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
        uiText->RenderText("PRESS SPACE TO SHOOT", 250, 250, { 255, 255, 255 });

    }

    if (currentState == GameState::GAMEOVER)
    {
        uiText->RenderText("GAME OVER - PRESS R TO RESTART", 180, 250, { 255, 0, 0 });
    }

    if (currentState == GameState::LEVEL_COMPLETE)
    {
        uiText->RenderText("LEVEL COMPLETE!", 250, 250, { 255, 255, 0 });
        uiText->RenderText("PRESS SPACE FOR NEXT LEVEL", 180, 300, { 255, 255, 255 });
        return;
    }

    if (currentState == GameState::MENU)
    {
        uiText->RenderText("MY SDL GAME", 320, 200, { 255,255,255 });
        uiText->RenderText("PRESS SPACE TO START", 250, 260, { 255,255,255 });
        return;
    }

    for (auto& b : barriers)
        b->RenderAtCamera(camX, camY, 0.0f);

    player->RenderAtCamera(camX, camY, player->rotationDegrees);

    for (auto& item : items)
        if (item->getAliveState())
            item->RenderAtCamera(camX, camY, item->rotationAngle);

    for (auto& p : projectiles)
        if (p->IsAlive())
            p->RenderAtCamera(camX, camY, p->rotationAngle);



}
//loads a level map from the Levels class
void Game::loadMap(int levelNumber)
{
    std::cout << "\n--- LOADING LEVEL " << levelNumber << " ---\n";

    // FULL RESET OF WORLD DATA
    for (auto& b : barriers) delete b;
    barriers.clear();

    for (auto& item : items) delete item;
    items.clear();

    for (auto& npc : npcs) delete npc;
    npcs.clear();

    if (player)
    {
        delete player;
        player = nullptr;
    }

    // BUILD LEVEL
    for (int y = 0; y < LEVEL_HEIGHT; y++)
    {
        for (int x = 0; x < LEVEL_WIDTH; x++)
        {
            int cell = levels.getTileContent(levelNumber, x, y);
            int worldX = x * TILE_SIZE;
            int worldY = y * TILE_SIZE;

            switch (cell)
            {
            case 1: // wall
            {
                GameObject* wall = new GameObject(renderer, "assets/Square_Cross_Grey.png", worldX, worldY);
                wall->setSize(TILE_SIZE, TILE_SIZE);
                wall->isSolid = true;
                barriers.push_back(wall);
                break;
            }

            case 2: // barrier
            {
                GameObject* wall = new GameObject(renderer, "assets/Square_Cross_Red.png", worldX, worldY);
                wall->setSize(TILE_SIZE, TILE_SIZE);
                wall->isSolid = true;
                barriers.push_back(wall);
                break;
            }

            case 3: // NPC
            {
                NPC* npc = new NPC(renderer, "assets/Pawn_Red.png", worldX, worldY);
                npc->setSize(TILE_SIZE, TILE_SIZE);
                npcs.push_back(npc);
                items.push_back(npc);
                break;
            }

            case 4: // player
            {
                player = new PlayerCharacter(renderer, inputManager, "assets/Fin.png", worldX, worldY);
                player->setSize(28, 28);
                player->SetPosition(worldX, worldY);
                break;
            }

            case 5: // collectible
            {
                GameObject* item = new GameObject(renderer, "assets/Fin_2.png", worldX, worldY);
                item->setSize(TILE_SIZE, TILE_SIZE);
                items.push_back(item);
                break;
            }

            case 9: // exit
            {
                GameObject* exitObj = new GameObject(renderer, "assets/Star_Yellow.png", worldX, worldY);
                exitObj->setSize(TILE_SIZE, TILE_SIZE);
                exitObj->objectType = 9;
                items.push_back(exitObj);
                break;
            }

            default:
                break;
            }
        }
    }

    std::cout << "Level Loaded. Barriers: " << barriers.size()
        << " Items: " << items.size()
        << " NPCs: " << npcs.size() << "\n";
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

	//free audios
    Mix_FreeMusic(music);
    Mix_FreeChunk(laserSound);
    Mix_FreeChunk(soundEffect);
    Mix_CloseAudio();

    delete player;

}
