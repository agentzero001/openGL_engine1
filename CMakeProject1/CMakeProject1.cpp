#include "Context.h"

using namespace std;





int main() {

	Context* context;
	GLFWwindow* window;

	int WIDTH = 1080;
	int HEIGHT = 1080;
	std::cout << "hola" << std::endl;
	if (!glfwInit()) { exit(EXIT_FAILURE); };
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	

	window = glfwCreateWindow(WIDTH, HEIGHT, "whatever", nullptr, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {exit(EXIT_FAILURE);}
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	context = new Context(window);
	try {
		context->run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
