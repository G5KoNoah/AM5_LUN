
//! \file tuto9_texture1.cpp utilisation d'un shader 'utilisateur' pour afficher un objet Mesh avec une texture.

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app.h"        // classe Application a deriver

#include <list>
using namespace std;


class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : App(1024, 640) {}
    
    int init( )
    {
        // Declaration des meshs

        m_objet= read_mesh("data/cube.obj");

        objets.emplace_front(m_objet);

        m_objet= read_mesh("data/Tree.obj");
        
        objets.emplace_front(m_objet);

        //Point
        Point pmin, pmax;
        m_objet.bounds(pmin, pmax);
        m_camera.lookat(pmin, pmax);
        
        // Declaration des textures dans l'ordre

        m_texture= read_texture(0, "data/debug2x2red.png");

        textures.emplace_front(m_texture);

        m_texture= read_texture(1, "data/pacman.png");

        textures.emplace_front(m_texture);

        // etape 1 : creer le shader program
        m_program= read_program("projets/test.glsl");
        program_print_errors(m_program);
        
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest
        
        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        // etape 3 : detruire le shader program
        release_program(m_program);
        
        m_objet.release();

        // FAIRE LES RELEASES

        glDeleteTextures(1, &m_texture);
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // deplace la camera
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            m_camera.rotation(mx, my);
        else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());
        
        // etape 2 : dessiner m_objet avec le shader program
        // configurer le pipeline 
        glUseProgram(m_program);
        
        // configurer le shader program
        // . recuperer les transformations
        Transform model= Identity(); //RotationX(global_time() / 20);
        Transform view= m_camera.view();
        Transform projection= m_camera.projection(window_width(), window_height(), 45);

        // . composer les transformations : model, view et projection
        Transform mvp= projection * view * model;
        
        // . parametrer le shader program :
        //   . transformation : la matrice declaree dans le vertex shader s'appelle mvpMatrix
        //program_uniform(m_program, "mvpMatrix", mvp);
        
        // . parametres "supplementaires" :
        //   . utilisation d'une texture configuree sur l'unite 0, le fragment shader declare "uniform sampler2D texture0;"
        //program_use_texture(m_program, "texture0", 0, m_texture);
        
        // go !
        // indiquer quels attributs de sommets du mesh sont necessaires a l'execution du shader.
        // tuto9_texture1.glsl n'utilise que position et texcoord. les autres de servent a rien.
        //m_objet.draw(m_program, /* use position */ true, /* use texcoord */ true, /* use normal */ false, /* use color */ false, /* use material index*/ false);
        

        // Affichage des meshs
        
        list<GLuint>::iterator text = textures.begin();
        for(list<Mesh>::iterator i = objets.begin(); i != objets.end(); i++){

            mvp = projection * view * model;
            program_uniform(m_program, "mvpMatrix", mvp);
            program_use_texture(m_program, "texture0", 0, (*text));
            (*i).draw(m_program, /* use position */ true, /* use texcoord */ true, /* use normal */ false, /* use color */ false, /* use material index*/ true);
            text++;

        }
        
        return 1;
    }

protected:
    Mesh m_objet; // Mesh exemple
    list<Mesh> objets; // Liste de tous les meshs a lire de la scene
    Orbiter m_camera;
    GLuint m_texture;
    list<GLuint> textures; // Liste de toutes les textures de la scene
    GLuint m_program;
};


int main( int argc, char **argv )
{
    TP tp;
    tp.run();
    
    return 0;
}
