#include "BruitPerlin.h"

// Classe contenant les fonctions pour le bruit de Perlin

float BruitPerlin::hash(float x, float y, float seed) {
    return std::fmod(std::sin(x * 127.1f + y * 311.7f) * 43758.5453f * seed, 1.0f);
}

void BruitPerlin::randomGradient(int ix, int iy, float& gx, float& gy) {
	float h = hash((float)ix, (float)iy, 411.1547f); // Changer la graine pour differentes variations
    float angle = h * 2.0f * 3.14159265f; // 2π
    gx = std::cos(angle);
    gy = std::sin(angle);
}

float BruitPerlin::fade(float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float BruitPerlin::gradDot(int ix, int iy, float x, float y) {
    float gx, gy;
    randomGradient(ix, iy, gx, gy);
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return gx * dx + gy * dy;
}

float BruitPerlin::perlinNoise(float x, float y) {
    // Coins de la cellule
    int x0 = (int)std::floor(x);
    int x1 = x0 + 1;
    int y0 = (int)std::floor(y);
    int y1 = y0 + 1;

    // Contributions des 4 coins
    float n00 = gradDot(x0, y0, x, y);
    float n10 = gradDot(x1, y0, x, y);
    float n01 = gradDot(x0, y1, x, y);
    float n11 = gradDot(x1, y1, x, y);

    // Coordonnées locales dans la cellule
    float fx = x - (float)x0;
    float fy = y - (float)y0;

    // Poids lissés
    float u = fade(fx);
    float v = fade(fy);

    // Interpolation bilinéaire lissée
    float nx0 = n00 + u * (n10 - n00);
    float nx1 = n01 + u * (n11 - n01);
    float nxy = nx0 + v * (nx1 - nx0);

    return nxy; // Valeur typiquement dans [-1,1]
}

float BruitPerlin::fbm(float x, float y) {
    float value = 0.0f;
    //modifier amplitude et frequency pour ajuster le résultat
    float amplitude = 0.75f;
    float frequency = 1.0f;

    // Remplacer le nombre d'octaves souhaité
    for (int i = 0; i < 6; i++) {
        value += amplitude * perlinNoise(x * frequency, y * frequency);

        amplitude *= 0.5f; //valeur à modifier pour ajuster le résultat
        frequency *= 2.0f; //valeur à modifier pour ajuster le résultat
    }
    return value;
}