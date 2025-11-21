#ifndef _BRUITPERLIN
#define _BRUITPERLIN

#include "math.h"
#include <iostream>

/*
    @class BruitPerlin
	Classe contenant les fonctions pour le bruit de Perlin
*/

class BruitPerlin{

public:
	static float hash(float x, float y, float seed);
	static void randomGradient(int ix, int iy, float& gx, float& gy);
	static float fade(float t);
	static float gradDot(int ix, int iy, float x, float y);
	static float perlinNoise(float x, float y);
	static float fbm(float x, float y);
};

#endif