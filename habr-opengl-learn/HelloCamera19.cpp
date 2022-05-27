#include "HelloCamera19.h"
#include "MaterialWithMesh.h"
#include "Camera.h"

static GLuint VAO;
static GLuint EBO;
static GLfloat FOV = 45.f;
static Camera camera;

static glm::vec3 cubesPositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


class FirstCubeMeshNMaterial : public MaterialWithMesh {
protected:
    const std::vector<GLfloat> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

public:
    virtual void LoadShader() {
        LoadShaderImpl("shader-1.8-vertexProjections3DCube.glsl", "shader-1.8-fragment3DCube.glsl");
    }

    virtual void FillVerticesBuffers() {
        /*
         * GL_STATIC_DRAW: данные либо никогда не будут изменяться, либо будут изменяться очень редко;
           GL_DYNAMIC_DRAW: данные будут меняться довольно часто;
           GL_STREAM_DRAW: данные будут меняться при каждой отрисовке.
         */
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    }

    virtual void DrawShape() override {
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    virtual void SetupVerticesData() override
    {
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
        this->FillVerticesBuffers();

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        // Явно включаем аттрибут
        glEnableVertexAttribArray(0);

        /*
        * layout(position=1),
        * vec2 = 2,
        * ...,
        * ...,
        * ...,
        * (GLvoid*)(6 * sizeof(GLfloat)) - - Указатель на величину отступа
        */
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        // Явно включаем аттрибут
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

static MaterialWithMesh* materialWithMeshObject;
static DeltaTime deltaTime;

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

static void TickFor3DCube() {
    materialWithMeshObject->UseShaderProgram();

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


    // MATRICES
    // Матрица модели: поворачиваем по X на -55 градусов
    glm::mat4 model = glm::mat4(1.f);
    //Зададим тут вращение, тк это 3d-модель
    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    // Матрица вида:смещаем по Z на -3, так как хотим камера отъехала назад (А ПО ФАКТУ ЧТОБЫ СЦЕНА ОТЪЕХАЛА ВПЕРЕД).
    // NOTE: Так как у нас правосторонняя система координат, то в сторону перспективы идет -Z
    // Далее, мы в проекциии вида двигаем сцену, а не камеру и именно по-этому мы двигаем матрицы в сторону -Z (отодвигаем кубик)
    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

    // Матрица проекции: сгенерируется в GLM
    glm::mat4 projection = glm::perspective(FOV, 800.f / 600.f, .1f, 100.f);


    // Первый аргумент должен быть позицией переменной.
    // Второй аргумент сообщает OpenGL сколько матриц мы собираемся отправлять, в нашем случае 1.
    // Третий аргумент говорит требуется ли транспонировать матрицу. OpenGL разработчики часто используют внутренних матричный формат, называемый column-major ordering, который используется в GLM по умолчанию, поэтому нам не требуется транспонировать матрицы, мы можем оставить GL_FALSE.
    // Последний параметр — это, собственно, данные, но GLM не хранит данные точно так как OpenGL хочет их видеть, поэтому мы преобразовываем их с помощью value_ptr.
    glUniformMatrix4fv(materialWithMeshObject->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(materialWithMeshObject->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(materialWithMeshObject->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    materialWithMeshObject->DrawShape();
    glBindVertexArray(0);
}

static glm::mat4 GetViewMatrixOnlyForRotation()
{
    // NEW: Используем матрицу вида для контроля за камерой - вращаем вокруг кубов
    GLfloat radius = 10.0f; // Радиус вращения
    GLfloat camX = sin(glfwGetTime()) * radius; // Вычисляем координату точки по X
    GLfloat camZ = cos(glfwGetTime()) * radius; // Вычисляем координату точку по Z
    // Интересно, почему это Z, а не Y. Камера лежит на боку?
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(
        glm::vec3(camX, 0.f, camZ), // Позиция камеры в каждом кадре
        glm::vec3(0.f), // прицелимся в начало координат
        glm::vec3(0.f, 1.f, 0.f) // upvector
    );

    return view;
}

// Устанавливаем камеру в определенную позицию для free look
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// Форвард вектор с учетом того что камера смотрит назад *BRAIN_BOOM*
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// Upvector для нахождения правого вектора
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static glm::mat4 GetViewMatrixForKeyboardTravelling()
{
    glm::mat4 view = glm::mat4(1.f);
    return glm::lookAt(
        cameraPos,
        cameraPos + cameraFront,
        cameraUp
    );
}

// Середина координат экрана
static GLfloat lastX = 400.f, lastY = 300.f;

// Хранение тангажа и рыскания
static GLfloat yaw = -90.f; // Опять минус из-за того что смотрим назад
static GLfloat pitch = 0.f;

static glm::mat4 GetViewMatrixForFreeLook()
{
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }  

    // @TODO: Докопать тригонометрию

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);

    glm::vec3 target = cameraPos + cameraFront;

    
    printf(
        "CameraPos: (%f, %f, %f); CameraFront: (%f, %f, %f); Sum: (x: %f,  y:%f, z: %f); Yaw: %f; Pitch: %f; Front: (%f, %f, %f)\n", 
        cameraPos.x, cameraPos.y, cameraPos.z, 
        front.x, front.y, front.z,
        target.x, target.y, target.z,
        yaw, pitch,
        front.x, front.y, front.z
    ); 

    return glm::lookAt(
        cameraPos, // camera
        target, // target
        cameraUp
    );
}

void Lesson19::Begin()
{
    materialWithMeshObject = new FirstCubeMeshNMaterial();
    materialWithMeshObject->LoadShader();
    materialWithMeshObject->SetupVerticesData();

    LoadTwoTextures();

	// Для того чтобы понять куда смотрит камера нам нужно вычесть ( cameraTarget - cameraPos )
	// Мы получим направление из позиции камеры в таргет
	glm::vec3 cameraPos = glm::vec3(.0f, .0f, 3.f);
	glm::vec3 cameraTarget = glm::vec3(0.f);
	// Нормализованный вектор, !обратный! направлению камеры, тк у нас z-axis (глубина) - отрицательная
	glm::vec3 cameraForward = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	// Делаем векторное произведение - получаем правый вектор камеры (порядок имеет значение)
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraForward));
	// У нас есть форвардвектор камеры и райтвектор, мы получаем ее настоящий up вектор
	// Нормализация не требуется тк, оба входных вектора нормализованы
	glm::vec3 cameraUp = glm::cross(cameraForward, cameraRight);

	// @NOTE!!!: Мы переворачваем Z у камеры, 
	// тк по соглашению OpenGL она смотрит назад относительно своих координат

	// Матрица LookAt - преобразует глобальные координаты в координаты камеры/вида
	// [ Rx Ry Rz 0 ]   [ 1 0 0 -Px ]
	// [ Ux Uy Uz 0 ] * [ 0 1 0 -Py ]
	// [ Fx Fy Fz 0 ]   [ 0 0 0 -Pz ]
	// [ 0  0  0  1 ]   [ 0 0 0  1  ]

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.f, 0.f, 3.f), // camera position
		glm::vec3(0.f), // target
		glm::vec3(0.f, 1.f, 0.f) // auxillary up vector, needs for right vector
	);
}

