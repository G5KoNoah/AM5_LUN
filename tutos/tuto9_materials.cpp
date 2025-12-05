//! \file tuto9_materials.cpp utilisation d'un shader 'utilisateur' pour afficher un objet Mesh et ses matieres.

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app_camera.h"        // classe Application a deriver

// pour lire les images et calculer une couleur moyenne si Kd == 0
#include "image_io.h"
#include "texture.h"

class TP : public AppCamera
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : AppCamera(1024, 640), m_program(0), m_default_texture(0)
    {
    }
    
    int init( )
    {
        const int MAX_MATERIALS = 16;

        m_objet= read_mesh("../data/tree.obj");
        std::vector<TriangleGroup> t = m_objet.groups();
		std::cout << "Nombre de groupes de triangles : " << t.size() << std::endl;
		std::cout << t[0].n << std::endl;
        std::cout << t[0].first << std::endl;
        std::cout << t[0].index << std::endl;
        std::cout << t[1].n << std::endl;
        std::cout << t[1].first << std::endl;
        std::cout << t[1].index << std::endl;
        if(m_objet.materials().count() == 0)
            // pas de matieres, pas d'affichage
            return -1;
        printf("%d materials.\n", m_objet.materials().count());
        
        // etape 1 : creer le shader program
        m_program= read_program("../tutos/tuto9_materials.glsl");
        program_print_errors(m_program);
        
        // recupere les matieres.
        // le shader declare un tableau de 16 matieres
        m_colors.resize(MAX_MATERIALS);
        m_material_textures.assign(MAX_MATERIALS, 0);
        m_material_has_texture.assign(MAX_MATERIALS, 0);
        
        // copier les matieres utilisees
        const Materials& materials= m_objet.materials();
        assert(materials.count() <= int(m_colors.size()));
        for(int i= 0; i < materials.count(); i++)
        {
            // copie la couleur diffuse
            m_colors[i]= materials.material(i).diffuse;

            // si la couleur diffuse est noire et qu'une texture diffuse est fournie,
            // lire l'image et utiliser sa couleur moyenne comme fallback.
            if(m_colors[i].r == 0.f && m_colors[i].g == 0.f && m_colors[i].b == 0.f)
            {
                int tex_index= materials.material(i).diffuse_texture;
                if(tex_index != -1 && tex_index < materials.filename_count())
                {
                    const char *texfile= materials.filename(tex_index);
                    if(texfile && texfile[0])
                    {
                        // on conserve la couleur moyenne (fallback), mais on chargera la texture pour l'échantillonnage réel
                        ImageData img= read_image_data(texfile);
                        if(img.width > 0 && img.pixels.size() > 0)
                        {
                            double sr= 0.0, sg= 0.0, sb= 0.0;
                            int channels= img.channels ? img.channels : 3;
                            size_t count= (size_t) img.width * (size_t) img.height;
                            for(size_t p= 0; p < img.pixels.size(); p += channels)
                            {
                                sr += img.pixels[p + 0];
                                sg += img.pixels[p + (channels > 1 ? 1 : 0)];
                                sb += img.pixels[p + (channels > 2 ? 2 : 0)];
                            }
                            if(count > 0)
                            {
                                float fr = float((sr / 255.0) / double(count));
                                float fg = float((sg / 255.0) / double(count));
                                float fb = float((sb / 255.0) / double(count));
                                m_colors[i]= Color(fr, fg, fb, 1.f);
                            }
                        }
                        else
                        {
                            // fallback si lecture impossible
                            m_colors[i]= Color(0.8f);
                        }
                    }
                }
                else
                {
                    // pas de texture, fallback pour que l'objet soit visible
                    m_colors[i]= Color(0.5f);
                }
            }

            // charger la texture (si elle existe) : on mappe chaque matériau sur une unité GL fixe i
            int diffuse_tex = materials.material(i).diffuse_texture;
            if(diffuse_tex != -1 && diffuse_tex < materials.filename_count())
            {
                const char* fname = materials.filename(diffuse_tex);
                if(fname && fname[0])
                {
                    // read_texture active l'unite et crée la texture ; on utilise l'unité i
                    GLuint tex = read_texture(i, fname);
                    if(tex != 0)
                    {
                        m_material_textures[i] = tex;
                        m_material_has_texture[i] = 1;
                    }
                }
            }
        }
        
        // create default white texture for empty slots
        {
            ImageData white(1,1,4);
            white.pixels[0]= 255; white.pixels[1]= 255; white.pixels[2]= 255; white.pixels[3]= 255;
            m_default_texture = make_texture(0, white, GL_RGBA32F);
            // we won't rely on unit 0 being free; we'll bind each slot explicitly in render()
        }

        // placer la camera
        Point pmin, pmax;
        m_objet.bounds(pmin, pmax);
        camera().lookat(pmin, pmax);
        
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

        // detruire les textures des matériaux
        for(auto t : m_material_textures)
            if(t) glDeleteTextures(1, &t);
        if(m_default_texture) glDeleteTextures(1, &m_default_texture);

        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        const int MAX_MATERIALS = 16;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // dessiner m_objet avec le shader program
        // configurer le pipeline 
        glUseProgram(m_program);
        
        // configurer le shader program
        // . recuperer les transformations
        Transform model= RotationY(global_time() / 40);
        Transform view= camera().view();
        Transform projection= camera().projection(window_width(), window_height(), 45);
        
        // . composer les transformations : model, view et projection
        Transform mv= view * model;
        Transform mvp= projection * mv;
        
        // . parametrer le shader program :
        //   . transformation : la matrice declaree dans le vertex shader s'appelle mvpMatrix
        program_uniform(m_program, "mvpMatrix", mvp);
        program_uniform(m_program, "mvMatrix", mv);
        
        // binder les textures materialsTex[i] sur les unités 0..MAX_MATERIALS-1
        // si un matériau n'a pas de texture, binder la texture blanche par défaut sur l'unité correspondante
        for(int i = 0; i < MAX_MATERIALS; ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            if(i < (int)m_material_textures.size() && m_material_textures[i] != 0)
                glBindTexture(GL_TEXTURE_2D, m_material_textures[i]);
            else
                glBindTexture(GL_TEXTURE_2D, m_default_texture);
        }
        // indique au shader quelles unités correspondent aux samplers
        GLint loc = glGetUniformLocation(m_program, "materialsTex");
        if(loc != -1)
        {
            int units[MAX_MATERIALS];
            for(int i=0;i<MAX_MATERIALS;i++) units[i]= i;
            glUniform1iv(loc, MAX_MATERIALS, units);
        }

        // indique au shader quels matériaux ont une texture
        GLint locHas = glGetUniformLocation(m_program, "material_has_texture");
        if(locHas != -1)
        {
            int has[MAX_MATERIALS];
            for(int i=0;i<MAX_MATERIALS;i++) has[i]= (i < (int)m_material_has_texture.size()) ? m_material_has_texture[i] : 0;
            glUniform1iv(locHas, MAX_MATERIALS, has);
        }

        // . parametres "supplementaires" :
        //   . couleur diffuse des matieres, cf la declaration 'uniform vec4 materials[];' dans le fragment shader
        int location= glGetUniformLocation(m_program, "materials");
        glUniform4fv(location, m_colors.size(), &m_colors[0].r);
        
        // go !
        // indiquer quels attributs de sommets du mesh sont necessaires a l'execution du shader.
        // maintenant le shader utilise position, texcoord, normal et material_index.
        // 1 draw pour tous les triangles de l'objet.
        m_objet.draw(m_program, /* use position */ true, /* use texcoord */ true, /* use normal */ true, /* use color */ false, /* use material index*/ true);
        
        return 1;
    }

protected:
    Transform m_model;
    Mesh m_objet;
    Orbiter m_camera;
    GLuint m_texture;
    GLuint m_program;
    std::vector<Color> m_colors;

    // per-material textures
    std::vector<GLuint> m_material_textures;
    std::vector<int>   m_material_has_texture;
    GLuint m_default_texture;
};
    
    
int main( int argc, char **argv )
{
    TP tp;
    tp.run();
    
    return 0;
}
