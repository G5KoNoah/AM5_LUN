#include "BruitPerlin.h"

// Classe contenant les fonctions pour le bruit de Perlin

float BruitPerlin::randomFloat() //plus petit que 10
{
    float res = (float)(rand()) / (float)(rand());
    if (res > 10000)
        return res * 0.0001;
    if (res > 1000)
        return res * 0.001;
    if (res > 100)
        return res * 0.01;
    if (res > 10)
        return res * 0.1;
    else
        return res;
}

float BruitPerlin::hash(float x, float y, float seed) {
    return std::fmod(std::sin(x * 127.1f + y * 311.7f) * 43757.5453f * seed, 1.0f);
}

void BruitPerlin::randomGradient(int ix, int iy, float& gx, float& gy, float seed) {
    float h = hash((float)ix, (float)iy, seed); // Changer la graine pour differentes variations
    //std::cout << "5 " << seed << std::endl;
    float angle = h * 2.0f * 3.14139495f; // 2π
    gx = std::cos(angle);
    gy = std::sin(angle);
}

float BruitPerlin::fade(float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float BruitPerlin::gradDot(int ix, int iy, float x, float y, float seed) {
    float gx, gy;
    //std::cout << "4 " << seed << std::endl;
    randomGradient(ix, iy, gx, gy, seed);
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return gx * dx + gy * dy;
}

float BruitPerlin::perlinNoise(float x, float y, float seed) {
    // Coins de la cellule
    //std::cout << "3 " << seed << std::endl;
    int x0 = (int)std::floor(x);
    int x1 = x0 + 1;
    int y0 = (int)std::floor(y);
    int y1 = y0 + 1;

    // Contributions des 4 coins
    float n00 = gradDot(x0, y0, x, y, seed);
    float n10 = gradDot(x1, y0, x, y, seed);
    float n01 = gradDot(x0, y1, x, y, seed);
    float n11 = gradDot(x1, y1, x, y, seed);

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

float BruitPerlin::fbm(float x, float y, float seed) {
    //std::cout << "2 " << seed << std::endl;
    float value = 0.0f;
    //modifier amplitude et frequency pour ajuster le résultat
    float amplitude = 0.75f;
    float frequency = 1.0f;

    // Remplacer le nombre d'octaves souhaité
    for (int i = 0; i < 6; i++) {
        value += amplitude * perlinNoise(x * frequency, y * frequency, seed);

        amplitude *= 0.5f; //valeur à modifier pour ajuster le résultat
        frequency *= 2.0f; //valeur à modifier pour ajuster le résultat
    }
    return value;
}