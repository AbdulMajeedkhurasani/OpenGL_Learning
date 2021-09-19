#ifndef _QUATERNION_H
#define _QUATERNION_H

#include <glm/glm.hpp>
#include <math.h>

class Quat
{
    public:
        glm::vec4 quat;
        glm::mat4 rotMat4;

        Quat(glm::vec4 v = glm::vec4(0.0f,0.0f,0.0f,1.0f))
        {
            float magnitude = sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
            quat.w = v.w / magnitude;
            quat.x = v.x / magnitude;
            quat.y = v.y / magnitude;
            quat.z = v.z / magnitude;
            rotMat4 = glm::mat4(1.0f);
        }
        
        glm::mat4 QuatRotMatrix4f(glm::vec4 rotAxis)
        {
            
            float mag = magnitude(glm::vec3(rotAxis.x, rotAxis.y, rotAxis.z));
            if (mag != 0)
            {
                rotAxis.x /=  mag;
                rotAxis.y /=  mag;
                rotAxis.z /=  mag;
            }
            glm::vec4 rotationQuat;
            float angleRadians = glm::radians(rotAxis.w / 2);
            rotationQuat.w = cosf(angleRadians);
            rotationQuat.x = rotAxis.x * sinf(angleRadians );
            rotationQuat.y = rotAxis.y * sinf(angleRadians );
            rotationQuat.z = rotAxis.z * sinf(angleRadians );
            rotVec(rotationQuat);

            rotMat4 = glm::mat4(1.0f);

            rotMat4[0][0] = 1 - 2*quat.y*quat.y - 2*quat.z*quat.z;
            rotMat4[0][1] = 2*quat.x*quat.y - 2*quat.w*quat.z;
            rotMat4[0][2] = 2*quat.x*quat.z + 2*quat.w*quat.y;
            rotMat4[1][0] = 2*quat.x*quat.y + 2*quat.w*quat.z;
            rotMat4[1][1] = 1 - 2*quat.x*quat.x - 2*quat.z*quat.z;
            rotMat4[1][2] = 2*quat.y*quat.z - 2*quat.w*quat.x;
            rotMat4[2][0] = 2*quat.x*quat.z - 2*quat.w*quat.y;
            rotMat4[2][1] = 2*quat.y*quat.z + 2*quat.w*quat.x;
            rotMat4[2][2] = 1 - 2*quat.x*quat.x - 2*quat.y*quat.y;

            return rotMat4;

        }

    private:

        float magnitude(glm::vec3 rotAxis)
        {
            return sqrt(rotAxis.x*rotAxis.x + rotAxis.y*rotAxis.y + rotAxis.z*rotAxis.z); 
        }

        void rotVec(glm::vec4 Q1)
        {
            glm::vec4 Q1xquat;
            Q1xquat.w = (Q1.w * quat.w - Q1.x*quat.x - Q1.y*quat.y - Q1.z*quat.z);
            Q1xquat.x = (Q1.w * quat.x + Q1.x*quat.w + Q1.y*quat.z - Q1.z*quat.y);
            Q1xquat.y = (Q1.w * quat.y - Q1.x*quat.z + Q1.y*quat.w + Q1.z*quat.x);
            Q1xquat.z = (Q1.w * quat.z + Q1.x*quat.y - Q1.y*quat.x + Q1.z*quat.w);
            quat.x = Q1xquat.x;
            quat.w = Q1xquat.w;
            quat.z = Q1xquat.z;  
            quat.y = Q1xquat.y;
        }
};

#endif