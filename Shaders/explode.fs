#version 330 core
out vec4 FragColor;

struct Light {
    vec3 direction; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;    
uniform float shininess;
uniform Light light;
uniform samplerCube skybox;
uniform int reflectOrRefrect;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 R;
    vec3 color = vec3(0.0);
    vec3 I = normalize(FragPos - viewPos);
    if (reflectOrRefrect == 0)
    {
        // reflect
        R = reflect(I, normalize(Normal));
        color = texture(skybox, R).rgb;
    }
    else if (reflectOrRefrect == 1)
    {
        // refract 
         R = refract(I, normalize(Normal), ratio);
         color = texture(skybox, R).rgb;
    }
    else
    {
        // ambient
        vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;  

        color = ambient + diffuse + specular;
    }
    

    FragColor = vec4(color, 1.0);
    
}