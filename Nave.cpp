#include "Nave.h"
#include <iostream>

Nave::Nave() {

	position = glm::vec3(0, 0, 0);
	vida = 100;
	color = glm::vec4(0, 0.5f, 0, 1.0f);
}
Nave::~Nave(){}

glm::vec3 Nave::GetPosition() {
	return this->position;
}

void Nave::SetPosition(glm::vec3 position) {
	this->position = position;
}

float Nave::GetVida() {
	return vida;
}
float Nave::GetHitbox() {
	return hitbox;
}
glm::vec4 Nave::GetColor() {
	return color;
}
void Nave::ChangeColor() {
	glm::vec4 temp = this->color;
	this->color = this->damagecolor;
	this->damagecolor = temp;
}
glm::vec3 Nave::MoveX(float x) {
	if ((this->position.x+x+this->hitbox >= 1) || (this->position.x+x-this->hitbox <= -1)) {
		return glm::vec3(0, 0, 0);
	}else {
		this->position.x += x;
		return glm::vec3(sin(glm::radians(this->direction))*x, cos(glm::radians(this->direction)) * x, 0);
	}
}
glm::vec3 Nave::MoveY(float y) {
	if ((this->position.y+y+ this->hitbox >= 1) || (this->position.y+y- this->hitbox <= -1)) {
		return glm::vec3(0, 0, 0);
	}
	else {
		this->position.y += y;
		return glm::vec3(-cos(glm::radians(this->direction)) * y, sin(glm::radians(this->direction)) * y, 0);
	}
}
void Nave::Rotate(float angle) {
	this->direction += angle;
}
float Nave::GetDirection() {
	return this->direction ;
}
bool Nave::Damage(float damage) {
	this->vida -= damage;
	printf("Damage to Spaceship: %0.0f\n", damage);
	return this->vida <= 0;
}
void Nave::SetTimer(float time) {
	this->timer = time + 0.1;
	timerset = true;
}
bool Nave::Timer(float time) {
	if (timerset && time >= timer) {
		timerset = false;
		return true;
	}
	return false;
}