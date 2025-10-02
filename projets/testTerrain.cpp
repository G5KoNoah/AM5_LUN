
//! \file tuto7_camera.cpp reprise de tuto7.cpp mais en derivant AppCamera, avec gestion automatique d'une camera.


#include "mat.h"
#include "wavefront.h"
#include "texture.h"
#include <iostream>

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"        
#include "app_camera.h"        // classe Application a deriver


// utilitaire. creation d'une grille / repere.
Mesh make_grid(const int n = 10)
{
    Mesh grid = Mesh(GL_LINES);

    // grille
    grid.color(White());
    for (int x = 0; x < n; x++)
    {
        float px = float(x) - float(n) / 2 + .5f;
        grid.vertex(Point(px, 0, -float(n) / 2 + .5f));
        grid.vertex(Point(px, 0, float(n) / 2 - .5f));
    }

    for (int z = 0; z < n; z++)
    {
        float pz = float(z) - float(n) / 2 + .5f;
        grid.vertex(Point(-float(n) / 2 + .5f, 0, pz));
        grid.vertex(Point(float(n) / 2 - .5f, 0, pz));
    }

    // axes XYZ
    grid.color(Red());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(1, .1, 0));

    grid.color(Green());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, 1, 0));

    grid.color(Blue());
    grid.vertex(Point(0, .1, 0));
    grid.vertex(Point(0, .1, 1));

    glLineWidth(2);

    return grid;
}

Mesh make_plane(float width, int subdivisions)
{
    Mesh mesh = Mesh(GL_TRIANGLES);

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

			mesh.color(0., 1., 0.);           // Couleur
            mesh.vertex(px, 0, pz);           // Position
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

    return mesh;
}

float dot(const vec2& u, const vec2& v) {
    return u.x * v.x + u.y * v.y;
}

vec2 floor(const vec2& v) {
    return vec2(floor(v.x), floor(v.y));
}

vec2 fract(const vec2& v) {
    return vec2(v.x - floor(v.x), v.y - floor(v.y));
}

float fract(const float v) {
    return v - floor(v);
}

float hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float interpolation(float x, float y, float a) {
	return x * (1 - a) + y * a;
}

float noise(vec2 p) {
	vec2 i = floor(p);
	vec2 f = fract(p);

    float a = hash(i);
	float b = hash(vec2(i.x + 1.0, i.y));
    float c = hash(vec2(i.x, i.y + 1.0));
    float d = hash(vec2(i.x + 1.0, i.y + 1.0));

	vec2 u = vec2(f.x * f.x * (3.0 - 2.0 * f.x), f.y * f.y * (3.0 - 2.0 * f.y));

	return interpolation(a, b , u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

Mesh make_terrain(float width, int subdivisions, float height_max)
{
    Mesh mesh = Mesh(GL_TRIANGLES);
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
            float y = interpolation(0., height_max, noise(vec2(px, pz)));
            
            if (y >= height_max-1)
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
    return mesh;
}


class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : App(1024, 640) {}

    // creation des objets de l'application
    int init()
    {
        // decrire un repere / grille 
        m_repere = make_grid(10);

        // creation d'un plan

		plane = make_plane(2.0f, 10);

		// creation d'un terrain

		terrain = make_terrain(10.0f, 100, 3.0f);

        Point pmin, pmax;
        terrain.bounds(pmin, pmax);
        m_camera.lookat(pmin, pmax);

        m_program = read_program("../tutos/tuto9_textures.glsl");
        program_print_errors(m_program);


        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest


        return 0;   // pas d'erreur, sinon renvoyer -1
    }

    // destruction des objets de l'application
    int quit()
    {
        release_program(m_program);
        
        m_objet.release();
        m_repere.release();
		plane.release();
		terrain.release();
        return 0;   // pas d'erreur
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int mx, my;
        unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
        if (mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            m_camera.rotation(mx, my);
        else if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);

        glUseProgram(m_program);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // configurer le shader program
        // . recuperer les transformations
        Transform model = Identity();
        Transform view = m_camera.view();
        Transform projection = m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
        Transform mvp = projection * view * model;
        program_uniform(m_program, "mvpMatrix", mvp);
        program_uniform(m_program, "time", global_time() / 1000);

        // dessine le repere, place au centre du monde, pour le point de vue de la camera 
        draw(m_repere, Identity(), m_camera);
        //draw(plane, /* model */ Identity(), camera());

		// dessine le terrain
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        draw(terrain, /* model */ Identity(), m_camera);


        return 1;
    }

protected:
    Mesh m_objet;
    Mesh m_cube;
    Mesh m_repere;
    Mesh plane;
	Mesh terrain;

    Orbiter m_camera;
    GLuint m_texture;
    GLuint m_program;
};


int main(int argc, char** argv)
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();

    return 0;
}
