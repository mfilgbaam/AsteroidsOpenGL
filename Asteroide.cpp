#include "dependente/glfw/glfw3.h"
#include "Asteroide.h"
#include <time.h>


Asteroide::Asteroide(int tipo, int angulo) {
	float distance = 1.41422;
	position = glm::vec3(cos(glm::radians((float)angulo))* distance/tipo, sin(glm::radians((float)angulo)) * distance/tipo, 0);
	trans = glm::scale(glm::mat4(1.0f), glm::vec3(tipo, tipo, 0));
	trans = glm::translate(trans, position);
	this->tipo = tipo;
	this->vida=10*pow(2.0,tipo-1);
	this->hitbox=0.04*tipo;
	this->speed = 0.001/tipo/tipo;
	if (tipo == 1) this->color = glm::vec4(1, 1, 0, 1.0);
	if (tipo == 2) this->color = glm::vec4(1, 0.5f, 0, 1.0);
	if (tipo == 3) this->color = glm::vec4(1, 0, 0, 1.0);
	srand(time(0));
	this->rotation = rotations[rand() % 6];
	this->forma = rand() % 2;
	
}
Asteroide::Asteroide(int tipo) {
	 Asteroide(tipo, 90);
}
Asteroide::Asteroide() {
	Asteroide(1);
}
Asteroide::~Asteroide(){}

glm::vec3 Asteroide::GetPosition() {
	return this->position;
}
void Asteroide::SetPosition(glm::vec3 position) {
	this->position = position;
}

glm::mat4 Asteroide::GetTrans() {
	return this->trans;
}
void Asteroide::SetTrans(glm::mat4) {
	this->trans = trans;
}

float Asteroide::GetVida() {
	return vida;
}
float Asteroide::GetHitbox() {
	return hitbox;
}
int Asteroide::GetTipo() {
	return tipo;
}
int Asteroide::GetForma() {
	return forma;

}
glm::vec4 Asteroide::GetColor() {
	return color;
}
void Asteroide::ChangeColor() {
	glm::vec4 temp = this->color;
	this->color = this->damagecolor;
	this->damagecolor = temp;
}
glm::mat4 Asteroide::Move(glm::vec3 nave, float delta) {
	float xspeed= 0, yspeed= 0;
	float deltaspeed = speed;
	
	if (this->position.x < nave.x) {
		xspeed += deltaspeed;
	}
	else if (this->position.x > nave.x) {
		xspeed -= deltaspeed;
	}
	if (this->position.y < nave.y) {
		yspeed += deltaspeed;
	}
	else if (this->position.y > nave.y) {
		yspeed -= deltaspeed;
	}

	this->direction += rotation * delta;
	this->trans = glm::rotate(this->trans, this->rotation * delta, glm::vec3(0.0, 0.0, 1.0));
	
	
	this->trans = glm::translate(this->trans, glm::vec3(-cos(glm::radians(this->direction)) * yspeed + sin(glm::radians(this->direction)) * xspeed,
		sin(glm::radians(this->direction)) * yspeed + cos(glm::radians(this->direction)) * xspeed, 0));

	this->position.x = trans[3][0];
	this->position.y = trans[3][1];
	
	return this->trans;

}
bool Asteroide::Damage(float damage) {
	this->vida -= damage;
	printf("Damage to Asteroid: %0.0f\n", damage);
	return this->vida <= 0;
}
void Asteroide::SetTimer(float time) {
	this->timer = time + 0.1;
	timerset = true;
}
bool Asteroide::Timer(float time) {
	if (timerset && time >= timer) {
		timerset = false;
		return true;
	}
	return false;
}