void doMovement();

void Lesson19::Update()
{
    deltaTime.UpdateDeltaTime();

    doMovement();

    materialWithMeshObject->UseShaderProgram();

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


    // MATRICES
    // Матрица модели: поворачиваем по X на -55 градусов
    glm::mat4 model = glm::mat4(1.f);
    //Зададим тут вращение, тк это 3d-модель
    model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    //  Матрица вида
    // glm::mat4 view = GetViewMatrixOnlyForRotation();
    //glm::mat4 view = GetViewMatrixForKeyboardTravelling();
    glm::mat4 view = GetViewMatrixForFreeLook();

    // Матрица проекции: сгенерируется в GLM
    glm::mat4 projection = glm::perspective(FOV, 800.f / 600.f, .1f, 100.f);

    // Первый аргумент должен быть позицией переменной.
    // Второй аргумент сообщает OpenGL сколько матриц мы собираемся отправлять, в нашем случае 1.
    // Третий аргумент говорит требуется ли транспонировать матрицу. OpenGL разработчики часто используют внутренних матричный формат, называемый column-major ordering, который используется в GLM по умолчанию, поэтому нам не требуется транспонировать матрицы, мы можем оставить GL_FALSE.
    // Последний параметр — это, собственно, данные, но GLM не хранит данные точно так как OpenGL хочет их видеть, поэтому мы преобразовываем их с помощью value_ptr.
    glUniformMatrix4fv(materialWithMeshObject->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(materialWithMeshObject->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    GLint modelLocation = materialWithMeshObject->GetUniformLocation("model");

    glBindVertexArray(VAO);

    int i = -1;
    for (const glm::vec3& position : cubesPositions) {
        // Calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, cubesPositions[++i]);
        GLfloat angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        materialWithMeshObject->DrawShape();
    }

    glBindVertexArray(0);
}


static bool keys[1024];

void Lesson19::KeyCallback(int key, int action)
{
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

static const GLfloat sensitivity = .05f;

void Lesson19::UpdateMousePosition(double xpos, double ypos)
{
    // Значения xpos ypos не нормализованы, тк нет ограничения угла
    // std::cout << "xpos: " << xpos << "; ypos: " << ypos << std::endl;
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;
}

static const GLfloat sensivityScroll = .1f;
void Lesson19::UpdateScrollOffset(double xoffset, double yoffset)
{
    if (FOV >= 1.f && FOV <= 45.f) {
        FOV -= yoffset * sensivityScroll;
    }
    if (FOV <= 1.f) {
        FOV = 1.f;
    }
    if (FOV >= 45.f) {
        FOV = 45.f;
    }
}

static inline glm::vec3 strafeDelta(GLfloat CameraSpeed) {
    return glm::normalize(glm::cross(cameraFront, cameraUp)) * CameraSpeed;
}

static void doMovement()
{
    GLfloat CameraSpeed = deltaTime * 5.f;

    if (keys[GLFW_KEY_W]) {
        cameraPos += CameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_S]) {
        cameraPos -= CameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_A]) {
        // двигаемся стрейфом
        // нормализуем вектора, тк при изменении направления вектор может быть больше 1
        // @TODO: проверить
        cameraPos -= strafeDelta(CameraSpeed);
    }
    if (keys[GLFW_KEY_D]) {
        cameraPos += strafeDelta(CameraSpeed);
    }
}
