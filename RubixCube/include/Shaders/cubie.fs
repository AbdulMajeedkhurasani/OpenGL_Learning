#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform Light light;
uniform vec3 color;
uniform vec3 viewPos;

vec3 alight(vec3 position)
{
    // ambient
    vec3 ambient = light.ambient + material.ambient;

    /// diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * (material.specular)) ;

    return (specular + diffuse );
}

void main()
{   vec3 light1 = alight(vec3( 10.0, 0.0,-13.0));
    vec3 light2 = alight(vec3( 0.0,-10.0,-13.0));
    vec3 light3 = alight(vec3(-10.0, 0.0,-13.0));
    vec3 light4 = alight(vec3(0.0,10.0,-13.0));
    vec3 light5 = alight(vec3(0.0,0.0,0.0));
    vec3 result  = (light1 + light2 + light3 + light4 + light5) ;
    FragColor = vec4(result, 1.0f);
}

