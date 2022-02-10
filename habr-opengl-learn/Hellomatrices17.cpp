#include "Hellomatrices17.h"
#include "MaterialWithMesh.h"

static GLuint VAO;
static GLuint EBO;

class TriangleWithTexMaterialNoStaticClass : public MaterialWithMesh {
protected:
    const std::vector<GLfloat> vertices = {
        // Позиции          // Цвета             // Текстурные координаты
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };

    const GLuint quadIndicies[6] = {
         0, 1, 3,   // Первый треугольник
         1, 2, 3    // Второй треугольник
    };

public:
    virtual void LoadShader() {
        LoadShaderImpl("shader-1.7-vertexWithMatrix.glsl", "shader-16-fragmentTexQuadMix.glsl");
    }

    virtual void FillVerticesBuffers() {
        /*
         * GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
           GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
           GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
         */
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndicies), quadIndicies, GL_STATIC_DRAW);
    }

    virtual void DrawShape() override {
        /*
            Первый аргумент описывает примитив, который мы хотим отрисовать, также как и в glDrawArrays.
            Второй аргумент — это количество элементов, которое мы хотим отрисовать. Мы указали 6 индексов, поэтому мы передаем функции 6 вершин.
            Третий аргумент — это тип данных индексов, в нашем случае — это GL_UNSIGNED_INT.
            Последний аргумент позволяет задать нам смещение в EBO (или передать сам массив с индексами, но при использовании EBO так не делают), поэтому мы указываем просто 0.
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};

static TriangleWithTexMaterialNoStaticClass* materialWithMeshObject = new TriangleWithTexMaterialNoStaticClass();


static GLuint texture1;
static GLuint texture2;

static void LoadTwoTextures() {
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Tex wrapping
     // STR = XYZ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // if parameter == GL_CLAMP_TO_BORDER
    // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Фильтрация текстур
    // The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image("Resources/Images/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);



    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Tex wrapping
    // STR = XYZ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Фильтрация текстур
    // The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = SOIL_load_image("Resources/Images/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}


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
    materialWithMeshObject->FillVerticesBuffers();

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
    8 * sizeof(GLfloat) - ШАГ (или общее кол-во данных для вершины) - шесть флоатов, тк у нас 3 на позицию + 3 на цвет,
    (GLvoid*)0 - Смещение - 0, тк мы читаем с нуля три флоат вершины и после этого перепрыгиваем на следующий шаг: (0-12, 24-36, 48-60, ...)
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    // Явно включаем аттрибут
    glEnableVertexAttribArray(0);
    /*
    * layout(position=1),
    * ...,
    * ...,
    * ...,
    * ...,
    * (GLvoid*)(3 * sizeof(GLfloat)) - Указатель на величину отступа
    */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // Явно включаем аттрибут
    glEnableVertexAttribArray(1);


    /*
    * layout(position=2),
    * vec2 = 2,
    * ...,
    * ...,
    * ...,
    * (GLvoid*)(6 * sizeof(GLfloat)) - - Указатель на величину отступа
    */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    // Явно включаем аттрибут
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


void Lesson17::Begin() {
    materialWithMeshObject->LoadShader();

    SetupVerticesData();

    LoadTwoTextures();

	glm::vec4 vec(1.f, 0.f, 0.f, 1.f);
	glm::mat4 trans = glm::mat4(1.f);

	// Translate
	trans = glm::translate(trans, glm::vec3(1.f, 1.f, 0.f));
	vec = trans * vec;

	trans = glm::rotate(trans, 90.f, glm::vec3(0.f, 0.f, 1.f));
    trans = glm::scale(trans, glm::vec3(.5f));
}

void Lesson17::Update() {
    materialWithMeshObject->UseShaderProgram();

    // glBindTexture(GL_TEXTURE_2D, texture);

     // Активируем текстурный блок перед привязкой текстуры
    glActiveTexture(GL_TEXTURE0);
    // Привязываем нужную текстуру
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Привязываем текстурный блок 0 к его uniform-переменной
    // glUniform1i and glUniform1iv are the only two functions that may be used to load uniform variables defined as sampler types.
    GLint texture1Sampler2Dlocation = materialWithMeshObject->GetUniformLocation("ourTexture1");
    glUniform1i(texture1Sampler2Dlocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(materialWithMeshObject->GetUniformLocation("ourTexture2"), 1);
    // Костыль, для того чтобы не дублировать фрагментный шейдер в этом уроке
    glUniform1f(materialWithMeshObject->GetUniformLocation("alpha"), .2f);

    // Перетаскиваем кубик в нижний правый угол и крутим там
    glm::mat4 trans = glm::mat4(1.f);
    /*
    * Матричные операции не коммутативны
    * 
    * Если поменять местами вот эти две операции, то сначала будет выполнен поворот на определенный угол
    * затем сдвиг повернутого изображения по матрице вниз и вправо, а так как
    * поворот поменял базис? (Есть более умное объясенение с базисами)
    */
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    GLuint transformShaderLocation = materialWithMeshObject->GetUniformLocation("transform");
    /*
        Первый аргумент должен быть позицией переменной.
        Второй аргумент сообщает OpenGL сколько матриц мы собираемся отправлять, в нашем случае 1.
        Третий аргумент говорит требуется ли транспонировать матрицу. OpenGL разработчики часто используют внутренних матричный формат, называемый column-major ordering, который используется в GLM по умолчанию, поэтому нам не требуется транспонировать матрицы, мы можем оставить GL_FALSE.
        Последний параметр — это, собственно, данные, но GLM не хранит данные точно так как OpenGL хочет их видеть, поэтому мы преобразовываем их с помощью value_ptr.
    */
    glUniformMatrix4fv(transformShaderLocation, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(VAO);
    materialWithMeshObject->DrawShape();
    glBindVertexArray(0);
}
