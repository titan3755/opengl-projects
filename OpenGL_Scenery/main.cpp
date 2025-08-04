// ogl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// std
#include <iostream>
#include <vector>

// local
#include <shader_manager.hpp>

// img
#define STB_IMAGE_IMPLEMENTATION
#include <img/stb_image.h>

const int WIDTH = 1280;
const int HEIGHT = 720;

// fsc
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// main
int main() {

    std::cout << "OpenGL Scenery - Initializing..." << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Scenery", nullptr, nullptr);
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

    ShaderManager shaderManager("shaders/vertex.glsl", "shaders/fragment.glsl");
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // vertex and index gen
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    vertices = {
        // bg rect
        -1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f,  // bottom left
         1.0f, -1.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f,// bottom right
		 1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f,// top right
		-1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f,  // top left
		// foreground boxes (2 boxes / quads)
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,   0.0f, 0.0f, // 4
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,   1.0f, 0.0f, // 5
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,   1.0f, 1.0f, // 6
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,   0.0f, 1.0f  // 7
	};

    indices = {
		0, 1, 2, // first triangle
		0, 2, 3,  // second triangle
        4, 5, 6,
        6, 7, 4
    };

    // brick wall texture
	int bWidth, bHeight, bnrChannels;
	unsigned char* brickData = stbi_load("assets/red_brick_diff_4k.jpg", &bWidth, &bHeight, &bnrChannels, 0);
    if (!brickData) {
        std::cerr << "Failed to load texture" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
	}
	unsigned int brickTexture;
	glGenTextures(1, &brickTexture);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bWidth, bHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, brickData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(brickData);
    glBindTexture(GL_TEXTURE_2D, 0);

	// wooden box texture
	int wWidth, wHeight, wnrChannels;
	unsigned char* woodData = stbi_load("assets/wooden_garage_door_diff_4k.jpg", &wWidth, &wHeight, &wnrChannels, 0);
    if (!woodData) {
        std::cerr << "Failed to load texture" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
		return -1;
	}
	unsigned int woodTexture;
	glGenTextures(1, &woodTexture);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wWidth, wHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, woodData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(woodData);
	glBindTexture(GL_TEXTURE_2D, 0);

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
    shaderManager.use();

    // ogl info
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Scenery initialized successfully!" << std::endl;

    // query limits
	std::cout << "\nQuerying OpenGL limits..." << std::endl;
	std::vector<GLint> query_output(64);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &query_output[0]);
	std::cout << "Max Vertex Attributes: " << query_output[0] << std::endl;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &query_output[1]);
	std::cout << "Max Texture Size: " << query_output[1] << std::endl;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &query_output[2]);
	std::cout << "Max Combined Texture Image Units: " << query_output[2] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &query_output[3]);
	std::cout << "Max Vertex Texture Image Units: " << query_output[3] << std::endl;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &query_output[4]);
	std::cout << "Max Fragment Uniform Components: " << query_output[4] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &query_output[5]);
	std::cout << "Max Vertex Uniform Components: " << query_output[5] << std::endl;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &query_output[6]);
	std::cout << "Max Uniform Block Size: " << query_output[6] << std::endl;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &query_output[7]);
	std::cout << "Max Uniform Buffer Bindings: " << query_output[7] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &query_output[8]);
	std::cout << "Max Vertex Uniform Blocks: " << query_output[8] << std::endl;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &query_output[9]);
	std::cout << "Max Fragment Uniform Blocks: " << query_output[9] << std::endl;
	glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &query_output[10]);
	std::cout << "Max Combined Uniform Blocks: " << query_output[10] << std::endl;
	glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &query_output[11]);
	std::cout << "Max Combined Vertex Uniform Components: " << query_output[11] << std::endl;
	glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &query_output[12]);
	std::cout << "Max Combined Fragment Uniform Components: " << query_output[12] << std::endl;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &query_output[13]);
	std::cout << "Max Draw Buffers: " << query_output[13] << std::endl;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &query_output[14]);
	std::cout << "Max Color Attachments: " << query_output[14] << std::endl;
	glGetIntegerv(GL_MAX_SAMPLES, &query_output[15]);
	std::cout << "Max Samples: " << query_output[15] << std::endl;
	glGetIntegerv(GL_MAX_VIEWPORTS, &query_output[16]);
	std::cout << "Max Viewports: " << query_output[16] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &query_output[17]);
	std::cout << "Max Vertex Output Components: " << query_output[17] << std::endl;
	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &query_output[18]);
	std::cout << "Max Fragment Input Components: " << query_output[18] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &query_output[19]);
	std::cout << "Max Vertex Attrib Bindings: " << query_output[19] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &query_output[20]);
	std::cout << "Max Vertex Attrib Relative Offset: " << query_output[20] << std::endl;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &query_output[21]);
	std::cout << "Max Vertex Attrib Stride: " << query_output[21] << std::endl;


    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        shaderManager.use();
        glClearColor(0.7f, 0.5f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderManager.use();
        glBindVertexArray(VAO);
        // CORRECT
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        // Draw ONLY the first 6 indices for the background
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glBindTexture(GL_TEXTURE_2D, woodTexture);
        // Draw ONLY the next 6 indices for the rectangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
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