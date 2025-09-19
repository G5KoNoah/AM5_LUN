//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;

out vec3 fragPos;
uniform mat4 mvpMatrix;
uniform float time;

void main( )
{
    vec3 pos = position;
	pos.y += 5. * (1+sin(pos.x*0.01 + time)) * (1+ cos(pos.z*0.01 + time));

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
    float a = 0.5 + 0.5 * fragPos.y;

    // tu peux aussi multiplier par material_color si tu veux
    fragment_color = vec4(r, g, b, a);

}

#endif
