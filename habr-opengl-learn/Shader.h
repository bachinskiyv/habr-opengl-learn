#pragma once
#include "Common.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class Shader
{
public:
    // Идентификатор программы
    GLuint Program;

    // Конструктор считывает и собирает шейдер
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    // Использование программы
    void Use();
};

