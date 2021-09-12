#define PB(a) push_back(a)

#include "Game.h"
#include "includes/Utiliy/resource_manager.h"
#include "includes/Utiliy/texture.h"
#include "SpriteRenderer.h"



// Game-related State data
SpriteRenderer  *Renderer;
GameObject *Player;
BallObject *Ball;


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
    this->Levels.PB(one);
    this->Levels.PB(two);
    this->Levels.PB(three);
    this->Levels.PB(four);
    this->Level = 0;
    // configure Player objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    // configure Ball object
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::Update(float dt)
{
    // Update ball object
    Ball->Move(dt, Width);
    // check for collisions
    this->DoCollisions();
}

void Game::ProcessInput(float dt)
{
    if(this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        //move Player / Move PlayerBoard
        if(this->Keys[GLFW_KEY_A])
        {
            if(Player->Position.x >= 0.0f)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
                
            }
        }
        if(this->Keys[GLFW_KEY_D])
        {
            if(Player->Position.x <= (this->Width - Player->Size.x))
            {
                Player->Position.x += velocity;
                if(Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if(Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
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
        Ball->Draw(*Renderer);
    }
}

void Game::DoCollisions()
{
    for (GameObject &box : Levels[Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (this->CheckCollision(*Ball, box))
            {
                if(!box.IsSolid)
                    box.Destroyed = true;
            }
        }
        
    }
    if (this->CheckCollision(*Ball, *Player))
    {
        Ball->Velocity.y = -Ball->Velocity.y;
        //Ball->Velocity.x = -(Ball->Velocity.x + abs(Ball->Position.x - Player->Position.x)) / 2;
    }
    
}

float Game::clamp(float value, float min, float max)
{
    return std::max(min, std::min(max, value));
}

// because there is no information about collision shape/ shape of 
// object we need to calculate shape of Game object and ball object
bool Game::CheckCollision(BallObject &one, GameObject &two)
{
    // get center point circle first
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extends)
    glm::vec2 aabb_half_extends(two.Size / 2.0f);
    glm::vec2 aabb_center(two.Position + aabb_half_extends);
    // get differece vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extends, aabb_half_extends);
    // add clamped value to AABB_center and get the value closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // vector between center circle and closest and closest point AABB
    difference = closest - center;

    return glm::length(difference) < one.Radius;
}

bool Game::CheckCollision(GameObject &one, GameObject &two)
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}