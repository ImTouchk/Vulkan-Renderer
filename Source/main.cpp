#include <GLFW\glfw3.h>

int main() {
	glfwInit();

	GLFWwindow* Window = glfwCreateWindow(800, 600, "Hello", nullptr, nullptr);

	while (!glfwWindowShouldClose(Window)) {
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 1;
}