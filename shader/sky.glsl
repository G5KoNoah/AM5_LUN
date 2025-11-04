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
uniform float time;
out vec4 fragment_color;

float hash(vec3 p) {
    p = fract(p * 0.3183099 + vec3(0.1, 0.2, 0.3));
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

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
    }else{
    // --- Jour / nuit ---
    float brightness = clamp(-sunDir.y , 0.0, 1.0); 
    // 0 = jour, 1 = nuit

    // --- Étoiles ---
    float star = hash(floor(dir * 100.0)); // densité (100.0 = espacé);
	float phase = hash(floor(dir * 100.0) + vec3(1.0,0.0,0.0)); // génère un décalage unique pour chaque étoile
	float blink = 0.5 + 0.5 * cos(time * 5.0 + phase * 6.28); // 6.28 = 2π
    float starsVisible = smoothstep(0.995, 1.0, star) * brightness * blink;

    vec3 skyColor = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 0.5), brightness);
    skyColor += vec3(1.0, 1.0, 0.8) * starsVisible ;
    fragment_color = vec4(skyColor, 1.0);
	}

}
#endif


