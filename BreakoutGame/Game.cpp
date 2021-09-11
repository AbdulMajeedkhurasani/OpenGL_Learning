#include "Game.h"
#include "includes/Utiliy/resource_manager.h"
#include "includes/Utiliy/texture.h"
#include "SpriteRenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;
GameObject *Player;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("includes/Shaders/sprite.vs", "includes/Shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    Shader shader = ResourceManager::GetShader("sprite");
    shader.Use().SetInteger("image", 0);
    shader.SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(shader);
    // load textures
    ResourceManager::LoadTexture("resources/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("resources/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("resources/textures/block.png", false, "block");
    ResourceManager::LoadTexture("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("resources/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("resources/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("resources/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    if(this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        //move Player
        if(this->Keys[GLFW_KEY_A])
        {
            if(Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if(this->Keys[GLFW_KEY_D])
        {
            if(Player->Position.x <= (this->Width - Player->Size.x))
                Player->Position.x += velocity;
        }
    }
   
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D texture = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(texture,
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height));
        //draw level
        this->Levels[Level].Draw(*Renderer);
        Player->Draw(*Renderer);
    }
    //Texture2D texture = ResourceManager::GetTexture("face");
    //Renderer->DrawSprite(texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}