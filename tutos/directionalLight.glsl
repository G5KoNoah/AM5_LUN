//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout (location = 2) in vec3 aNormal;


uniform mat4 mvpMatrix;
uniform mat4 model;
out vec3 Normal;
out vec3 FragPos;
out vec2 vertex_texcoord;

void main( )
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vertex_texcoord= texcoord;
}

#endif







#ifdef FRAGMENT_SHADER

in vec3 Normal;
in vec3 FragPos;
out vec4 fragment_color; 
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material material;


struct Light {
    //vec3 position;
    vec3 direction;
 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

in vec2 vertex_texcoord;


void main( )
{


    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertex_texcoord));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertex_texcoord));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertex_texcoord));
        
    vec3 result = (ambient + diffuse + specular);
    fragment_color = vec4(result, 1.0);

}

#endif
