#ifndef _BALL_OBJECT_H
#define _BALL_OBJECT_H

#include "GameObject.h"

class BallObject: public GameObject
{
public:
    // ball state 
    float Radius;
    bool Stuck;

    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(float dt, unsigned int windowWidth);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif