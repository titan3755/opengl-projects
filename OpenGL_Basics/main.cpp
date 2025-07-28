// chess board pattern with OpenGL
// titan3755->github

// ogl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <vector>

// local_headers
#include <shader_manager.hpp>

const int WIDTH = 800;
const int HEIGHT = 800;
const int BOARD_SIZE = 8;
const float SQUARE_SIZE = 2.0f / BOARD_SIZE;
const float BOARD_START_X = -1.0f;
const float BOARD_START_Y = -1.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

	std::cout << "OpenGL Basics - Initializing..." << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Basics", nullptr, nullptr);
    if (window == nullptr) {
		std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

	// shader compilation
	ShaderManager shaderManager("shaders/vertex.glsl", "shaders/fragment.glsl");

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // hexagon vertices and indices
    float vertices[] = {
        0.0f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,
        0.25f, 0.433013f, 0.0f,
        -0.25f, 0.433013f, 0.0f,
        -0.5f, 0.0f,  0.0f,
        -0.25f, -0.433013f, 0.0f,
        0.25f, -0.433013f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 3, 4, 5, 6, 1
    };

    // rendering setups
    unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
	shaderManager.use();

	// ogl info
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Reloaded initialized successfully!" << std::endl;

    // RDLP
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		shaderManager.use();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean
    glfwDestroyWindow(window);
	glfwTerminate();
    
	return 0;
}