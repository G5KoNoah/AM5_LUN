//! \file arbre.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location = 3) in uint material;

uniform mat4 mvpMatrix;

out vec2 vertex_texcoord;
flat out uint v_material;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    vertex_texcoord= texcoord;
    v_material = material;
}

#endif


#ifdef FRAGMENT_SHADER
out vec4 fragment_color;

in vec2 vertex_texcoord;    
flat in uint v_material;

uniform vec4 materials[16];

void main( )
{
    vec4 matColor = materials[v_material];
    fragment_color = matColor; 
}

#endif
