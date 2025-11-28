//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER

layout(location=0) in vec3 position;

out vec4 clipSpace;
out vec2 texCoord;

uniform vec4 mvpMatrix;

void main( )
{
    clipSpace = mvpMatrix * vec4(position.x, 0.0, position.y, 1.0);
    gl_Position = clipSpace;
    texCoord = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * 6.0; /*Arbitraire ?*/
}

#endif




#ifdef FRAGMENT_SHADER

in vec4 clipSpace;
in vec2 texCoord;

out vec4 color;

uniform sampler2D reflextionTexture;
uniform sampler2D refractionTexture;

void main( )
{
    //vec2 ndc = (clipSpace.xy / clipSpace.w)/2.0 + 0.5;
    //vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    //vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    //vec4 reflectColor = texture(reflectionTexture, texCoord);
    //vec4 refractColor = texture(refractionTexture, texCoord);

    //color = mix(reflectColor,refractColor,0.5);
    color = vec4(1.0,1.0,0.0,1.0);

}

#endif
