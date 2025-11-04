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

uniform vec3 sun_dir; // direction normalisée du soleil
out vec4 fragment_color;

void main() {
    // Direction du fragment depuis le centre du cube
    vec3 dir = normalize(vPos);
    vec3 sunDir = normalize(sun_dir);
    vec3 moonDir = -sunDir; // direction opposée = lune

    // cos(angle) = alignement entre la direction du fragment et du soleil/lune
    float cosSun = dot(dir, sunDir);
    float cosMoon = dot(dir, moonDir);

    // Taille apparente des disques
    float sunSize = 0.9995;
    float moonSize = 0.9995;

    // Couleur de base du ciel
    vec3 skyColor = vec3(0.0, 0.0, 1.0); // bleu
    vec3 nightColor = vec3(0.0, 0.0, 0.5); // bleu fonce

    // Soleil : disque jaune
    if (cosSun > sunSize) {
        fragment_color = vec4(1.0, 1.0, 0.0, 1.0);
    }
    // Lune : disque blanc (ou gris clair)
    else if (cosMoon > moonSize) {
        fragment_color = vec4(0.9, 0.9, 1.0, 1.0);
    }
    // Ciel par défaut
    else if(sunDir.y > 0) {
        fragment_color = vec4(skyColor, 1.0);
    }else  {
        fragment_color = vec4(nightColor, 1.0);
    }

}
#endif


