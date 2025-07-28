#pragma once

#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

class ShaderManager {
public:
	ShaderManager(std::string vertexShaderPath, std::string fragmentShaderPath);
	~ShaderManager() = default;

	void loadShaders();
	unsigned int getShaderProgram() const;
	unsigned int getVertexShader() const;
	unsigned int getFragmentShader() const;
	void use() const;

private:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};

#endif