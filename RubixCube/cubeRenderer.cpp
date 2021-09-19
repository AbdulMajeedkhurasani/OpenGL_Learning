#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include "include/Utility/Shader.h"
#include "include/Utility/rubixCube.h"
#include "include/Utility/camera.h"
#include "include/Utility/quaternion.h"
#include "include/Utility/mouse.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow* initialize();


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



int main()
{
    GLFWwindow* window = initialize();
    if (window == nullptr)
        return -1;
    
    glEnable(GL_DEPTH_TEST);

    Shader shader("include/Shaders/cubie.vs", "include/Shaders/cubie.fs");
    Shader shaderVisNormals("include/Shaders/visNormals.vs", "include/Shaders/visNormals.gs","include/Shaders/visNormals.fs");
    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    glm::mat4 view(1.0f);
    glm::vec3 viewPos = glm::vec3(0.0,0.0,-13.0);
    view = glm::translate(view, viewPos);

    shader.setVec3("viewPos", viewPos);
    shader.setVec3("light.position", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.setVec3("light.ambient", glm::vec3(0.1));
    shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
    shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    Mouse mouse = Mouse();
    
    RubixCube rubixCube = RubixCube();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        
        glClearColor(0.3f, 0.3f, 1.0f, 1.0f); //background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Quat quaterinon = Quat();
        view *= quaterinon.QuatRotMatrix4f(mouse.rotaionVector(window));
        shader.use();
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
                rubixCube.Draw(shader, view, R);
        }
        else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
                rubixCube.Draw(shader, view, L);
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE)
                rubixCube.Draw(shader, view, U);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
                rubixCube.Draw(shader, view, D);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
                rubixCube.Draw(shader, view, F);
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)     
                rubixCube.Draw(shader, view, B);
        }
        else
            rubixCube.Draw(shader, view);
        
        
        


/*/////////////
        shaderVisNormals.use();
        shaderVisNormals.setMat4("projection", projection);
        rubixCube.Draw(shaderVisNormals, view);
//////////////////////////*/

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "name", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
    }
    return window;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
