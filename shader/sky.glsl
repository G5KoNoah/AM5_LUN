#version 330 core

#ifdef VERTEX_SHADER
layout(location=0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
out vec3 vPos;

void main() {
	mat4 nview = mat4(mat3(view));
    gl_Position = projection * nview * vec4(position, 1.0);
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
    // --- Directions ---
    vec3 dir = normalize(vPos);
    vec3 moonDir = normalize(sun_dir);
    vec3 sunDir = vec3(-moonDir.x, -moonDir.y, moonDir.z);

    // --- Alignement Soleil / Lune ---
    float cosSun = dot(dir, sunDir);
    float cosMoon = dot(dir, moonDir);

    // --- Paramètres apparents ---
    float sunSize = 0.995;  // plus proche de 1 = plus petit disque
    float moonSize = 0.995;


    // --- Couleurs de base ---
    vec3 daySky   = vec3(0.2, 0.5, 1.0); // bleu ciel
    vec3 nightSky = vec3(0.0, 0.0, 0.3); // bleu foncé
    vec3 sunColor = vec3(1.0, 0.9, 0.6); // jaune chaud
    vec3 moonColor = vec3(0.9, 0.9, 1.0); // blanc bleuté

    // --- Jour / nuit ---
    float brightness = clamp(-sunDir.y, 0.0, 1.0); // 0 = jour, 1 = nuit
    vec3 skyColor = mix(daySky, nightSky, brightness);

    // --- Soleil : disque + halo ---
    float halo = smoothstep(sunSize - 0.002, 1.0, cosSun); // dégradé doux
    skyColor = mix(skyColor, sunColor, halo);              // mélange progressif
    skyColor += sunColor * pow(halo, 100.0);               // cœur très brillant



    float moonCore = smoothstep(moonSize, 1.0, cosMoon); 
    skyColor = mix(skyColor, moonColor, pow(moonCore, 10.0)); 

    // --- Étoiles ---
    float star = hash(floor(dir * 100.0)); // densité
    float phase = hash(floor(dir * 100.0) + vec3(1.0, 0.0, 0.0));
    float blink = 0.5 + 0.5 * cos(time * 5.0 + phase * 6.2831);
    float starsVisible = smoothstep(0.998, 1.0, star) * brightness * blink;

    // --- Ajout des étoiles ---
    skyColor += vec3(1.0, 1.0, 0.8) * starsVisible;

    // --- Sortie finale ---
    fragment_color = vec4(skyColor, 1.0);
}
#endif


