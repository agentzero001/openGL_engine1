#include "Context.h"

using namespace std;





int main() {

	Context* context;
	GLFWwindow* window;

	int WIDTH = 800;
	int HEIGHT = 800;
	std::cout << "hola" << std::endl;
	if (!glfwInit()) { exit(EXIT_FAILURE); };
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(WIDTH, HEIGHT, "whatever", NULL, NULL);
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
