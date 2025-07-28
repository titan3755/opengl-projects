// ogl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <vector>

// local
#include <shader_manager.hpp>

const int WIDTH = 800;
const int HEIGHT = 800;
const int BOARD_SIZE = 8;
const float SQUARE_SIZE = 2.0f / BOARD_SIZE;
const float BOARD_START_X = -1.0f;
const float BOARD_START_Y = -1.0f;

// fsc
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// main
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

    ShaderManager shaderManager("shaders/vertex.glsl", "shaders/fragment.glsl");
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // vertex and index gen
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int current_vertex_offset = 0;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            float x_square_bl = BOARD_START_X + col * SQUARE_SIZE;
            float y_square_bl = BOARD_START_Y + row * SQUARE_SIZE;
            float r, g, b;
            if ((row + col) % 2 == 0) {
                // w
                r = 1.0f; g = 1.0f; b = 1.0f;
            } else {
                // b
                r = 0.1f; g = 0.1f; b = 0.1f;
            }
            vertices.push_back(x_square_bl);
            vertices.push_back(y_square_bl);
            vertices.push_back(0.0f);
            vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
            vertices.push_back(x_square_bl + SQUARE_SIZE);
            vertices.push_back(y_square_bl);
            vertices.push_back(0.0f);
            vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
            vertices.push_back(x_square_bl + SQUARE_SIZE);
            vertices.push_back(y_square_bl + SQUARE_SIZE);
            vertices.push_back(0.0f);
            vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
            vertices.push_back(x_square_bl);
            vertices.push_back(y_square_bl + SQUARE_SIZE);
            vertices.push_back(0.0f);
            vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
            indices.push_back(current_vertex_offset + 0);
            indices.push_back(current_vertex_offset + 1);
            indices.push_back(current_vertex_offset + 2);
            indices.push_back(current_vertex_offset + 0);
            indices.push_back(current_vertex_offset + 2);
            indices.push_back(current_vertex_offset + 3);
            current_vertex_offset += 4;
        }
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
    std::cout << "OpenGL Basics initialized successfully!" << std::endl;

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
