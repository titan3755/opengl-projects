#include <shader_manager.hpp>

ShaderManager::ShaderManager(std::string vertexShaderPath, std::string fragmentShaderPath)
	: vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath) {
	loadShaders();
}

void ShaderManager::loadShaders() {
	// Load vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream vertexFile(vertexShaderPath);
	if (!vertexFile) {
		std::cerr << "Failed to open vertex shader file: " << vertexShaderPath << std::endl;
		return;
	}
	std::string vertexCode((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
	const char* vertexShaderSource = vertexCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
		glDeleteShader(vertexShader);
		return;
	}
	// Load fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream fragmentFile(fragmentShaderPath);
	if (!fragmentFile) {
		std::cerr << "Failed to open fragment shader file: " << fragmentShaderPath << std::endl;
		return;
	}
	std::string fragmentCode((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());
	const char* fragmentShaderSource = fragmentCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		return;
	}
	// Create shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
		glDeleteProgram(shaderProgram);
		glDeleteShader
		(fragmentShader);
		glDeleteShader(vertexShader);
		return;
	}
	// Clean up shaders after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	std::cout << "Shaders loaded and compiled successfully." << std::endl;
}

unsigned int ShaderManager::getShaderProgram() const {
	return shaderProgram;
}

void ShaderManager::use() const {
	glUseProgram(shaderProgram);
}

unsigned int ShaderManager::getVertexShader() const {
	return vertexShader;
}

unsigned int ShaderManager::getFragmentShader() const {
	return fragmentShader;
}