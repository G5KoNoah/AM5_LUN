#version 330 core

#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
}  

#endif



#ifdef FRAGMENT_SHADER


layout(location = 0) out float fragmentdepth;

void main(){
    fragmentdepth = gl_FragCoord.z;
}

#endif