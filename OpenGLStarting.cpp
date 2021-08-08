
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
// incase of resizing window glViewPort should resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow* window);

// vertex shader just passes input to out put 
const char *vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";
const char *fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(ourColor, 1.0);\n"
        "}\0";



int main()
{
    // all for opening a window can be done with other libraries
    glfwInit(); // initialize glfw 
    // set version and make it core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 800;
    const int height = 600;
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

    //Initialize glad. Glad manages function pointers for openGl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
    }

    
    // register resize function 
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    



    
    //create a shader object 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Compiling shader 
    // shader id, number of strings, shader source . binding shader to shader ogject
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //compile shader 
    glCompileShader(vertexShader);
    // check compile time error 
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"  << infoLog << "\n";
    }
    // do same for fragment shader 
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // compile time error
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"  << infoLog << "\n";
    }

    // creat a shader program to link them with shaders and have its id reference
    // uGLuint is an unsigned int 
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors 
    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::LINKING::COMPILATION_FAILED\n"  << infoLog << "\n";
    }
    // once shader are linked there objects are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

       // render stuff 
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //Input event 
        glfwPollEvents();
        //will swap the color buffer
        glfwSwapBuffers(window);
    }


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