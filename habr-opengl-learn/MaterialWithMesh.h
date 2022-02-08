#pragma once

#include "Common.h"
#include "Shader.h"
#include <vector>

class MaterialWithMesh
{
public:
	virtual void DrawShape() = 0;

	virtual const GLchar* getVertexShaderSource() const
	{
		return "";
	}

	virtual const GLchar* getFragmentShaderSource() const
	{
		return "";
	}

	virtual void SetupVerticies() = 0;

	virtual void LoadShader() = 0;

	virtual void UseShaderProgram();

	GLint GetUniformLocation(const GLchar* parameter) const;

protected:
	Shader* shader = nullptr;

	void LoadShaderImpl(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
};

