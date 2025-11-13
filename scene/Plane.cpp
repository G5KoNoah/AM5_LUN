#include "Plane.h"

<<<<<<< HEAD
Plane::Plane(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = makePlane(100, 100);
}

Plane::Plane(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = makePlane(100, 100);
}

Plane::Plane(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	mesh = makePlane(100, 100);
}


Mesh Plane::makePlane(int width, int height)
{
    Mesh p_mesh = Mesh(GL_TRIANGLES);

    // 1) GÈnÈrer tous les sommets
    for (int j = 0; j <= height; j++)
    {
        for (int i = 0; i <= width; i++)
        {
            // coordonnÈes dans [0,1], tu peux centrer si tu veux
            float x = float(i);
            float z = float(j);

            // uv
            p_mesh.texcoord(vec2(float(i) / width, float(j) / height));

            // normale (vers le haut)
            p_mesh.normal(vec3(0, 1, 0));
            // position
            p_mesh.vertex(Point(x, 0.0f, z));
=======
// Classe representant un plane

Plane::Plane(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p) {
    mesh = make_plane(10.0f, 100);

}

Plane::Plane(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p) {
    mesh = make_plane(10.0f, 100);
}

Plane::Plane(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p) {
    mesh = make_plane(10.0f, 100);
}

Mesh Plane::make_plane(float width, int subdivisions)
{
    Mesh mesh = Mesh(GL_TRIANGLES);

    int vertex_count = subdivisions + 1;
    float half_width = width / 2.0f;
    float step = width / subdivisions;

    // GÈnÈration des sommets
    for (int z = 0; z < vertex_count; ++z)
    {
        for (int x = 0; x < vertex_count; ++x)
        {
            float px = -half_width + x * step;
            float pz = -half_width + z * step;

            mesh.color(0., 1., 0.);           // Couleur
            mesh.vertex(px, 0, pz);           // Position
            mesh.normal(0, 1, 0);             // Normale vers le haut
>>>>>>> origin/sc√®ne

        }
    }

<<<<<<< HEAD
    // 2) GÈnÈrer les triangles (2 par carrÈ)
    // attention : (width+1) sommets par ligne
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int row1 = j * (width + 1);
            int row2 = (j + 1) * (width + 1);

            // 2 triangles pour chaque quad
            p_mesh.triangle(row1 + i, row1 + i + 1, row2 + i + 1);
            p_mesh.triangle(row1 + i, row2 + i + 1, row2 + i);
        }
    }

    return p_mesh;
}
=======
    // GÈnÈration des triangles
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

>>>>>>> origin/sc√®ne
