#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "Shader.h"
#include "quaternion.h"

#include <string>
#include <vector>

using namespace std;

class Model 
{
public:
    // model data 
    string name;
    vector<Mesh>   meshes;
    Quat quaterinion;
    
    // constructor, expects a filepath to a 3D model.
    Model(string name, vector<Mesh> meshes)
    {
        this->name = name;
        this->meshes = meshes;
        this->quaterinion = Quat();
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        glm::mat4 model(1.0f);
        shader.setMat4("model", model);
        for(unsigned int i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);
    }
};

#endif
