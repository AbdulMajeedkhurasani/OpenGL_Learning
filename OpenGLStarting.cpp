
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shaders/Shader.h"

// incase of resizing window glViewPort should resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow* window);

const int width = 800;
const int height = 600;

int main()
{
    // all for opening a window can be done with other libraries
    glfwInit();
    // set version and make it core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // creat a winddow 
    GLFWwindow* window = glfwCreateWindow(width, height, "name", NULL, NULL);
    // Check for window if it is created 
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    //make the context of our window the main context on the current thread
    glfwMakeContextCurrent(window);
    // register resize function 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initialize glad. Glad manages function pointers for openGl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
    }

    // build and compile  shader program
    // ------------------------------------
    Shader shaderProgram("Shaders/verShader330", "Shaders/fregShader330"); 
    
    
    float vertices[] = {
        // vetices            //color
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0  
    };
    unsigned int indices[] = {  
        0, 1, 2
    };

    // vertex buffer objects(VBO) , vertex array object, Element buffer object
    unsigned int VBO, VAO, EBO;  // buffer id 
    // genrate buffers and vertex array
    glGenBuffers(1,&VBO); 
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &EBO);
    //1. bind vertex array object
    glBindVertexArray(VAO);
    //2. copy vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER,VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers for vertex and color
    // give the structure of vertex data to the vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // give the structure of color data to the vertex shader (layout = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    ////// render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // set the state of color
        glClearColor(0.0f,0.3f,1.0f,1.0f);
        // clear screen with set color
        glClear(GL_COLOR_BUFFER_BIT);
        // the triangle

        //offseting triangle using a uniform variable
        float offset = 0.5;
        shaderProgram.setFloat("offset", offset);
        // render stuff 
        shaderProgram.use();
        
        

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //Input event 
        glfwPollEvents();
        //will swap the color buffer
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    // tell openGl what size of window we are working with 
    // x,y,width,height
    // x,y = 0,0  sets lower left corner of widow as origin
    glViewport(0,0,width,height);
} 