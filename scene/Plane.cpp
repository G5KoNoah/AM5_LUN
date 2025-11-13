#include "Plane.h"

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

    // 1) Générer tous les sommets
    for (int j = 0; j <= height; j++)
    {
        for (int i = 0; i <= width; i++)
        {
            // coordonnées dans [0,1], tu peux centrer si tu veux
            float x = float(i);
            float z = float(j);

            // uv
            p_mesh.texcoord(vec2(float(i) / width, float(j) / height));

            // normale (vers le haut)
            p_mesh.normal(vec3(0, 1, 0));
            // position
            p_mesh.vertex(Point(x, 0.0f, z));

        }
    }

    // 2) Générer les triangles (2 par carré)
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