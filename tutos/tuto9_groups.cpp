//! \file tuto9_groups.cpp afficher un objet Mesh et ses matieres, dessine les groupe de triangles, un par un.

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "texture.h"
#include "draw.h"

#include "app_camera.h"        // classe Application a deriver


class TP : public AppCamera
{
public:
    GLuint m_texture0;
    GLuint m_texture1;
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : AppCamera(1024, 640) {}
    
    int init( )
    {
        m_objet= read_mesh("../data/Tree.obj");
        if(m_objet.materials().count() == 0)
            // pas de matieres, pas d'affichage
            return -1;
            
        printf("%d materials.\n", m_objet.materials().count());
        
        // trie les triangles par matiere et recupere les groupes de triangles utilisant la meme matiere.
        m_groups= m_objet.groups();
        /* remarque : c'est long, donc il vaut mieux le faire une seule fois au debut du programme...
         */
        
        // placer la camera
        Point pmin, pmax;
        m_objet.bounds(pmin, pmax);
        camera().lookat(pmin, pmax);
        
        // creer le shader program, uniquement necessaire pour l'option 2, cf render()
        m_program= read_program("../tutos/tuto9_groups.glsl");
        m_texture1 = read_texture(0, "../data/DB2X2_L01.png");
        program_print_errors(m_program);
        m_texture0 = read_texture(0, "../data/bark_0021.jpg");

        // corrige l'affichage du tronc : autoriser le tiling si les UV dépassent [0,1]
        glBindTexture(GL_TEXTURE_2D, m_texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        // Activer le blending pour supporter les textures avec alpha (feuillage, feuilles...)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        // etape 3 : detruire le shader program
        release_program(m_program);
        m_objet.release();
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // . recuperer les transformations
        Transform model= RotationY(global_time() / 40);
        Transform view= camera().view();
        Transform projection= camera().projection();
        
    #if 0
        // option 1 : avec les utilitaires draw()
        {
            // dessine chaque groupe de triangle, avec sa matiere
            for(unsigned i= 0; i < m_groups.size(); i++)
                draw(m_groups[i], m_objet, model, view, projection);
                // ou :
                // draw(m_groups[i], m_objet, model, camera());    // meme resultat...
        }
    #else
        // option 2 : dessiner m_objet avec le shader program
        {
            // configurer le pipeline 
            glUseProgram(m_program);
            
            // configurer le shader program
            // . composer les transformations : model, view et projection
            Transform mv= view * model;
            Transform mvp= projection * mv;
            
            // . parametrer le shader program :
            //   . transformations : la matrice declaree dans le vertex shader s'appelle mvpMatrix
            program_uniform(m_program, "mvpMatrix", mvp);
            // et mvMatrix 
      
            // afficher chaque groupe
            const Materials& materials= m_objet.materials();
            for(unsigned i= 0; i < m_groups.size(); i++)
            {
                // recuperer la couleur de base de la matiere
                const Material& material= materials(m_groups[i].index);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_texture0); // Texture diffuse
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_texture1);



                if (m_groups[i].index == 0) {

                    program_uniform(m_program, "texture0", 0);
              
                }
                else {
                    program_uniform(m_program, "texture0", 1);
                    
                }

                // go !
                // indiquer quels attributs de Sommets du mesh sont necessaires a l'execution du shader.
                // tuto9_groups.glsl n'utilise que position et normale. les autres de servent a rien.
                
                // 1 draw par groupe de triangles...
                m_objet.draw(m_groups[i].first, m_groups[i].n, m_program, /* use position */ true, /* use texcoord */ true, /* use normal */ true, /* use color */ false, /* use material index*/ false);
            }
        }
    #endif
        
        /* et directement avec openGL, qu'est ce qui change ?
            
            il faut creer un ou plusieurs buffers pour stocker les positions et les normales de l'objet, et configurer le format de sommet, 
            cf vertex array object / vao, comme dans tuto9_buffers.cpp ou tuto4GL.cpp et tuto4GL_normals.cpp, par exemple
            
            ensuite, c'est comme d'habitude :
            
            glBindVertexAttrib(m_vao);
            glUseProgram(m_program);
            
            // composer les transformations : model, view et projection
            Transform mv= view * model;
            Transform mvp= projection * mv;
            
            // parametrer le shader program :
            program_uniform(m_program, "mvpMatrix", mvp);
            program_uniform(m_program, "mvMatrix", mv);
            
            // dessiner chaque groupe...
            for(unsigned i= 0; i < groups.size(); i++)
            {
                // recuperer la couleur de la matiere du groupe de triangles
                const Material& material= m_objet.materials().material(m_groups[i].material_index);
                Color color= material.diffuse;
                
                // parametrer les uniforms du shader qui dependent de la matiere
                program_uniform(m_program, "material_color", color);
                
                // go ! dessiner les triangles du groupe
                glDrawArrays(GL_TRIANGLES, groups[i].first, groups[i].n);
            }
        */
        
        return 1;
    }

protected:
    Transform m_model;
    Mesh m_objet;
    GLuint m_texture;
    GLuint m_program;
    std::vector<TriangleGroup> m_groups;
};


int main( int argc, char **argv )
{
    TP tp;
    tp.run();
    
    return 0;
}
