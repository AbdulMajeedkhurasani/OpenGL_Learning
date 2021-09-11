#ifndef _GAME_H
#define _GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameLevel.h"

//represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// init size and velocity of player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

// Game Class
class Game
{
    public:
        //Game state
        GameState State;
        bool Keys[1024];
        unsigned int Width, Height;
        std::vector<GameLevel> Levels;
        unsigned int Level;
        //constructor/distructor
        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize game state (load all shaders/textures/levels)
        void Init();

        //Game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};

#endif