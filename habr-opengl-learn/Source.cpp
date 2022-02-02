#include "Common.h"
#include "HelloTriangle14.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


int main() {

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW system!" << std::endl;
		return 95;
	}

	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// создаем окно
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/*
	 Далее мы создаем контекст окна, который будет основным контекстом в данном потоке.
	*/
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // Новейщее у них вооружение!
	if (glewInit() != GLEW_OK) // Будем точно знать
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	/*Первые 2 аргумента функции glViewport — это позиция нижнего левого угла окна. 
	Третий и четвертый — это ширина и высота отрисовываемого окна в px, 
	которые мы получаем напрямую из GLFW. Вместо того, чтобы руками задавать 
	значения ширины и высоты в 800 и 600 соответственно мы будем использовать значения из GLFW, 
	поскольку такой алгоритм также работает и на экранах с большим DPI (как Apple Retina).*/
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);	
	glViewport(0, 0, width, height);

	/*
	За кулисами OpenGL использует данные, переданные через glViewport для преобразования 2D координат 
	в координаты экрана. К примеру позиция (-0.5, 0.5) в результате будет преобразована в (200, 450). 
	Заметьте, что обрабатываемые координаты OpenGL находятся в промежутке от -1 до 1, 
	соответственно мы можем эффективно преобразовывать из диапазона (-1, 1) в (0,800) и (0,600).
	*/

	glfwSetKeyCallback(window, key_callback); // bind close key

	Lesson14::Begin();

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// команды отрисовки здесь
		Lesson14::Update();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
