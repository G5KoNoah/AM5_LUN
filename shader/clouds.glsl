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


 out vec4 fragment_color;
  uniform float time = 0.0;
  uniform float cirrus = 0.4;
  uniform float cumulus = 0.6;
  uniform vec3 sun_dir;

  const float Br = 0.0025;
  const float Bm = 0.0003;
  const float g =  0.9800;
  const vec3 nitrogen = vec3(0.650, 0.570, 0.475);
  const vec3 Kr = Br / pow(nitrogen, vec3(4.0));
  const vec3 Km = Bm / pow(nitrogen, vec3(0.84));

  float hash(float n)
  {
    return fract(sin(n) * 43758.5453123);
  }

  float hashStars(vec3 p) {
    p = fract(p * 0.3183099 + vec3(0.1, 0.2, 0.3));
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
  }

  float noise(vec3 x)
  {
    vec3 f = fract(x);
    float n = dot(floor(x), vec3(1.0, 157.0, 113.0));
    return mix(mix(mix(hash(n +   0.0), hash(n +   1.0), f.x),
                   mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y),
               mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
                   mix(hash(n + 270.0), hash(n + 271.0), f.x), f.y), f.z);
  }

  const mat3 m = mat3(0.0, 1.60,  1.20, -1.6, 0.72, -0.96, -1.2, -0.96, 1.28);
  float fbm(vec3 p)
  {
    float f = 0.0;
    f += noise(p) / 2; p = m * p * 1.1;
    f += noise(p) / 4; p = m * p * 1.2;
    f += noise(p) / 6; p = m * p * 1.3;
    f += noise(p) / 12; p = m * p * 1.4;
    f += noise(p) / 24;
    return f;
  }

  void main()
  {
  	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    vec3 dir = normalize(vPos);
    vec3 moonDir = normalize(sun_dir);
    vec3 fsun = vec3(-moonDir.x, -moonDir.y, moonDir.z);
    //vec3 fsun = vec3(0.0, sin(time * 0.01), cos(time * 0.01));

    if (vPos.y < 0)
      color = vec4(0.0, 0.0, 0.0, 1.0);

    // Atmosphere Scattering
    float mu = dot(normalize(vPos), normalize(fsun));
    float rayleigh = 3.0 / (8.0 * 3.14) * (1.0 + mu * mu);
    vec3 mie = (Kr + Km * (1.0 - g * g) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5)) / (Br + Bm);

    vec3 day_extinction = exp(-exp(-((vPos.y + fsun.y * 4.0) * (exp(-vPos.y * 16.0) + 0.1) / 80.0) / Br) * (exp(-vPos.y * 16.0) + 0.1) * Kr / Br) * exp(-vPos.y * exp(-vPos.y * 8.0 ) * 4.0) * exp(-vPos.y * 2.0) * 4.0;
    vec3 night_extinction = vec3(1.0 - exp(fsun.y)) * 0.2;
    vec3 extinction = mix(day_extinction, night_extinction, -fsun.y * 0.2 + 0.5);

    vec3 scattering = rayleigh * mie * extinction;
    scattering = clamp(scattering, 0.0, 1.0);   
    color.rgb = scattering * 0.7;             


    float cosSun = dot(dir, fsun);
    float cosMoon = dot(dir, moonDir);

    // --- Paramètres apparents ---
    float sunSize = 0.995;  // plus proche de 1 = plus petit disque
    float moonSize = 0.995;

    vec3 sunColor = vec3(1.0, 0.9, 0.6); // jaune chaud
    vec3 moonColor = vec3(0.9, 0.9, 1.0); // blanc bleuté

        // --- Soleil : disque + halo ---
    float halo = smoothstep(sunSize - 0.002, 1.0, cosSun); // dégradé doux
    color.rgb = mix(color.rgb, sunColor, halo);              // mélange progressif
    color.rgb += sunColor * pow(halo, 100.0);               // cœur très brillant

    float moonCore = smoothstep(moonSize, 1.0, cosMoon); 
    color.rgb = mix(color.rgb, moonColor, pow(moonCore, 10.0)); 

    // --- Étoiles ---
    float brightness = clamp(-fsun.y, 0.0, 1.0); // 0 = jour, 1 = nuit
    float star = hashStars(floor(dir * 100.0)); // densité
    float phase = hashStars(floor(dir * 100.0) + vec3(1.0, 0.0, 0.0));
    float blink = 0.5 + 0.5 * cos(time * 5.0 + phase * 6.2831);
    float starsVisible = smoothstep(0.998, 1.0, star) * brightness * blink;

    // --- Ajout des étoiles ---
    color.rgb += vec3(1.0, 1.0, 0.8) * starsVisible;

    // Cirrus Clouds
    float density = smoothstep(1.0 - cirrus, 1.0, fbm(vPos.xyz / vPos.y * 2.0 + time * 0.05)) * 0.3;
    color.rgb = mix(color.rgb, extinction * 4.0, density * max(vPos.y, 0.0));

    // Cumulus Clouds
    for (int i = 0; i < 10; i++)
    {
      float density = smoothstep(1.0 - cumulus, 1.0, fbm((0.7 + float(i) * 0.01) * vPos.xyz / vPos.y + time * 0.3));
      color.rgb = mix(color.rgb, extinction * density * 2.0, min(density, 1.0) * max(vPos.y, 0.0));
    }

    // Dithering Noise
    color.rgb += noise(vPos * 1000) * 0.01;
    fragment_color = color; // bleu ciel fixe
  }

#endif


