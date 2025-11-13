//! \file tuto9_color.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;

uniform mat4 mvpMatrix;
uniform mat4 ViewMatrix;

uniform vec3 center_worldspace;
uniform vec2 BillboardSize;
out vec2 vertex_texcoord;
void main( )
{
	vec3 CameraRight_worldspace = vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	vec3 CameraUp_worldspace = vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	vec3 vertexPosition_worldspace = center_worldspace + CameraRight_worldspace * position.x * BillboardSize.x + CameraUp_worldspace * position.y * BillboardSize.y;
	gl_Position= mvpMatrix * vec4(vertexPosition_worldspace, 1.0);

    vertex_texcoord= texcoord;

}

#endif

#ifdef FRAGMENT_SHADER
out vec4 fragment_color;
in vec2 vertex_texcoord;

uniform float time;
uniform vec2 windDir;

// Génération du bruit 2D
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    vec2 u = f*f*(3.0-2.0*f);
    return mix(a, b, u.x) +
           (c - a)*u.y*(1.0 - u.x) +
           (d - b)*u.x*u.y;
}

float fbm(vec2 p)
{
    float f = 0.0;
    f += 0.5  * noise(p); p *= 2.02;
    f += 0.25 * noise(p); p *= 2.03;
    f += 0.125 * noise(p); p *= 2.01;
    f += 0.0625 * noise(p); p *= 2.02;
    f += 0.03125 * noise(p);
    return f;
}

void main()
{
    // Mouvement du vent sur les coordonnées
    vec2 movingUV = vertex_texcoord * 4.0 ;

    // Génère le bruit "nuage"
	float n = fbm(movingUV);

    // Forme du nuage : seuil doux pour créer les contours
    float density = smoothstep(0.45, 0.7, n);

    // Couleur du nuage (blanc/gris)
    vec3 cloudColor = mix(vec3(0.8, 0.8, 0.85), vec3(1.0, 1.0, 1.0), density);

    // Alpha selon la densité → bords doux
    float alpha = density * 0.9;

    fragment_color = vec4(cloudColor, alpha);
}
#endif
