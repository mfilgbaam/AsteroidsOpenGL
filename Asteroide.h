#pragma once

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
class Asteroide
{

private:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec4 damagecolor = glm::vec4(1, 1, 1, 1.0f);
	glm::mat4 trans;
	float vida;
	float hitbox ;
	float direction = 90;
	int tipo;
	int forma;
	float speed;
	float rotation;
	static float constexpr rotations[] = {-30, -20, -10, 10, 20, 30,};
	float timer;
	bool timerset = false;

public:
	Asteroide(int, int);
	Asteroide(int);
	Asteroide();
	~Asteroide();
	bool operator == (const Asteroide& a) const { return position == a.position; }
	bool operator != (const Asteroide& a) const { return !operator==(a); }
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3);
	glm::mat4 GetTrans();
	void SetTrans(glm::mat4);
	float GetVida();
	float GetHitbox();
	int GetTipo();
	int GetForma();
	glm::vec4 GetColor();
	void ChangeColor();
	glm::mat4 Move(glm::vec3,float);
	bool Damage(float);
	void SetTimer(float);
	bool Timer(float);

};

