#include "terrain.h"

// Classe representant un cube

Terrain::Terrain(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p) {
    mesh = Mesh(GL_TRIANGLES);

	// Valeurs à modifié pour ajuster le terrain
	float width = 20.0f;
	int subdivisions = 100;
	float height_max = 5.0f;

    int vertex_count = subdivisions + 1;
    float half_width = width / 2.0f;
    float step = width / subdivisions;
    // Génération des sommets
    for (int z = 0; z < vertex_count; ++z)
    {
        for (int x = 0; x < vertex_count; ++x)
        {
            float px = -half_width + x * step;
            float pz = -half_width + z * step;
            float y = BruitPerlin::fbm(px, pz);

            if (y >= height_max - 1)
                mesh.color(1., 1., 1.);
            else {
                mesh.color(0., 1., 0.);           // Couleur
            }
            mesh.vertex(px, y, pz);           // Position
            mesh.normal(0, 1, 0);             // Normale vers le haut

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
}

