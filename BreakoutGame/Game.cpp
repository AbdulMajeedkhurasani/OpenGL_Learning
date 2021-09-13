#define PB(a) push_back(a)

#include "Game.h"
#include "includes/Utiliy/resource_manager.h"
#include "includes/Utiliy/texture.h"
#include "SpriteRenderer.h"
#include "particleGenerator.h"



// Game-related State data
SpriteRenderer  *Renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    // deleting object of polymorphic class type ‘BallObject’ or 'PlayerObject' 
    //which has non-virtual destructor might cause undefined 
    //behavior [-Wdelete-non-virtual-dtor]
    //delete Player;
    //delete Ball;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("includes/Shaders/sprite.vs", "includes/Shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("includes/Shaders/particles.vs", "includes/Shaders/particles.fs", nullptr, "particle");
    // configure renderer shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    Shader shader = ResourceManager::GetShader("sprite");
    shader.Use().SetInteger("image", 0);
    shader.SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(shader);
    // configure particle shader
    shader = ResourceManager::GetShader("particle");
    shader.Use().SetInteger("sprite", 0);
    shader.SetMatrix4("projection", projection);
    // load textures
    ResourceManager::LoadTexture("resources/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("resources/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("resources/textures/block.png", false, "block");
    ResourceManager::LoadTexture("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("resources/textures/particle.png", true, "particle");
    // load levels
    GameLevel one; one.Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("resources/levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("resources/levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("resources/levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.PB(one);
    this->Levels.PB(two);
    this->Levels.PB(three);
    this->Levels.PB(four);
    this->Level = 2;
    // configure Player objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    // configure Ball object
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
}

void Game::Update(float dt)
{
    // Update ball object
    Ball->Move(dt, Width);
    // check for collisions
    this->DoCollisions();
    // reset if ball reached bottom line
    if (Ball->Position.y >= this->Height)
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
    // update particles 
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
    
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
        // draw player
        Player->Draw(*Renderer);
        // draw particles
        Particles->Draw();
        // draw ball
        Ball->Draw(*Renderer);
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)
        this->Levels[0].Load("resources/levels/one.lvl", this->Width, this->Height / 2);
    else if (this->Level == 1)
        this->Levels[1].Load("resources/levels/two.lvl", this->Width, this->Height / 2);
    else if (this->Level == 2)
        this->Levels[2].Load("resources/levels/three.lvl", this->Width, this->Height / 2);
    else if (this->Level == 3)
        this->Levels[3].Load("resources/levels/four.lvl", this->Width, this->Height / 2);
}

void Game::ResetPlayer()
{
    // reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}


void Game::DoCollisions()
{
    for (GameObject &box : Levels[Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision))  // if collision is true
            {
                // distroy block if not solid
                if(!box.IsSolid)
                    box.Destroyed = true;
                // collision resolution
                Direction dir = std::get<1> (collision);
                glm::vec2 diffVector = std::get<2>(collision);
                if(dir == LEFT || dir == RIGHT) // horizontal collition
                {
                    Ball->Velocity.x = -Ball->Velocity.x;  // reverse
                    // relocate 
                    float penetration =  Ball->Radius - std::abs(diffVector.x);
                    if(dir == LEFT)
                        Ball->Position.x += penetration; // move right
                    else 
                        Ball->Position.x -= penetration; // move left
                }
                else // vertical collision
                {
                    Ball->Velocity.y = -Ball->Velocity.y;  // reverse
                    // relocate
                    float penetration = Ball->Radius - std::abs(diffVector.y);
                    if(dir == UP)
                        Ball->Position.y -= penetration; // move up
                    else
                        Ball->Position.y += penetration; // move down
                }
            }
        }
    }

    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity 
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // then move accordingly 
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
        Ball->Velocity   = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
    
}

float Game::clamp(float value, float min, float max)
{
    return std::max(min, std::min(max, value));
}

// because there is no information about collision shape/ shape of 
// object we need to calculate shape of Game object and ball object
// circular collision
Collision Game::CheckCollision(BallObject &one, GameObject &two)
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

    if(glm::length(difference) <= one.Radius)
        return std::make_tuple(true, this->VectorDirection(difference), difference);
    else 
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}
// rectangular collision
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

Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2( 0.0f,  1.0f), // up
        glm::vec2( 1.0f,  0.0f), // right
        glm::vec2( 0.0f, -1.0f), // down
        glm::vec2(-1.0f,  0.0f), // left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for(unsigned int i = 0; i < 4; i++)
    {
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if(dotProduct > max)
        {
            max = dotProduct;
            bestMatch = i;
        }
    }
    return (Direction)bestMatch;
}