#include "BallObject.h"

BallObject::BallObject(): 
    GameObject(), Radius(12.5f), Stuck(true), Sticky(false), PassThrough(false) {}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    :GameObject(pos,glm::vec2(2.0f * radius), sprite, glm::vec3(1.0f),velocity), Radius(radius), Stuck(true), Sticky(false), PassThrough(false)
{
    
}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth)
{
    if(!this->Stuck)
    {
        //move the ball
        this->Position += this->Velocity * dt;
        // check if outside window bounds; if so, reverse velocity
        // and restore at correct position
        if(this->Position.x <= 0.0f)
        {
            this->Velocity.x = - this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= windowWidth)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = windowWidth - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}
    
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Sticky = false;
    this->PassThrough = false;
}
