#include "HelloShaders15.h"
#include <vector>
#include "MaterialWithMesh.h"

const GLchar* vertexShader = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"  // Устанавливаем позицию атрибута в 0
"out vec4 vertexColor;\n"// Передаем цвет во фрагментный шейдер
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n" // Напрямую передаем vec3 в vec4
"vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n" // Устанавливаем значение выходной переменной в темно-красный цвет.
"}\n";

const GLchar* fragmentShaderRedTriangle = "#version 330 core\n"
"in vec4 vertexColor;\n" // Входная переменная из вершинного шейдера (то же название и тот же тип)
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vertexColor;\n"
"}\n";

const GLchar* fragmentShaderUniform = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n" // Мы устанавливаем значение этой переменной в коде OpenGL.
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n";

static GLuint shaderProgram;

static GLuint VAO;
static GLuint EBO;


static class SetupVertices : public MaterialWithMesh  {
public:
	virtual const GLchar* getVertexShaderSource() const override {
		return vertexShader;
	}

	virtual const GLchar* getFragmentShaderSource() const override {
		return fragmentShaderUniform;
	}
};

static class SetupQuadVertices : public SetupVertices {
protected:
	bool isWireframe = false;

	const GLfloat quadVertices[12] = {
		 0.5f,  0.5f, 0.0f,  // Верхний правый угол
		 0.5f, -0.5f, 0.0f,  // Нижний правый угол
		-0.5f, -0.5f, 0.0f,  // Нижний левый угол
		-0.5f,  0.5f, 0.0f   // Верхний левый угол
	};

	const GLuint quadIndicies[6] = {
		 0, 1, 3,   // Первый треугольник
		 1, 2, 3    // Второй треугольник
	};

public:
	virtual void DrawShape() override {
		if (isWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		/*
			Первый аргумент описывает примитив, который мы хотим отрисовать, также как и в glDrawArrays.
			Второй аргумент — это количество элементов, которое мы хотим отрисовать. Мы указали 6 индексов, поэтому мы передаем функции 6 вершин.
			Третий аргумент — это тип данных индексов, в нашем случае — это GL_UNSIGNED_INT.
			Последний аргумент позволяет задать нам смещение в EBO (или передать сам массив с индексами, но при использовании EBO так не делают), поэтому мы указываем просто 0.
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	virtual void SetupVerticies() override {
		/*
		* GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
		  GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
		  GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
		*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndicies), quadIndicies, GL_STATIC_DRAW);
	}
};

static class SetupTriangleVertices : public SetupVertices {
protected:
	const GLfloat triangleVertices[9] = {
		// 1st vertex
		-0.5f, -0.5f, 0.0f,
		// 2nd vertex
		0.5f, -0.5f, 0.0f,
		// 3rd vertex
		0.0f, 0.5f, 0.0f
	};
public:
	virtual void SetupVerticies() override {
		std::vector<GLfloat> verticesVector = getVerticesPointer();

		/*
		* GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
		  GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
		  GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
		*/
		glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(GLfloat), &verticesVector[0], GL_STATIC_DRAW);
	}

	virtual void DrawShape() override {
		/*
		* GL_TRIANGLES - draw triangle
		* 0 - starting vertices index
		* 3 - vertices count
		*/
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	virtual std::vector<float> getVerticesPointer() const {
		std::vector<float> out;
		out.assign(triangleVertices, std::end(triangleVertices));
		return out;
	}
};

















/**  WITH COLORS!  */
/*
const GLchar* vertexWithColoredTriangleVertexShader = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n" // Устанавливаем позиция переменной с координатами в 0
"layout(location = 1) in vec3 color;\n"    // А позицию переменной с цветом в 1
"out vec3 ourColor;\n" // Передаем цвет во фрагментный шейдер
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n" // Устанавливаем значение цвета, полученное от вершинных данных
"}\n";

const GLchar* coloredFragmentShader = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n";
*/



static class SetupTriangleVerticesWithColors : public SetupTriangleVertices {
protected:
	const GLfloat verticesWithColors[18] = {
		// Позиции         // Цвета
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Нижний правый угол
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Нижний левый угол
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Верхний угол
	};
public:
	virtual void LoadShader() {
		LoadShaderImpl("shader1.5-triangleWithColoredVertexShader.glsl", "shader1.5-coloredFragmentShader.glsl");
	}

