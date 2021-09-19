#ifndef _MOUSE_H
#define _MOUSE_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Mouse
{
public:

    Mouse(){}
    
    glm::vec4 rotaionVector(GLFWwindow* window)
    {
        glm::vec2 directionVector = mouseMoveVector(window);
        /*if(directionVector.y == 0)
        {
            if (directionVector.x > 0)
                return glm::vec4(0.0f, 1.0f, 0.0f,  5.0f);
            if (directionVector.x < 0)
                return glm::vec4(0.0f, 1.0f, 0.0f, -5.0f);
            else
                return glm::vec4(0.0f, 0.0f, 0.0f,  0.0f);
        }
        if(directionVector.x == 0)
        {
            if (directionVector.y > 0)
                return glm::vec4(1.0f, 0.0f, 0.0f,  -5.0f);
            if (directionVector.y < 0)
                return glm::vec4(1.0f, 0.0f, 0.0f,   5.0f);
            else
                return glm::vec4(0.0f, 0.0f, 0.0f,   0.0f);
        }*/
        
        if(directionVector.y > 0 && directionVector.x > 0)
        {
            return glm::vec4(-1.0f, -1.0f, 0.0f,  5.0f);
        }
        if(directionVector.y > 0 && directionVector.x < 0)
        {
            return glm::vec4(-1.0f, 1.0f, 0.0f,  5.0f);
        }

        if(directionVector.y < 0 && directionVector.x > 0)
        {
            return glm::vec4(-1.0f, 1.0f, 0.0f,  -5.0f);
        }
        if(directionVector.y < 0 && directionVector.x < 0)
        {
            return glm::vec4(-1.0f, -1.0f, 0.0f,  -5.0f);
        }
        return glm::vec4(0.0f, 0.0f, 0.0f,  0.0f);
    }
private:
    double xPos, yPos, lastX, lastY, currentX, currentY;

    glm::vec2 mouseMoveVector(GLFWwindow *window)
    {
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        if (state == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xPos, &yPos);
            lastX = currentX; //last x
            lastY = currentY; //last y
            currentX = xPos;  //current x
            currentY = yPos;  //current y
        }
        else
            lastX = lastY = currentX = currentY = 0;

        return glm::vec2((currentX - lastX), (currentY - lastY));
    }
};
#endif