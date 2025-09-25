//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout (location = 2) in vec3 aNormal;


uniform mat4 mvpMatrix;
uniform mat4 model;
out vec3 Normal;
out vec3 FragPos;

void main( )
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

#endif


#ifdef FRAGMENT_SHADER

in vec3 Normal;
in vec3 FragPos;
out vec4 fragment_color;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 color;


void main( )
{
vec3 objectColor = color;

vec3 lightColor= vec3(1.0, 1.0, 1.0);

float specularStrength = 0.5;

float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor ;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos); 

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm); 

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
vec3 specular = specularStrength * spec * lightColor;  

vec3 result = (ambient + diffuse + specular) * objectColor;
    fragment_color = vec4(result,1.0);

}

#endif
