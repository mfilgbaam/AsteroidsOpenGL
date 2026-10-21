#pragma once

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
class Bullet {
private:
	glm::vec3 position;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 trans;
	float hitbox = 0;
	float speed = 1;
	float direction;
public:
	Bullet(float, glm::vec3);
	Bullet();
	~Bullet();
	bool operator == (const Bullet& b) const { return position == b.position; }
	bool operator != (const Bullet& b) const { return !operator==(b); }
	glm::mat4 Move(float);
	glm::vec3 GetPosition();
	glm::vec4 GetColor();
	float GetHitbox();
	void SetColor(glm::vec4);
};