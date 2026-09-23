#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];
	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* massage = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, massage);
		std::cout << "Failed to compile Shader!" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation failed: " << std::endl;
		std::cout << massage << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Modern OpenGL", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << '\n';

		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	float positions[] = {
	-0.5f, -0.5f,
	 0.5f,  -0.5f,
	 0.5f, 0.5f,
	 -0.5f, 0.5f,
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,0
	};

	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, std::size(indices));

	ShaderProgramSource source = parseShader("res/shaders/Basic.shader");

	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	assert(location != -1);
	GLCALL(glUniform4f(location, 1.0f, 0.3f, 0.8f, 1.0f));

	va.Bind();
	GLCALL(glUseProgram(0));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float red_component = 0.0f;
	float increment = 0.05f;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		GLCALL(glUseProgram(shader));
		GLCALL(glUniform4f(location, red_component, 0.3f, 0.8f, 1.0f));

		va.Bind();
		ib.Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (red_component > 1.0f)
			increment = -0.05f;
		else if (red_component < 0.0f)
			increment = 0.05f;

		red_component += increment;

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}