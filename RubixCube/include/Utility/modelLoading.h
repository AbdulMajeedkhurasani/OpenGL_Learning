#ifndef _MODEL_LOaDING_H
#define _MODEL_LOaDING_H


#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

const std::string path = "/home/abkhu/opengl/source/RubixCube-OpenGL/resources/RubixCubeModel/RubixCube.obj";

class ModelLoading 
{
public:
    
    // constructor, expects a filepath to a 3D model.
    ModelLoading() 
    {
        loadModel(path);
    }

    ~ModelLoading()
    {
        models.clear();
    }
   
    vector<Model>  GetModels()
    {
        return this->models;
    }
    
private:
    // model data 
    vector<Model> models;
    
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        string name; 
        vector<Mesh>    meshes;
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            name = node->mName.C_Str();
            meshes.push_back(processMesh(mesh, scene));

        }
        models.push_back(Model(name, meshes));
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        Material materials;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        //aiColor3D color;
        aiColor3D aiAmbient;
        aiColor3D aiDiffuse;
        aiColor3D aiSpecular;

        material->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient);
        materials.ambient = glm::vec3(aiAmbient.r, aiAmbient.g, aiAmbient.b);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
        materials.diffuse = glm::vec3(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);

        material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);
        materials.specular = glm::vec3(aiSpecular.r, aiSpecular.g, aiSpecular.b);

        material->Get(AI_MATKEY_SHININESS,materials.shininess);
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, materials);
    }
};

#endif