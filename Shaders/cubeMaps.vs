#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;    
    // giving skybox a depth of 1.0 always
    gl_Position = (projection * view * vec4(aPos, 1.0)).xyww;
}