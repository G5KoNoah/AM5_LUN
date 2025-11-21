#include "Sky.h"

std::vector<std::string> faces = {
    "../data/ciel.jpg",
    "../data/ciel.jpg",
    "../data/ciel.jpg",
    "../data/ciel.jpg",
    "../data/ciel.jpg",
    "../data/ciel.jpg"
};
GLuint loadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        Image image = read_image(faces[i].c_str());
        if (image == Image::error())
        {
            std::cout << "❌ Erreur : impossible de charger " << faces[i] << std::endl;
            continue;
        }

        // gKit stocke chaque pixel en RGBA (Color), donc format GL_RGBA
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGBA,
            image.width(),
            image.height(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.data()
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// Classe representant un Sky

Sky::Sky(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh("../data/cube.obj");
	
}

Sky::Sky(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = read_mesh("../data/cube.obj");
}

Sky::Sky(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	std::cout << "Sky constructeur" << std::endl;
	mesh = read_mesh("../data/cube.obj");
	std::cout << "Sky textures" << std::endl;
    GLuint skyboxTex = loadCubemap(faces);
    //skyTextures.push_back(skyboxTex);
	//skyTextures.push_back(read_texture(0, "../data/lune.jpg"));
	std::cout << "Sky texture sun" << std::endl;
	std::cout << "Init Sky" << std::endl;
}




void Sky::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {

	//std::cout << "Drawing Sky" << std::endl;
	Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, skyTextures[0]); // Texture diffuse


    glDepthMask(GL_FALSE);

    program_uniform(shader, "projection", Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f));
    program_uniform(shader, "view", camera->view());
    program_uniform(shader, "sun_dir", dirLight->direction);
    float time = SDL_GetTicks() / 1000.0f;
    program_uniform(shader, "time", time);

    mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);

    glDepthMask(GL_TRUE);



	

}

void Sky::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights, float waterHeight, bool dir) {

	//std::cout << "Drawing Sky" << std::endl;
	Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, skyTextures[0]); // Texture diffuse


    glDepthMask(GL_FALSE);

    program_uniform(shader, "projection", Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f));
    program_uniform(shader, "view", camera->view());
    program_uniform(shader, "sun_dir", dirLight->direction);
    float time = SDL_GetTicks() / 1000.0f;
    program_uniform(shader, "time", time);

    mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);

    glDepthMask(GL_TRUE);



	

}

