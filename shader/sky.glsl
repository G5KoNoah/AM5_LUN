#version 330 core

#ifdef VERTEX_SHADER
layout(location=0) in vec3 position;

uniform mat4 mvpMatrix;
out vec3 vPos;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
    vPos = position; // Position du fragment dans le cube
}
#endif


#ifdef FRAGMENT_SHADER
in vec3 vPos;

uniform vec3 sun_pos; // direction du soleil (normalisée)
out vec4 fragment_color;

void main() {
    // Direction du fragment depuis le centre du cube
    vec3 dir = normalize(vPos);
    vec3 sunDir = normalize(sun_pos);

    // Produit scalaire = cos(angle) entre les deux directions
    float cosTheta = dot(dir, sunDir);

    // Plus cosTheta est proche de 1, plus on est aligné avec le soleil
    // Ajuste la "taille" du soleil avec ce seuil :
    float sunSize = 0.9995; // proche de 1 = petit soleil, plus bas = plus gros

    if (cosTheta > sunSize) {
        fragment_color = vec4(1.0, 1.0, 0.0, 1.0); // Soleil jaune
    } else {
        fragment_color = vec4(0.0, 0.0, 1.0, 1.0); // Ciel bleu
    }
}
#endif