	virtual std::vector<GLfloat> getVerticesPointer() const override {
		std::vector<GLfloat> out;
		out.assign(verticesWithColors, std::end(verticesWithColors));
		return out;
	}
	/*
	virtual const GLchar* getVertexShaderSource() override {
		return vertexWithColoredTriangleVertexShader;
	}

	virtual const GLchar* getFragmentShaderSource() override {
		return coloredFragmentShader;
	}*/
};






static SetupVertices* setupVertices = new SetupTriangleVerticesWithColors();




static void SetupVerticesData() {
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
	// =================================   Bind Vertex array FIRST =================================
	glBindVertexArray(VAO);

	/*
	 * Создаем просто буфер типа ARRAY (с сырыми данными?) и складываем туда вертексы
	 */
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Then...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// В зависимости от фигуры - вызываем отрисовку
	setupVertices->SetupVerticies();

	// Размечаем память в шейдере
	/**
		Первый аргумент описывает какой аргумент шейдера мы хотим настроить. Мы хотим специфицировать значение аргумента position, позиция которого была указана следующим образом: layout (location = 0).
		Следующий аргумент описывает размер аргумента в шейдере. Поскольку мы использовали vec3 то мы указываем 3.
		Третий аргумент описывает используемый тип данных. Мы указываем GL_FLOAT, поскольку vec в шейдере использует числа с плавающей точкой.
		Четвертый аргумент указывает необходимость нормализовать входные данные. Если мы укажем GL_TRUE, то все данные будут расположены между 0 (-1 для знаковых значений) и 1. Нам нормализация не требуется, поэтому мы оставляем GL_FALSE;
		Пятый аргумент называется шагом и описывает расстояние между наборами данных. Мы также можем указать шаг равный 0 и тогда OpenGL высчитает шаг (работает только с плотно упакованными наборами данных). Как выручить существенную пользу от этого аргумента мы рассмотрим позже.
		Последний параметр имеет тип GLvoid* и поэтому требует такое странное приведение типов. Это смещение начала данных в буфере. У нас буфер не имеет смещения и поэтому мы указываем 0.
	*/
	/*
	layout(position=0),
	vec3 = 3,
	GLFLOAT - тк вектор с флоатами,
	GL_FALSE - не нормализовывать,
	6 * sizeof(GLfloat) - ШАГ (или общее кол-во данных для вершины) - шесть флоатов, тк у нас 3 на позицию + 3 на цвет,
	(GLvoid*)0 - Смещение - 0, тк мы читаем с нуля три флоат вершины и после этого перепрыгиваем на следующий шаг: (0-12, 24-36, 48-60, ...)
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	// Явно включаем аттрибут
	glEnableVertexAttribArray(0);
	/*
	* layout(position=1),
	* ...,
	* ...,
	* ...,
	* ...,
	* (GLvoid*)(3 * sizeof(GLfloat)) - Указатель  на
	*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	// Явно включаем аттрибут
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


































// endregion WITH_COLORS



/**
GLuint CreateVertexShader() {
	// Create shader handle
	GLuint vertexShaderHandle;
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource = setupVertices->getVertexShaderSource();

	// Compile shader source
	glShaderSource(vertexShaderHandle, 1, &vertexShaderSource, NULL);
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


static GLuint CreateFragmentShader() {
	const GLchar* fragmentShaderSource = setupVertices->getFragmentShaderSource();

	GLuint fragmentShaderHandle;
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderHandle, 1, &fragmentShaderSource, NULL);
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

static GLuint CreateShaderProgram(GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
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
*/


static void SetupVerticesDataOnlyVertices() {
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
	// =================================   Bind Vertex array FIRST =================================
	glBindVertexArray(VAO);

	/*
	 * Создаем просто буфер типа ARRAY (с сырыми данными?) и складываем туда вертексы
	 */
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Then...
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// В зависимости от фигуры - вызываем отрисовку
	setupVertices->SetupVerticies();

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
	// Явно включаем аттрибут
	glEnableVertexAttribArray(0);

	// THEN UNBIND VERTEX ARRAY CTX
	glBindVertexArray(0);
}



void Lesson15::Begin() {

	setupVertices->LoadShader();
	/*
	// Создаем шейдеры
	GLuint vertexShaderHandle = CreateVertexShader();
	GLuint fragmentShaderHandle = CreateFragmentShader();

	if (!vertexShaderHandle || !fragmentShaderHandle) {
		return;
	}

	// Создаем шейдер-программу
	shaderProgram = CreateShaderProgram(vertexShaderHandle, fragmentShaderHandle);

	// Удаялем шейдеры после линковки
	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);
	*/

	// Bootstrap вершин
	SetupVerticesData();
}

void Lesson15::Update() {
	GLfloat timeValue = glfwGetTime();
	GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
	// Обновляем цвет в шейдере через uniform переменную
	GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	// Для того чтобы установить uniform-переменную нам нужно сначала определить, с какой программой мы имеем дело
	setupVertices->UseShaderProgram();
	//glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	glBindVertexArray(VAO);
	setupVertices->DrawShape();
	glBindVertexArray(0);
}





