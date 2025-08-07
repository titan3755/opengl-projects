#include <log_manager.hpp>

LogManager::LogManager(std::string tl) : title(tl) {
	// Initialize OpenGL parameters
	getLog();
}	

void LogManager::getLog() {
	params.clear();
	params.push_back(glGetString(GL_VENDOR));
	params.push_back(glGetString(GL_RENDERER));
	params.push_back(glGetString(GL_VERSION));
	params.push_back(glGetString(GL_SHADING_LANGUAGE_VERSION));
	// Get string parameters (descriptions)
	stringParams.clear();
	stringParams.push_back("Vendor");
	stringParams.push_back("Renderer");
	stringParams.push_back("OpenGL Version");
	stringParams.push_back("Shading Language Version");
	
	// Get integer parameters
	intParams.clear();
	GLint maxTextureSize, maxVertexAttribs, maxVertexUniformComponents, 
		maxFragmentUniformComponents, maxCombinedTextureImageUnits, 
		maxDrawBuffers;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	intParams.push_back(maxTextureSize);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	intParams.push_back(maxVertexAttribs);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
	intParams.push_back(maxVertexUniformComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
	intParams.push_back(maxFragmentUniformComponents);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
	intParams.push_back(maxCombinedTextureImageUnits);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
	intParams.push_back(maxDrawBuffers);
	// Get integer parameter descriptions
	intStringParams.clear();
	intStringParams.push_back("Max Texture Size");
	intStringParams.push_back("Max Vertex Attribs");
	intStringParams.push_back("Max Vertex Uniform Components");
	intStringParams.push_back("Max Fragment Uniform Components");
	intStringParams.push_back("Max Combined Texture Image Units");
	intStringParams.push_back("Max Draw Buffers");
}

void LogManager::introLog() {
	std::cout << "|-------------------------------|" << std::endl;
	std::cout << title << std::endl;
	std::cout << "|-------------------------------|" << std::endl;
	std::cout << "Basic OpenGL Application" << std::endl;
	std::cout << "|-------------------------------|" << std::endl;
}

void LogManager::printLog() {
	std::cout << "OpenGL String Parameters:" << std::endl;
	for (size_t i = 0; i < params.size(); ++i) {
		std::cout << stringParams[i] << ": " << reinterpret_cast<const char*>(params[i]) << std::endl;
	}
	std::cout << "\nOpenGL Integer Parameters:" << std::endl;
	for (size_t i = 0; i < intParams.size(); ++i) {
		std::cout << intStringParams[i] << ": " << intParams[i] << std::endl;
	}
}

std::vector<std::string> LogManager::returnParams() {
	std::vector<std::string> allParams;
	for (const auto& param : stringParams) {
		allParams.push_back(param);
	}
	for (const auto& intParam : intStringParams) {
		allParams.push_back(intParam);
	}
	return allParams;
}