#version 330
#define NR_POINT_LIGHTS 4

#ifdef VERTEX_SHADER
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 aNormal;

uniform mat4 mvpMatrix;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 vertex_texcoord;
out vec3 pos;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vertex_texcoord = texcoord;
    pos = position;
}
#endif


#ifdef FRAGMENT_SHADER

in vec3 Normal;
in vec3 FragPos;
in vec2 vertex_texcoord;
in vec3 pos;
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

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Fonction noise
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = smoothstep(0.0, 1.0, f);
    
    return mix(
        mix(a, b, u.x),
        mix(c, d, u.x),
        u.y
    );
}

float fbm(vec2 st, int octaves) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for(int i = 0; i < 10; i ++ ) {
        if (i >= octaves)break;
        
        value += amplitude * noise(st * frequency);
        
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return value;
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
    return (ambient + diffuse + specular);
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

    return (ambient + diffuse + specular);
}

void main()
{
    vec4 tex = texture(material.diffuse, vertex_texcoord); // sample once (r,g,b,a)

    float alpha = tex.a;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < nbPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    
    float t = max(min((pos.y - 2) + (random(vec2(pos.x, pos.z))/5), 0.8), 0.0);
    result = t * vec3(0.9,0.9,0.9) + (1-t) * result;
    FragColor = vec4(result, alpha);
}
#endif
