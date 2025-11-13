#include "terrain.h"

// Classe representant un terrain

Terrain::Terrain(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p) {
    mesh = make_terrain(20.0f, 100, 5.0f);

}

Terrain::Terrain(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p) {
    mesh = make_terrain(20.0f, 100, 5.0f);
}

Terrain::Terrain(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p) {
	mesh = make_terrain(20.0f, 100, 5.0f);
}

Mesh Terrain::make_terrain(float width, int subdivisions, float height_max)
{
    Mesh mesh = Mesh(GL_TRIANGLES);
    int vertex_count = subdivisions + 1;
    float half_width = width / 2.0f;
    float step = width / subdivisions;
	float frequency = 0.1f;
    // Génération des sommets
    for (int z = 0; z < vertex_count; ++z)
    {
        for (int x = 0; x < vertex_count; ++x)
        {
            float px = -half_width + x * step;
            float pz = -half_width + z * step;
            //float y = interpolation(0., height_max, ridgedfbm(px, pz));
			float y = 3.0f * BruitPerlin::fbm(px * frequency, pz * frequency) + 2.0f; // Le 3 et le 2 permettent de régler la hauteur 
            //std::cout << ridgedfbm(px, pz) << std::endl;

            float eps = step;
			float Hl = 3.0f * BruitPerlin::fbm((px - eps) * frequency, pz * frequency) + 2.0f;
			float Hr = 3.0f * BruitPerlin::fbm((px + eps) * frequency, pz * frequency) + 2.0f;
			float Hd = 3.0f * BruitPerlin::fbm(px * frequency, (pz - eps) * frequency) + 2.0f;
			float Hu = 3.0f * BruitPerlin::fbm(px * frequency, (pz + eps) * frequency) + 2.0f;

            vec3 normal = normalize(vec3(Hl - Hr, 2.0f * step, Hd - Hu));

            if (y >= height_max - 1)
                mesh.color(1., 1., 1.);
            else {
                mesh.color(0., 1., 0.);           // Couleur
            }
            mesh.texcoord(float(x) / subdivisions, float(z) / subdivisions);
            mesh.vertex(px, y, pz);           // Position
            mesh.normal(normal);             // Normale vers le haut

        }
    }
    // Génération des triangles
    for (int z = 0; z < subdivisions; ++z)
    {
        for (int x = 0; x < subdivisions; ++x)
        {
            int i0 = z * vertex_count + x;
            int i1 = i0 + 1;
            int i2 = i0 + vertex_count;
            int i3 = i2 + 1;
            // Deux triangles par case
            mesh.triangle(i0, i2, i1); // triangle bas-gauche
            mesh.triangle(i1, i2, i3); // triangle haut-droit
        }
    }
    return mesh;
}

float Terrain::getHeight(float x, float z)
{
	float frequency = 0.1f;
    float height = 3.0f * BruitPerlin::fbm(x * frequency, z * frequency) + 2.0f; // Le 3 et le 2 permettent de régler la hauteur 
    return height;
}