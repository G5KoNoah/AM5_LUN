
//! \file tuto9_texture1.cpp utilisation d'un shader 'utilisateur' pour afficher un objet Mesh avec une texture.

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"
#include <vector>

#include "app.h"        // classe Application a deriver


class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP() : App(1024, 640) {}

    Mesh makePlane(int width, int height)
    {
        Mesh p_mesh = Mesh(GL_TRIANGLES);

        // 1) G�n�rer tous les sommets
        for (int j = 0; j <= height; j++)
        {
            for (int i = 0; i <= width; i++)
            {
                // coordonn�es dans [0,1], tu peux centrer si tu veux
                float x = float(i);
                float z = float(j);

                // position
                p_mesh.vertex(Point(x, 0.0f, z));
                // normale (vers le haut)
                p_mesh.normal(Vector(0, 1, 0));
                // uv
                p_mesh.texcoord(vec2(float(i) / width, float(j) / height));
            }
        }

        // 2) G�n�rer les triangles (2 par carr�)
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

    int init()
    {

   //     for (int i = 0; i < 5; i++) {
			//v_mesh.push_back(Mesh(GL_TRIANGLES));
			//v_mesh[i].vertex(-1 + i, 0, -1);
			//v_mesh[i].vertex(0 + i, 0, 1);
			//v_mesh[i].vertex(1 + i, 0, -1);
			//v_mesh[i].triangle(0, 1, 2);
   //     }
        v_mesh = makePlane(100, 100);
        m_objet = read_mesh("../data/cube.obj");
        Point pmin, pmax;
        v_mesh.bounds(pmin, pmax);
        m_camera.lookat(pmin, pmax);

        // etape 1 : creer le shader program
        m_program = read_program("../tutos/eau.glsl");
        program_print_errors(m_program);

        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit()
    {
        // etape 3 : detruire le shader program
        release_program(m_program);
        
        v_mesh.release();
        glDeleteTextures(1, &m_texture);
        return 0;
    }

    // dessiner une nouvelle image
    int render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // deplace la camera
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
        Transform model =Identity();
        Transform view = m_camera.view();
        Transform projection = m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
        Transform mvp = projection * view * model;
        program_uniform(m_program, "mvpMatrix", mvp);
		program_uniform(m_program, "time", global_time()/1000);
        v_mesh.draw(m_program, /* use position */ true, /* use texcoord */ true, /* use normal */ false, /* use color */ false, /* use material index*/ false);
		
        draw(m_objet, Identity(), m_camera);
        return 1;
    }

protected:
    Mesh v_mesh;
	Mesh m_objet;
    Orbiter m_camera;
    GLuint m_texture;
    GLuint m_program;
};


int main(int argc, char** argv)
{
    TP tp;
    tp.run();

    return 0;
}
