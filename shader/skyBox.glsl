#version 330 core

#ifdef VERTEX_SHADER

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
	mat4 nview = mat4(mat3(view));
    gl_Position = projection * nview * vec4(aPos, 1.0);
}  
#endif


#ifdef FRAGMENT_SHADER

in vec3 TexCoords;
out vec4 fragment_color;
uniform samplerCube skybox;

void main()
{    
    fragment_color = texture(skybox, TexCoords);

}

#endif


