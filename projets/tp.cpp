
//! \file tp.cpp code de depart minimaliste


#include "wavefront.h"
#include "texture.h"

#include "draw.h"        
#include "app.h"        // classe Application a deriver


// utilitaire. creation d'une grille / repere. permet de visualiser l'espace autour des objets.
Mesh make_grid( const int n= 10 )
{
    Mesh grid= Mesh(GL_LINES);
    
    // grille
    grid.color(White());
    for(int x= 0; x < n; x++)
    {
        float px= float(x) - float(n)/2 + .5f;
        grid.vertex(Point(px, 0, - float(n)/2 + .5f)); 
        grid.vertex(Point(px, 0, float(n)/2 - .5f));
    }

    for(int z= 0; z < n; z++)
    {
        float pz= float(z) - float(n)/2 + .5f;
        grid.vertex(Point(- float(n)/2 + .5f, 0, pz)); 
        grid.vertex(Point(float(n)/2 - .5f, 0, pz)); 
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


class TP : public App
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : App(1024, 640) {}
    
    // creation des objets de l'application
    int init( )
    {
        // decrire un repere / grille 
        m_repere= make_grid(10);
        
        // charge un objet
        m_objet= read_mesh("data/cube.obj");
        //~ m_objet= read_mesh("data/robot.obj");
        //~ m_objet= read_mesh("data/bigguy.obj");
        
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // pas d'erreur, sinon renvoyer -1
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        m_objet.release();
        m_repere.release();
        return 0;   // pas d'erreur
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        // transformations standards
        Transform view= Translation(0, -1, -10);
        Transform projection= Perspective(45, float(window_width()) / float(window_height()), 0.1, 100);
        
        // Transform viewport= Viewport(window_width(), window_height());
        glViewport(0, 0, window_width(), window_height());
        
        // couleur et profondeur par defaut
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    // dessine le repere, place au centre du monde
        draw(m_repere, /* model */ Identity(), view, projection);
        
        
    // dessine un objet... avec sa propre transformation
        Transform model= Identity();
        draw(m_objet, model, view, projection);
        
        
        // continuer, afficher une nouvelle image
        // tant que la fenetre est ouverte...
        return 1;
    }

protected:
    Mesh m_objet;
    Mesh m_repere;
};


int main( int argc, char **argv )
{
    // il ne reste plus qu'a creer un objet application et la lancer 
    TP tp;
    tp.run();
    
    return 0;
}
