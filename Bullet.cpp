#include "dependente/glfw/glfw3.h"
#include "Bullet.h"

Bullet::Bullet(float direction, glm::vec3 pos) {
	this->position = pos;
	this->direction = direction;
	trans = glm::translate(glm::mat4(1.0f), pos);
	trans = glm::rotate(trans, direction - 90, glm::vec3(0.0, 0.0, 1.0));

}
Bullet::Bullet() {
	Bullet(90, glm::vec3(0.0, 0.0, 0.0));
}
Bullet::~Bullet(){}

glm::vec3 Bullet::GetPosition() {
	return this->position;
}
glm::vec4 Bullet::GetColor() {
	return this->color;
}
void Bullet::SetColor(glm::vec4 color) {
	this->color = color;
}
float Bullet::GetHitbox() {
	return hitbox;
}
glm::mat4 Bullet::Move(float delta) {
	this->trans = glm::translate(trans, glm::vec3(0.0, this->speed * delta, 0.0));
	this->position.x = trans[3][0];
	this->position.y = trans[3][1];
	return trans;
}
