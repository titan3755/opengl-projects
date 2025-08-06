#pragma once

#ifndef LOG_MANAGER_HPP
#define LOG_MANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>

class LogManager {
public: 
	LogManager(std::string tl);
	~LogManager() = default;

	void introLog();
	void getLog();
	void printLog();
	std::vector<std::string> returnParams();

private:
	std::string title;
	std::vector<const GLubyte*> params;
	std::vector<std::string> stringParams;
	std::vector<GLint> intParams;
	std::vector<std::string> intStringParams;
};

#endif