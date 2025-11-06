#include "Billboard.h"

Billboard::Billboard(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	makePlane(5, 2, vec3(1.0f, 0.0f, 1.0f), &mesh);
}

Billboard::Billboard(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	makePlane(5, 2, vec3(1.0f, 0.0f, 1.0f), &mesh);
}	

void Billboard::makePlane(int width, int height, vec3 normal, Mesh * mesh) {
	mesh->create(GL_TRIANGLES);
	normal = normalize(normal);

	mesh->normal(normal.x, normal.y, normal.z);
	mesh->texcoord(0.0f, 0.0f);
	mesh->vertex(vec3(-width / 2.0f, -height / 2.0f, 0.0f));

	mesh->normal(normal.x, normal.y, normal.z);
	mesh->texcoord(1.0f, 0.0f);
	mesh->vertex(vec3(width / 2.0f, -height / 2.0f, 0.0f));

	mesh->normal(normal.x, normal.y, normal.z);
	mesh->texcoord(1.0f, 1.0f);
	mesh->vertex(vec3(width / 2.0f, height / 2.0f, 0.0f));

	mesh->normal(normal.x, normal.y, normal.z);
	mesh->texcoord(0.0f, 1.0f);
	mesh->vertex(vec3(-width / 2.0f, height / 2.0f, 0.0f));
	mesh->triangle(0, 1, 2);
	mesh->triangle(0, 2, 3);
}

void Billboard::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights)  {
	//std::cout << "Drawing Billboard" << std::endl;
	Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;

	glUseProgram(shader);
	program_uniform(shader, "mvpMatrix", mvp);
	program_uniform(shader, "ViewMatrix", camera->view());

	program_uniform(shader, "center_worldspace", transform[3]);
	program_uniform(shader, "BillboardSize", vec2(1.0,1.0));
	program_uniform(shader, "color", Color(color.x, color.y, color.z, 1.0f));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); // Texture diffuse
	program_use_texture(shader, "texture0", 0, texture);
	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);


}
