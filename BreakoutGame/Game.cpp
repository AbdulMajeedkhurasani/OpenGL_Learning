#include "Game.h"
#include "includes/Utiliy/resource_manager.h"
#include "includes/Utiliy/texture.h"
#include "SpriteRenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;


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
    ResourceManager::LoadTexture("resources/textures/awesomeface.png", true, "face");
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
   
}

void Game::Render()
{
    Texture2D texture = ResourceManager::GetTexture("face");
    Renderer->DrawSprite(texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}