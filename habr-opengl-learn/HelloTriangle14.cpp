#include "HelloTriangle14.h"

const GLchar* VertexShader14 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* FragmentShader14 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const GLfloat triangleVertices[] = {
	// 1st vertex
	-0.5f, -0.5f, 0.0f,
	// 2nd vertex
	0.5f, -0.5f, 0.0f,
	// 3rd vertex
	0.0f, 0.5f, 0.0f
};

GLuint CreateVertexShader14() {
	// Create shader handle
	GLuint vertexShaderHandle;
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

	// Compile shader source
	glShaderSource(vertexShaderHandle, 1, &VertexShader14, NULL);
	glCompileShader(vertexShaderHandle);

	// Check compilation errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShaderHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;

		return 0;
	}

	return vertexShaderHandle;
}


GLuint CreateFragmentShader14() {
	GLuint fragmentShaderHandle;
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderHandle, 1, &FragmentShader14, NULL);
	glCompileShader(fragmentShaderHandle);

	// Check compilation errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << infoLog << std::endl;

		return 0;
	}

	return fragmentShaderHandle;
}

GLuint CreateShaderProgram14(GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
	// Create a shader program
	GLuint shaderProgram = glCreateProgram();

	// Attach shaders to it
	glAttachShader(shaderProgram, vertexShaderHandle);
	glAttachShader(shaderProgram, fragmentShaderHandle);
	// Link(?) program (?Linker?)
	glLinkProgram(shaderProgram);

	// Check linker errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		return 0;
	}

	return shaderProgram;
}

static GLuint VAO;
static GLuint shaderProgram;

void SetupVerticesData() {
	/**
	* Vertex Array Object

	Объект вершинного массива (VAO) может быть также привязан как и VBO и после этого все последующие вызовы вершинных атрибутов будут храниться в VAO. Преимущество этого метода в том, что нам требуется настроить атрибуты лишь единожды, а все последующие разы будет использована конфигурация VAO. Также такой метод упрощает смену вершинных данных и конфигураций атрибутов простым привязыванием различных VAO.
	Core OpenGL требует чтобы мы использовали VAO для того чтобы OpenGL знал как работать с нашими входными вершинами. Если мы не укажем VAO, OpenGL может отказаться отрисовывать что-либо.

	VAO хранит следующие вызовы:
		* Вызовы glEnableVertexAttribArray или glDisableVertexAttribArray.
		* Конфигурация атрибутов, выполненная через glVertexAttribPointer.
		* VBO ассоциированные с вершинными атрибутами с помощью glVertexAttribPointer
	*/
	glGenVertexArrays(1, &VAO);
	/*
	 * Создаем просто буфер типа ARRAY (с сырыми данными?) и складываем туда вертексы
	 */
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// =================================   Bind Vertex array FIRST =================================
	glBindVertexArray(VAO);

	// Then...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
	* GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
	  GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
	  GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	// Размечаем память в шейдере
	/**
		Первый аргумент описывает какой аргумент шейдера мы хотим настроить. Мы хотим специфицировать значение аргумента position, позиция которого была указана следующим образом: layout (location = 0).
		Следующий аргумент описывает размер аргумента в шейдере. Поскольку мы использовали vec3 то мы указываем 3.
		Третий аргумент описывает используемый тип данных. Мы указываем GL_FLOAT, поскольку vec в шейдере использует числа с плавающей точкой.
		Четвертый аргумент указывает необходимость нормализовать входные данные. Если мы укажем GL_TRUE, то все данные будут расположены между 0 (-1 для знаковых значений) и 1. Нам нормализация не требуется, поэтому мы оставляем GL_FALSE;
		Пятый аргумент называется шагом и описывает расстояние между наборами данных. Мы также можем указать шаг равный 0 и тогда OpenGL высчитает шаг (работает только с плотно упакованными наборами данных). Как выручить существенную пользу от этого аргумента мы рассмотрим позже.
		Последний параметр имеет тип GLvoid* и поэтому требует такое странное приведение типов. Это смещение начала данных в буфере. У нас буфер не имеет смещения и поэтому мы указываем 0.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// THEN UNBIND VERTEX ARRAY CTX
	glBindVertexArray(0);
}



void Lesson14::Begin() {
	// Создаем шейдеры
	GLuint vertexShaderHandle = CreateVertexShader14();
	GLuint fragmentShaderHandle = CreateFragmentShader14();

	if (!vertexShaderHandle || !fragmentShaderHandle) {
		return;
	}

	// Создаем шейдер-программу
	shaderProgram = CreateShaderProgram14(vertexShaderHandle, fragmentShaderHandle);

	// Удаялем шейдеры после линковки
	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);

	// Bootstrap вершин
	SetupVerticesData();
}

void Lesson14::Update() {
	// Run shader program in gameloop
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	/*
	* GL_TRIANGLES - draw triangle
	* 0 - starting vertices index
	* 3 - vertices count
	*/
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
