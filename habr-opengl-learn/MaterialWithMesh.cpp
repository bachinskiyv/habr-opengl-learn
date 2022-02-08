#include "MaterialWithMesh.h"

void MaterialWithMesh::LoadShaderImpl(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath) {
	shader = new Shader(vertexShaderPath, fragmentShaderPath);
}

void MaterialWithMesh::UseShaderProgram() {
	shader->Use();
}

GLint MaterialWithMesh::GetUniformLocation(const GLchar* parameter) const {
	return glGetUniformLocation(shader->Program, parameter);
}

