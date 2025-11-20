//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER

in vec2 position;

out vec4 clipSpace;

uniform vec4 mvpMatrix;

void main( )
{
    clipSpace = mvpMatrix * vec4(position.x, 0.0, position.y, 1.0);
    gl_Position = clipSpace;
}

#endif


#ifdef FRAGMENT_SHADER

in vec4 clipSpace;

in vec2 texCoord;

out vec4 color;

uniform sampler2D reflextionTexture;
uniform sampler2D refractionTexture;

{
    vec2 ndc = (clipSpace.xy / clipSpace.w)/2,0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    vec4 reflectColor = texture(reflectionTexture, textureCoords);
    vec4 refractColor = texture(refractionTexture, textureCoords);

    color = mix(reflectColor,refractColor,0.5);

}

#endif
