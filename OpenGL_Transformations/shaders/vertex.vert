#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec4 vertexColor;
out vec2 vertexTexCoord;
uniform mat4 transform;
void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	vertexColor = aColor;
	vertexTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}