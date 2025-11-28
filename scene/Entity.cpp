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
	// on applique t à cette entité (application "locale" : ancienne * t)
	transform = transform * t;
	// mais pour les enfants, la transformation parentale doit être pré-multipliée
	for(auto it = child.begin(); it != child.end(); ++it){
		(*it)->applyParentTransform(t);
	}
}

// applique la transformation du parent aux descendants (pré-multiplication)
void Entity::applyParentTransform(const Transform& t){
	// pré-multiplier : la transformation parentale doit s'appliquer avant la transform locale
	transform = t * transform;
	for(auto it = child.begin(); it != child.end(); ++it){
		(*it)->applyParentTransform(t);
	}
}