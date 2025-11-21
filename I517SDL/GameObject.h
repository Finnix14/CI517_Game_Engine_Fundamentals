    #pragma once
    #include <SDL.h>
    #include <SDL_image.h>
    #include <string>
    #include "Sprite.h"

    //basic game object with position, velocity and sprite
    class GameObject
    {
    public:
        GameObject(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
        ~GameObject();

        virtual void Update(int windowWidth, int windowHeight, float deltaTime);

        void Render(float angle);

        //movement
        void setVelocity(float vx, float vy);
        void MultiplyVelocity(float factor);
        void Stop();

	    //collision with screen edges
        void ScreenWrap(int screenWidth, int screenHeight);
        void ScreenLimit(int screenWidth, int screenHeight);
        void ScreenBounce(int screenWidth, int screenHeight);
        void RenderAtCamera(int camX, int camY, float angle);


        bool CheckCollision(const GameObject& other) const;
        void BounceFrom(const GameObject& other);
        void resolveCollision(const GameObject& other, float prevX, float prevY);

	    //getters / setters
        bool getAliveState() const { return isActive; }
        void setAlive(bool newState) { isActive = newState; }
        void setSize(int width, int height);
        void Destroy() { isActive = false; }

        int objectType = 0;
	    float rotationAngle = 0.0f;
        bool isSolid = false;


	    // --- Accessors for position ---
        float GetX() const { return x; }
        float GetY() const { return y; }

        void SetPosition(float newX, float newY);
        void AddXY(float dx, float dy)
        {
            x += dx;
            y += dy;
        }

	    //make varables accessible to child classes
    protected:
        bool isActive = false;
        float x = 0, y = 0;

        float vx = 0.0f;
        float vy = 0.0f;

        Sprite* sprite = nullptr;
        SDL_Renderer* renderer = nullptr;

        int width = 32, height = 32;
    };

