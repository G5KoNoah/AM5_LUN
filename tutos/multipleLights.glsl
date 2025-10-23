#version 330
#define NR_POINT_LIGHTS 4

#ifdef VERTEX_SHADER
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 aNormal;

uniform mat4 mvpMatrix;
uniform mat4 model;
//uniform mat4 lightSpaceMatrix; //Nouveau

out vec3 Normal;
out vec3 FragPos;
out vec2 vertex_texcoord;
//out vec4 FragPosLightSpace; //Nouveau

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vertex_texcoord = texcoord;
    //FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
#endif


#ifdef FRAGMENT_SHADER

in vec3 Normal;
in vec3 FragPos;
in vec2 vertex_texcoord;
//in vec4 FragPosLightSpace; //Nouveau
out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int nbPointLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];


float ShadowCalculation(vec4 fragPosLightSpace) //Nouveau
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vertex_texcoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vertex_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertex_texcoord));
//    float shadow = ShadowCalculation(FragPosLightSpace); //Nouveau
    return (ambient + diffuse + specular /* (1.0 - shadow) * (diffuse + specular) */); //Modif
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vertex_texcoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vertex_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertex_texcoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

//    float shadow = ShadowCalculation(FragPosLightSpace); //Nouveau
    return (ambient + diffuse + specular /* (1.0 - shadow) * (diffuse + specular) */); //Modif
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < nbPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}
#endif
