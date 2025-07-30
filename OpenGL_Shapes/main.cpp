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
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

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
        // separator rectangle
		-1.0f, 0.01f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
		1.0f, 0.01f, 0.0f, 0.0f, 0.0f, 0.0f, // top right
		1.0f, -0.01f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom right
		-1.0f, -0.01f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        // first portion
        // triangle
		-0.75f, 0.7f, 0.0f, 1.0f, 0.0f, 0.0f, // top vertex
		-0.60f, 0.20f, 0.0f, 1.0f, 0.0f, 0.0f, // right vertex
        -0.90f, 0.20f, 0.0f, 1.0f, 0.0f, 0.0f, // left vertex
		// rectangle
		-0.25f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f, // top left
		0.25f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f, // top right
		0.25f, 0.20f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.25f, 0.20f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		// square
		0.60f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, // top left
		0.90f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, // top right
        0.90f, 0.20f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
		0.60f, 0.20f, 0.0f, 0.0f, 0.0f, 1.0f // bottom left
		// end of first portion
		// second portion will contain circle, pentagon, and hexagon
		// second portions vertices will be generated dynamically
	};
    indices = {
		// separator rectangle indices
		0, 1, 2, // first triangle
		0, 2, 3,  // second triangle
        // first portion indices
		// triangle indices
		4, 5, 6, // triangle
		// rectangle indices
		7, 8, 9, // first triangle
        7, 9, 10, // second triangle
		// square indices
		11, 12, 13, // first triangle
		11, 13, 14  // second triangle
		// end of first portion indices
		// second portion will contain circle, pentagon, and hexagon
		// second portion indices will be generated dynamically
    };

    // circle generator
        // 1. Define circle properties
    const float cx = -0.6f;
    const float cy = -0.45f;
    const float radius = 0.25f;
    const int num_segments = 100;

    // This is the starting index for the new vertices.
    // We need this to correctly calculate the indices for the circle.
    const unsigned int base_index = vertices.size() / 6; // 6 floats per vertex (3 pos, 3 color)

    // 2. Generate vertices
    // Add the center vertex of the circle
    vertices.insert(vertices.end(), { cx, cy, 0.0f, 1.0f, 1.0f, 0.0f }); // Position and Color (Yellow)

    // Add the circumference vertices
    for (int i = 0; i <= num_segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        vertices.insert(vertices.end(), { x + cx, y + cy, 0.0f, 1.0f, 1.0f, 0.0f });
    }

    // 3. Generate indices
    // Create triangles by connecting the center to the circumference vertices
    for (int i = 1; i <= num_segments; ++i) {
        indices.push_back(base_index);      // Center vertex
        indices.push_back(base_index + i);
        indices.push_back(base_index + i + 1);
    }

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