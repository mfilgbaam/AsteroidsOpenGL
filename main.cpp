// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
#include <iterator>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"



#include "shader.hpp"
#include "Nave.h"
#include "Asteroide.h"
#include "Bullet.h"

/*
Movement: WASD
Rotation: Q E or MouseButton4 MouseButton5
Firing: Mouse Left
Spawn a single enemy: Mouse Right
Toggle Wave Spawning: Space
*/

//Variables
GLFWwindow* window;
const int width = 896, height = 896;
bool wavespawning = false;
float wstime;
float difficulty;
int wscounter;

//Create matrices for transforms
glm::mat4 transnave(1.0f);

//Objects
Nave nave= Nave::Nave();
std::list<Asteroide> aslist;
std::list<Bullet> bulletlist;

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



//Window Callback
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

//Input
void processInput(GLFWwindow* window)
{
	static bool rightbutton, leftbutton, space;
	float rotate_speed = 200 * deltaTime;
	float speed = 0.3 * deltaTime;
	glm::vec3 movement;
	
	//Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		movement = nave.MoveY(speed);
		transnave = glm::translate(transnave, movement);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		movement = nave.MoveY(-speed);
		transnave = glm::translate(transnave, movement);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		movement = nave.MoveX(speed);
		transnave = glm::translate(transnave, movement);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		movement = nave.MoveX(-speed);
		transnave = glm::translate(transnave, movement);
	}
	//Rotation
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)||(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)) {
		nave.Rotate(rotate_speed);
		transnave = glm::rotate(transnave, rotate_speed, glm::vec3(0.0, 0.0, 1.0));
	}
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS)||(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)) {
		nave.Rotate(-rotate_speed);
		transnave = glm::rotate(transnave, -rotate_speed, glm::vec3(0.0, 0.0, 1.0));
	}
	//Spawn Random Asteroid
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && rightbutton) {
		rightbutton = false;
		aslist.push_back(Asteroide::Asteroide((rand() % 3)+1, rand() % 360));
		
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !rightbutton) {
		rightbutton = true;
		
	}
	//Fire a Bullet
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && leftbutton) {
		bulletlist.push_back(Bullet::Bullet(nave.GetDirection(),nave.GetPosition()));
		leftbutton = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && !leftbutton) {
		leftbutton = true;
	}
	//Wave spawning
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && space) {
		printf("Wave Spawning Toggled\n");
		space = false;
		difficulty = 1;
		wscounter = 0;
		wavespawning = !wavespawning;
		wstime = glfwGetTime();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && !space) {
		space = true;
	}

}
//Collisions
bool Collide(glm::vec3 obj1, float hitbox1, glm::vec3 obj2, float hitbox2) {
	if ((obj1.x < obj2.x && obj1.x + hitbox1 + hitbox2 < obj2.x)|| (obj2.x < obj1.x&& obj2.x + hitbox1 + hitbox2 < obj1.x)) return false;

	if ((obj1.y < obj2.y && obj1.y + hitbox1 + hitbox2 < obj2.y)|| (obj2.y < obj1.y && obj2.y + hitbox1 + hitbox2 < obj1.y)) return false;

	return true;
}
//Waves with increasing difficulty
void WavesSpawning() {
	float time = glfwGetTime() - wstime;
	if (time >= difficulty * 10) {
		printf("\nDIFFICULTY INCREASE\n\n");
		difficulty += 0.1;
		wscounter = 0;
		wstime = glfwGetTime();
	}
	if (time >= wscounter*2/difficulty) {
		printf("Spawning...\n");
		wscounter++;
		aslist.push_back(Asteroide::Asteroide((rand() % 3) + 1, rand() % 360));
	}
}
//Game Over Screen
void GameOver() {
	printf("\nGAME OVER\n");
	while (!glfwWindowShouldClose(window)) {
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

int main(void)
{
	//Random Seed
	srand(time(0));
	
	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Asteroids", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	
	//Spaceship
	GLfloat vertices[] = {
		0,  0.025, 0.0f,  // top 
		0.020f, -0.025f, 0.0f,  // bottom right
		-0.020f, -0.025f, 0.0f,  // bottom left
		-0.010f, -0.005f, 0.0f, //middle left
		0.010f, -0.005f, 0.0f, //middle right
		
	};

	GLuint indices[] = {  
		0, 3, 1,  
		0, 2, 4,  
	
	};

	//Asteroid 1
	GLfloat vertices2[] = {
		-0.005,  0.05, 0.0f,  
		0.04f, 0.04f, 0.0f,  
		-0.05f, 0.01f, 0.0f,  
		0.05f, 0.0f, 0.0f,
		0.04f, -0.03f, 0.0f,
		-0.02f, -0.05f, 0.0f,

	};

	GLuint indices2[] = {  
		0, 2, 1,  
		1, 2, 3,
		3, 2, 4,
		4, 2, 5,
	};

	//Asteroid 2
	GLfloat vertices3[] = {
		0.005,  0.05, 0.0f,
		-0.04f, 0.04f, 0.0f,
		0.05f, 0.01f, 0.0f,
		-0.05f, -0.01f, 0.0f,
		-0.04f, -0.03f, 0.0f,
		0.02f, -0.05f, 0.0f,

	};

	GLuint indices3[] = {  
		0, 2, 1,  
		1, 2, 3,
		3, 2, 4,
		4, 2, 5,
	};

	//Bullets
	GLfloat vertices4[] = {
		-0.003f, 0.005f, 0.0f,
		0.003f, 0.005f, 0.0f,
		-0.003f, -0.005f, 0.0f,
		0.003f, -0.005f, 0.0f,
	};
	GLuint indices4[] = { 
		0, 2, 3,
		1, 2, 3,
	};

	 
	GLuint navevbo, navevao, naveibo,
		asvbo, asvao, asibo,
		asvbo2, asvao2, asibo2,
		bulletvbo, bulletvao, bulletibo;

	//Spaceship
	glGenVertexArrays(1, &navevao);
	glGenBuffers(1, &navevbo);
	glGenBuffers(1, &naveibo);

	//bind VAO
	glBindVertexArray(navevao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, navevbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, naveibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//Asteroid 1
	glGenVertexArrays(1, &asvao);
	glGenBuffers(1, &asvbo);
	glGenBuffers(1, &asibo);

	//bind VAO
	glBindVertexArray(asvao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, asvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//Asteroid 2

	glGenVertexArrays(1, &asvao2);
	glGenBuffers(1, &asvbo2);
	glGenBuffers(1, &asibo2);

	//bind VAO
	glBindVertexArray(asvao2);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, asvbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//Bullets

	glGenVertexArrays(1, &bulletvao);
	glGenBuffers(1, &bulletvbo);
	glGenBuffers(1, &bulletibo);

	//bind VAO
	glBindVertexArray(bulletvao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, bulletvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	//Callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);


	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{

		//calculate delta time 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*To not mess up with the lists,
		every time we delete an object we run the loop from the begining*/
		Next:

		//Wavespawning Call
		if (wavespawning) WavesSpawning();

		glBindVertexArray(0);
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		//Process Inputs
		processInput(window);

		// Use our shader
		glUseProgram(programID);

		//Bullets
		
		//bind VAO
		glBindVertexArray(bulletvao);

		for (Bullet& bullet : bulletlist) {

			glm::vec3 bulletpos = bullet.GetPosition();

			//Check if its inside the boundaries
			if (abs(bulletpos.x) > 1 || abs(bulletpos.y) > 1) {
				bulletlist.remove(bullet);
				bullet.~Bullet();
				break;
			}

			glm::mat4 transbullet = bullet.Move(deltaTime);

			//transform matrix
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transbullet));

			//color
			unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
			glm::vec4 color = bullet.GetColor();
			glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(0);

		//Spaceship

		//bind VAO
		glBindVertexArray(navevao);

		//Color change effect while taking damage
		if (nave.Timer(glfwGetTime())) nave.ChangeColor();

		
		//transform matrix
		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transnave));

		//color
		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = nave.GetColor();
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		//Asteroids
		
		
		glm::vec3 posnave = nave.GetPosition();


		for (Asteroide& as : aslist) {

			//Collision with spaceship
			if (Collide(as.GetPosition(), as.GetHitbox(), nave.GetPosition(), nave.GetHitbox())) {
				if (nave.Damage(10 * as.GetTipo())) GameOver();
				nave.SetTimer(glfwGetTime());
				nave.ChangeColor();
				aslist.remove(as);
				as.~Asteroide();
				break;

			}
			//Collision with Bullets
			for (Bullet& bullet : bulletlist) {
				if (Collide(as.GetPosition(), as.GetHitbox(), bullet.GetPosition(), bullet.GetHitbox())) {
					bulletlist.remove(bullet);
					bullet.~Bullet();
					if (as.Damage(10)) {
						aslist.remove(as);
						as.~Asteroide();
						goto Next;
					}
					as.SetTimer(glfwGetTime());
					as.ChangeColor();
					break;
				}
			}

			//bind VAO
			glBindVertexArray(0);
			if (as.GetForma() == 0) glBindVertexArray(asvao);
			if (as.GetForma() == 1) glBindVertexArray(asvao2);


			glm::mat4 transas;


			transas = as.Move(posnave, deltaTime);

			if (as.Timer(glfwGetTime())) as.ChangeColor();

			unsigned int transformLoc3 = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(transas));

			unsigned int transformLoc4 = glGetUniformLocation(programID, "color");
			glm::vec4 color2 = as.GetColor();
			glUniform4fv(transformLoc4, 1, glm::value_ptr(color2));



			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		}
	
	}

	// Cleanup
	glDeleteBuffers(1, &navevbo);
	glDeleteBuffers(1, &naveibo);
	glDeleteVertexArrays(1, &navevao);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &asvbo);
	glDeleteBuffers(1, &asibo);
	glDeleteVertexArrays(1, &asvao);
	glDeleteBuffers(1, &asvbo2);
	glDeleteBuffers(1, &asibo2);
	glDeleteVertexArrays(1, &asvao2);
	glDeleteBuffers(1, &bulletvbo);
	glDeleteBuffers(1, &bulletibo);
	glDeleteVertexArrays(1, &bulletvao);
	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


