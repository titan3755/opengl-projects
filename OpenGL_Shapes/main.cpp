// ogl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <vector>

// local
#include <shader_manager.hpp>

const int WIDTH = 1920;
const int HEIGHT = 1080;

// fsc
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// main
int main() {

    std::cout << "OpenGL Shapes - Initializing..." << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Shapes", nullptr, nullptr);
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

    ShaderManager shaderManager("shaders/vertex.glsl", "shaders/fragment.glsl");
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // vertex and index gen
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

	// triangle, rectangle, and square vertices
    vertices = {
        // positions          // colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top
		 // rectangle vertices
        -0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f,  0.5f, 1.0f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // top left
		// square vertices
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f, // bottom left
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f // top left
	};
    indices = {
        0, 1, 2, // triangle
		3, 4, 5, // rectangle
		3, 5, 6, // rectangle
		3, 6, 7, // rectangle
		8, 9, 10, // square
		8, 10, 11, // square
		8, 11, 12, // square
        8, 12, 13 // square 
	};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    shaderManager.use();

    // ogl info
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Shapes initialized successfully!" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderManager.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}