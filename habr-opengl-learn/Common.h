#pragma once

#include <iostream>

// Glew and glfw - BASE!
#define GLEW_STATIC
#include <glew.h> 
#include <glfw3.h>

// Matrices
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Images
#include "soil.h"

// Common Utils
struct DeltaTime {
    void UpdateDeltaTime()
    {
        GLfloat currentFrame = glfwGetTime();
        value = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;
    }

    GLfloat operator * (const GLfloat& Rhs) const {
        return Rhs * value;
    }

protected:
    GLfloat value = 0.f;
    GLfloat lastFrameTime = 0.f;
};
