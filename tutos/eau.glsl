//! \file tuto9_groups.glsl

#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;

out vec3 fragPos;
uniform mat4 mvpMatrix;
uniform float time;

void main( )
{
    float A[5]; //Amplitude
    A[0] = 1.;
    A[1] = 0.5;
    A[2] = 0.2;
    A[3] = 0.4;
    A[4] = 0.7;

    float L[5]; //Longueur de la vague
    L[0] = 5.;
    L[1] = 4.;
    L[2] = 3.;
    L[3] = 2.;
    L[4] = 1.;

    float w[5];
    for(int i = 0; i < 5; i++){
        w[i] = 2/L[i];
    }

    float S[5]; //Vitesse de la vague
    S[0] = 8.;
    S[1] = 6.;
    S[2] = 4.;
    S[3] = 2.;
    S[4] = 2.;
    
    float phi[5];
    for(int i = 0; i < 5; i++){
        phi[i] = S[i]*(2/L[i]);
    }

    //float Q = 1/(w*A); //Steepness

    vec3 D[5]; //Direction des vagues
    D[0] = vec3(1.,0.,0.);
    D[1] = vec3(1.,0.,0.2);
    D[2] = vec3(1.,0.,0.4);
    D[3] = vec3(1.,0.,-0.2);
    D[4] = vec3(1.,0.,-0.4);

    vec3 pos = position;
    vec3 vagSum = vec3(0.,0.,0.); // Somme de toutes les vagues

	//pos.y += 1. * (1+sin(pos.x*0.5 + time)) * (1+ cos(pos.z*0.5 + time)) + (0.2 * (1+sin(pos.x*0.5 + (time*1.2))) * (1+ cos(pos.z*0.5 + (time*1.2))));
    
    //vagSum += A * sin(dot(D1,vec3(pos.x,pos.y,pos.z)*w + time*phi));
    //vagSum += 0.5 * A * sin(dot(D2,vec3(pos.x,pos.y,pos.z)*w + 1.2*time*phi));

    for(int i = 0; i < 5; i++){
        vagSum += 2*A[i] * pow(sin(dot(D[i],vec3(pos.x,pos.y,pos.z)*w[i] + time*phi[i]))/2,2);
    }

    //Gerstner waves

    //pos.x = pos.x + (Q*A * D1.x * cos(dot(w*D1,vec3(pos.x,pos.y,pos.z)) + phi*time));
    //pos.z = pos.z + (Q*A * D1.z * cos(dot(w*D1,vec3(pos.x,pos.y,pos.z)) + phi*time));
    //pos.y = A * sin(dot(w*D1,vec3(pos.x,pos.y,pos.z) + time*phi));

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
    float a = 0.5 + 0.5 * fragPos.y;

    // tu peux aussi multiplier par material_color si tu veux

    //fragment_color = vec4(r, g, a, 1); //N'utilise pas la transparance entre guillemets
    fragment_color = vec4(r, g, b, a); //Utilise la transparence entre guillemets

}

#endif
