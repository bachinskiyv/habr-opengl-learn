#include "SystemProhjections18.h"

const GLfloat FOV = 45.f;

void Lesson18::Begin()
{
	glm::mat4 proj = glm::perspective(FOV, 800.f / 600.f, .1f, 100.f);
}

void Lesson18::Update()
{

}
