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

uniform sampler2D texture0;

uniform vec4 color;

void main( )
{
    vec4 color= texture(texture0, vertex_texcoord);
    fragment_color= color;
}

#endif
