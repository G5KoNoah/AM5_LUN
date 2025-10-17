#include "Arbres.h"
#include <chrono>
#include <cmath>

// Classe representant un plane

/*Arbres::Arbres(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p) {
    mesh = make_plane(10.0f, 100);

}

Arbres::Arbres(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p) {
    mesh = make_plane(10.0f, 100);
}*/

Arbres::Arbres(std::string strShader, vec3 c, Transform tr, Entity* p){
    arbres = make_trees(strShader, c, tr, p, 0.0f, 0.0f, 2.0f, 15, 0.2f);
}

vector<Object3D*> Arbres::make_trees(std::string strShader, vec3 c, Transform tr, Entity* p, float px, float pz, float r, int nb_arbre, float ra)
{
    vector<Object3D*> a;
    int i = 0;
	float pxa, pza;
    bool hit;
	default_random_engine generator(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    uniform_real_distribution<float> dist_angle(0, 2 * M_PI);
    uniform_real_distribution<float> dist_radius(0, 1.0f);

	int essais = 0;
	int max_essais = 1000;

    while (i < nb_arbre && essais<max_essais) {
		float angle = dist_angle(generator);
		float radius = r * sqrt(dist_radius(generator));
        
		pxa = px + radius * cos(angle);
		pza = pz + radius * sin(angle);

        hit = false;

        for (auto arbre : a) {
			float dx = pxa - arbre->transform.column(3).x;
			float dz = pza - arbre->transform.column(3).z;
            if(sqrt(dx * dx + dz * dz) < ra) {
                hit = true;
                break;
			}
        }

        if (!hit) {
            a.push_back(new Cube(strShader, c, tr * Translation(vec3(pxa, 0.0f, pza))* Scale(0.2f), p));
            i++;
        }

		essais++;
    }

	return a;
}

