//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;

out vec3 fragPos;
uniform mat4 mvpMatrix;
uniform float time;

void main( )
{

    float A = 0.7; //Amplitude
    float L = 4.; //Longueur de la vague
    float w = 2/L;
    float S = 2.; //Vitesse de la vague
    float phi = S*(2/L);
    vec3 D1 = vec3(1.,0.,0.); //Direction des vagues
    vec3 D2 = vec3(1.,0.,0.8);

    vec3 pos = position;
    vec3 vagSum = vec3(0.,0.,0.); // Somme de toutes les vagues

	//pos.y += 1. * (1+sin(pos.x*0.5 + time)) * (1+ cos(pos.z*0.5 + time)) + (0.2 * (1+sin(pos.x*0.5 + (time*1.2))) * (1+ cos(pos.z*0.5 + (time*1.2))));
    
    vagSum += A * sin(dot(D1,vec3(pos.x,pos.y,pos.z)*w + time*phi));
    vagSum += 0.5 * A * sin(dot(D2,vec3(pos.x,pos.y,pos.z)*w + 1.2*time*phi));

    pos += vagSum;

    fragPos = pos;
    gl_Position = mvpMatrix * vec4(pos, 1.0);
}

#endif


#ifdef FRAGMENT_SHADER

in vec3 fragPos;
out vec4 fragment_color;

uniform vec4 material_color; // couleur de base
uniform float time;

void main( )
{
    // créer une variation de couleur en fonction du temps
    float r = 0.;
    float g =  0.;
    float b = 1;
    float a = 0.7 + 0.5 * fragPos.y;

    // tu peux aussi multiplier par material_color si tu veux

    //fragment_color = vec4(r, g, a, 1); //N'utilise pas la transparance entre guillemets
    fragment_color = vec4(r, g, b, a); //Utilise la transparence entre guillemets

}

#endif
