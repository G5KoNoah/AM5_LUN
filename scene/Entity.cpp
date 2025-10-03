#include "Entity.h"

Entity::Entity() {
	transform = Identity();
	parent = nullptr;
}

Entity::Entity(Transform t, Entity * p){
	transform = t;
	parent = p;
	if(parent != nullptr){
		parent->addChild(this);
	}
}

Entity::~Entity(){}

// Ajoute un enfant a l'entite
void Entity::addChild(Entity* e){
	child.push_back(e);
}

// Applique la transformation t a l'entite et a tous ses enfants
void Entity::ChangeTransform(Transform t){
	transform = t;
	for(auto it = child.begin(); it != child.end(); ++it){
		(*it)->ChangeTransform(t * (*it)->transform);
	}
}