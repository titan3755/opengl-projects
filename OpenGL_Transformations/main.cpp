// ogl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <vector>

// local
#include <shader_manager.hpp>
#include <log_manager.hpp>

// img
#define STB_IMAGE_IMPLEMENTATION
#include <img/stb_image.h>

const int WIDTH = 1280;
const int HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Wave", nullptr, nullptr);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ShaderManager shaderManager("shaders/vertex.glsl", "shaders/fragment.glsl");
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<float> vertices = {
		// simple quad covering the screen
        // positions          // colors           // texture coords
        -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 -1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	LogManager logManager("OpenGL Wave");
	logManager.introLog();
	logManager.getLog();
	logManager.printLog();

	shaderManager.use();
    int freqXLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "freq_x");
    int freqYLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "freq_y");
    glUniform1f(freqXLocation, 20.0f);
    glUniform1f(freqYLocation, 20.0f);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        float currentTime = glfwGetTime();
        int timeLocation = glGetUniformLocation(shaderManager.getShaderProgram(), "u_time");
        glUniform1f(timeLocation, currentTime);

		// change frequency based on time
		float freqX = 20.0f + 10.0f * sin(currentTime) * 20.0f;
		float freqY = 20.0f + 10.0f * cos(currentTime) * 20.0f;
		glUniform1f(freqXLocation, freqX);
		glUniform1f(freqYLocation, freqY);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderManager.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}