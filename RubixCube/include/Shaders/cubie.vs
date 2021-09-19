#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //Normal = mat3(transpose(inverse(view * model))) * aNormal;
    Normal = mat3(transpose(inverse(view))) * aNormal;
    FragPos = vec3(view * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(aPos, 1.0);
}