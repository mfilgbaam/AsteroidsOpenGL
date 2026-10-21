#pragma once

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

class Nave
{
private:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec4 damagecolor = glm::vec4(1, 1, 1, 1.0f);
	float vida= 100;
	float hitbox = 0.020;
	float direction = 90;
	float timer;
	bool timerset = false;
public:
	Nave();
	~Nave();
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3);
	glm::vec4 GetColor();
	void ChangeColor();
	float GetVida();
	float GetHitbox();
	glm::vec3 MoveX(float);
	glm::vec3 MoveY(float);
	void Rotate(float);
	float GetDirection();
	bool Damage(float);
	void SetTimer(float);
	bool Timer(float);
};

